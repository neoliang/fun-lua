//
//  Lex.cpp
//  fun-lua
//
//  Created by neo on 15-11-20.
//  Copyright (c) 2015年 user. All rights reserved.
//

#include "Lex.h"
#include <map>

namespace Parser{
    
#define CONSL(e1,r) CONS(Token,TexStream::PtrType ,e1,r)
#define CONSLF(e1,r) CONSF(Token,TexStream::PtrType ,e1,r)
#define RETL(e) RET(e,TexStream::PtrType)
#define RETLF(e) RETF(e,TexStream::PtrType)
    //tk_and = 256,tk_not,tk_or,tk_local,tk_return,tk_function,tk_end,tk_do,tk_break,tk_if,tk_then,
    //tk_else,tk_elseif,tk_false,tk_true,tk_nil,tk_for,tk_in,tk_repeat,tk_until,tk_while,
    const static std::map<std::string, TokenType> _keywords = {
        {"and",tk_and},
        {"not",tk_not},
        {"or",tk_or},
        {"local",tk_local},
        {"return",tk_return},
        {"function",tk_function},
        {"end",tk_end},
        {"do",tk_do},
        {"break",tk_break},
        {"if",tk_if},
        {"then",tk_then},
        {"else",tk_else},
        {"elseif",tk_elseif},
        {"false",tk_false},
        {"true",tk_true},
        {"nil",tk_nil},
        {"for",tk_for},
        {"in",tk_in},
        {"repeat",tk_repeat},
        {"until",tk_until},
        {"while",tk_while},
    };
    
    TokenType lookupKeyword(const std::string& keyword)
    {
        return _keywords.find(keyword)->second;
    }
    const LexType& identifierParser();
    
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
            auto iter = _keywords.find(id);
            if(iter != _keywords.end()) {
                t.t = iter->second;
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
    const LexType& numberParser()
    {
        static auto dig = satParser([](char c)->bool
                             {
                                 return isnumber(c);
                             }, "");
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