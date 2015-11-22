//
//  Lex.cpp
//  fun-lua
//
//  Created by neo on 15-11-20.
//  Copyright (c) 2015年 user. All rights reserved.
//

#include "Lex.h"
#include "KeywordsHelper.h"

namespace Parser{
    
#define CONSL(e1,r) CONS(Token,TexStream::PtrType ,e1,r)
#define CONSLF(e1,r) CONSF(Token,TexStream::PtrType ,e1,r)
#define RETL(e) RET(e,TexStream::PtrType)
#define RETLF(e) RETF(e,TexStream::PtrType)

    
    const LexType& defaultParser();

    const LexType& identifierParser();
    
    
    inline LexTypeChar::Result item(const  TexStream::PtrType& inp)
    {
        if (inp->empty()) {
            return CPS::None<char,TexStream::PtrType>("end of file");
        }
        else
        {
            return CPS::Some(inp->get(),inp->next());
        }
    }
    
    inline LexTypeChar satParser(std::function<bool(char)> f)
    {
        auto funx = [f](char x)->LexTypeChar{
            if (f(x)) {
                return LexTypeChar::ret(x);
            }
            else
            {
                return LexTypeChar::failure("");
            }
        };
        return CPS::Bind<char,char,TexStream::PtrType>({item,""}, funx);
    }
    inline LexTypeChar charParser(char c)
    {
        return satParser([c](char i){return c == i;});
    }
    
    const LexType& whiteSpaceParser()
    {
        static auto white = satParser([](char c){
           return c == ' ' || c == '\t';
        });
        static auto whiteToken = CPS::fmap<char,Token,TexStream::PtrType>(white, [](char c)->Token{
            Token t;
            t.t = c;
            return t;
        });
        return whiteToken;
    }
    const LexType& EOFPaser()
    {
        static LexType eof = {
            [](const  TexStream::PtrType& inp)->LexType::Result
            {
                if (inp->empty()) {
                    return CPS::Some<Token,TexStream::PtrType>(Token(tk_eof),inp);
                }
                else
                {
                    return  CPS::None<Token,TexStream::PtrType>("not end of file");
                }
            },
            ""
        };
        return eof;
    }
    LexType::Result parserToken(const TexStream::PtrType& inp )
    {
        static auto p = CPS::ChooseN<Token,TexStream::PtrType>({identifierParser(),whiteSpaceParser(),EOFPaser()});
        return p.fun(inp);
    }
    
    const LexType& identifierParser()
    {
        static auto firstChar = satParser([](char c){
            return c == '_' || isalnum(c);
        });
        static auto otherChars = CPS::Many(satParser([](char c){
            return isalnum(c);
        }));
        
        static auto idP = CONSLF(firstChar, c)
        CONSL(otherChars, cs)
        std::string id;
        id.push_back(c);
        id.insert(id.end(), cs.begin(), cs.end());
        Token t;
        auto keyWordT = lookupKeyword(id);
        if(keyWordT != tk_invalid) {
            t.t = keyWordT;
        }
        else
        {
            t.t = tk_identifier;
            t.value = id;
        }
        RETL(t);
        ENDCONS;
        ENDCONS;
        idP.label = "identifer";
        return idP;
    }
    const LexType& defaultParser()
    {
        static LexTypeChar itemP = {item};
        static auto d = CONSLF(itemP, ch)
        Token t((int)ch);
        RETL(t);
        ENDCONS;
        return d;
    }
    
    const CPS::CPSType<std::string, TexStream::PtrType>& numberParser(std::function<bool(char)> pred)
    {
        static auto hexParser = satParser(pred);
        static auto dotP = charParser('.');
        static auto hexsP1  = CPS::Many1(hexParser);
        static auto hexsP = CPS::fmap<std::list<char>,std::string,TexStream::PtrType>(hexsP1, [](const std::list<char>& ls)->std::string {
            return std::string(ls.begin(),ls.end());
        });
        static auto dotHexsP = CONSF(std::string, TexStream::PtrType, dotP, _)
        CONS(std::string, TexStream::PtrType, hexsP, hexs)
        std::string r(".");
        r.insert(r.end(), hexs.begin(),hexs.end());
        RET(r, TexStream::PtrType);
        ENDCONS;
        ENDCONS;
        static auto optionDotP = CPS::Option(dotHexsP);
        static auto hexsdothexsP = CONSF(std::string, TexStream::PtrType, hexsP, hex1)
        CONS(std::string, TexStream::PtrType, optionDotP, hex2)
        RET((std::string)(hex1 + hex2), TexStream::PtrType);
        ENDCONS;
        ENDCONS;
        static auto allHexP = CPS::Choice<std::string,TexStream::PtrType>(dotHexsP, hexsdothexsP);
        return allHexP;
    }

    const LexType& posHexnumberParser()
    {

        static const auto& allHexP = numberParser([](char c){
            return isnumber(c) || ('a' <= c && c <= 'f') ||('A' <= c && c <= 'F');
        });
        static auto xP = satParser([](char c){return c == 'x' || c == 'X';});
        static auto preFixAllHexp = CONSLF(charParser('0'),_)
        CONSL(xP,x)
        CONSL(allHexP, hexs)
        Token t;
        t.t = tk_number;
        t.value.push_back('0');
        t.value.push_back(x);
        t.value.insert(t.value.end(), hexs.begin(),hexs.end());
        RETL(t);
        ENDCONS;
        ENDCONS;
        ENDCONS;
        static std::string _ = preFixAllHexp.label = "parser hex number";
        return preFixAllHexp;
    }
    const LexType& numberParser()
    {
        static auto dig = satParser([](char c)->bool{
                                 return isnumber(c);
                             });
        static auto digs = CPS::Many1(dig);
        static auto digString = CPS::fmap<std::list<char>, Token, TexStream::PtrType>(digs, [](const std::list<char>& lchar)->Token{
            Token t;
            t.value = std::string(lchar.begin(),lchar.end());
            t.t = tk_number;
            return t;
        },"Parser digits");
        return digString;
    }
    
}