//
//  Lex.cpp
//  fun-lua
//
//  Created by neo on 15-11-20.
//  Copyright (c) 2015年 user. All rights reserved.
//

#include "Lex.h"
#include "KeywordsHelper.h"
#include "Util.h"
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
#define None(errMsg) CPS::None<Token, TexStream::PtrType>(errMsg)
    LexType::Result paserString (const TexStream::PtrType& inp) {
        char del = inp->get();
        unsigned int current = 1;
        
        char cChar = inp->lookAhead(current);
        while (cChar != del) {
            switch (cChar) {
                case -1:
                    None("unfinished string");
                    break;  /* to avoid warnings */
                case '\n':
                case '\r':
                    None("unfinished string");
                    break;  /* to avoid warnings */
                case '\\': {  /* escape sequences */
                    ++ current;
                    int c = inp->lookAhead(current);  /* final character to be saved */
                    switch (c) {
                        case 'a': c = '\a'; goto read_save;
                        case 'b': c = '\b'; goto read_save;
                        case 'f': c = '\f'; goto read_save;
                        case 'n': c = '\n'; goto read_save;
                        case 'r': c = '\r'; goto read_save;
                        case 't': c = '\t'; goto read_save;
                        case 'v': c = '\v'; goto read_save;
                        case 'x': c = readhexaesc(ls); goto read_save;
                        case '\\': case '\"': case '\'':
                            c = ls->current; goto read_save;
                        case EOZ: goto no_save;  /* will raise an error next loop */
                        case 'z': {  /* zap following span of spaces */
                            next(ls);  /* skip the 'z' */
                            while (lisspace(ls->current)) {
                                if (currIsNewline(ls)) inclinenumber(ls);
                                else next(ls);
                            }
                            goto no_save;
                        }
                        default: {
                            if (!lisdigit(ls->current))
                                escerror(ls, &ls->current, 1, "invalid escape sequence");
                            /* digital escape \ddd */
                            c = readdecesc(ls);
                            goto only_save;
                        }
                    }
                read_save: next(ls);  /* read next character */
                only_save: save(ls, c);  /* save 'c' */
                no_save: break;
                }
                default:
                    save_and_next(ls);
            }
        }
        save_and_next(ls);  /* skip delimiter */
        seminfo->ts = luaX_newstring(ls, luaZ_buffer(ls->buff) + 1,
                                     luaZ_bufflen(ls->buff) - 2);
    }
    std::string createErroMsg(const TexStream::PtrType& inp)
    {
        std::string errorMsg = "in line";
        errorMsg +=  util::TtoStr(inp->lineNum());
        errorMsg += " col";
        errorMsg += util::TtoStr(inp->colNum());
        return errorMsg;
    }
    int processComment(const TexStream::PtrType& inp)
    {
        //long comment
        if (inp->lookAhead(2) == '[' && inp->lookAhead(3) == '[') {
            unsigned int start = 4;
            char nc = inp->lookAhead(start);
            while (nc != -1) {
                
                if (nc == ']' && inp->lookAhead(start+1) == ']') {
                    ++start;
                    break;
                }
                ++start;
                nc = inp->lookAhead(start);
            }
            if (nc == -1) {
                return -1;
            }

            return start+1;
        }
        else
        {
            unsigned int start = 2;
            char nc = inp->lookAhead(start);
            while (nc != -1) {
                
                if (nc == '\n') {
                    break;
                }
                ++start;
                nc = inp->lookAhead(start);
            }
            if (nc == -1) {
                return -1;
            }
            return start + 1;
        }
    }
#define SomeT(t,ninp) CPS::Some<Token,TexStream::PtrType>(t,ninp)
    LexType::Result parserToken(const TexStream::PtrType& inp )
    {
        if (inp->empty()) {
            return CPS::Some<Token,TexStream::PtrType>(Token(tk_eof),inp);
        }
        const char c = inp->get();
        if (isnumber(c)) {
            return numberParser().fun(inp);
        }
        else if (c == '=' && inp->lookAhead(1) == '=')
        {
            return SomeT(Token(tk_equal),inp->next(2));
        }
        else if (c == '_' or isalpha(c))
        {
            return identifierParser().fun(inp);
        }
        else if (c == '~' && inp->lookAhead(1) == '=')
        {
            return SomeT(Token(tk_notequal),inp->next(2));
        }
        else if (c == '<' && inp->lookAhead(1) == '=')
        {
            return SomeT(Token(tk_le),inp->next(2));
        }
        else if (c == '>' && inp->lookAhead(1) == '=')
        {
            return SomeT(Token(tk_ge),inp->next(2));
        }
        else if (c == '.')
        {
            char l1 = inp->lookAhead(1);
            char l2 = inp->lookAhead(2);
            if ( l1 == '.') {
                if (l2 == '.') {
                    return SomeT(Token(tk_3dots),inp->next(3));
                }
                else
                {
                    return SomeT(Token(tk_concat), inp->next(2));
                }
            }
            else if(isnumber(l1))
            {
                return numberParser().fun(inp);
            }
            else
            {
                return SomeT(Token('.'),inp->next());
            }
        }
        else if (c == '-' && inp->lookAhead(1) == '-') //ignore comment
        {
            int offset = processComment(inp);
            if (offset == -1) {
                std::string errMsg = "unexpected comment " + createErroMsg(inp);
                return CPS::None<Token, TexStream::PtrType>(errMsg);
            }
            else
            {
                return parserToken(inp->next(offset));
            }
        }
        else//default
        {
            return CPS::Some<Token,TexStream::PtrType>(Token(c),inp->next());
        }
    }
    
    const LexType& identifierParser()
    {
        static auto firstChar = satParser([](char c){
            return c == '_' || isalpha(c);
        });
        static auto otherChars = CPS::Many(satParser([](char c){
            return isalnum(c) || c == '_';
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
    

    const CPS::CPSType<std::string, TexStream::PtrType>& numberParser(std::function<bool(char)> pred)
    {
        static auto hexParser = satParser(pred);
        
        static auto hexsP  = CPS::Many1(hexParser);
        static auto dotP = charParser('.');
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
        std::string hex1Str(hex1.begin(),hex1.end());
        RET((std::string)(hex1Str + hex2), TexStream::PtrType);
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
        return preFixAllHexp;
    }
    
    const LexType& posNumberParser()
    {
        
        static const auto& numberP = numberParser([](char c){return isnumber(c);});
        static const auto& digitP =  CPS::Many1(satParser([](char c){return isnumber(c);}));
        static auto eP = satParser([](char c){return c == 'e' || c == 'E';});
        static auto optionSign = CPS::Option(satParser([](char c){return c == '+' || c == '-';}));
        static auto eNP = CONSF(std::string, TexStream::PtrType, eP, e)
        CONS(std::string, TexStream::PtrType, optionSign, sig)
        CONS(std::string, TexStream::PtrType,digitP, ePart)
        std::string r;
        r.push_back(e);
        if (sig == '+' || sig == '-') {
            r.push_back(sig);
        }
        r.insert(r.end(), ePart.begin(),ePart.end());
        RET(r, TexStream::PtrType);
        ENDCONS;
        ENDCONS;
        ENDCONS;
        static auto optionENP = CPS::Option(eNP);
        static auto preFixAllHexp = CONSLF(numberP,numbers)
        CONSL(optionENP,es)
        Token t;
        t.t = tk_number;
        t.value = numbers + es;
        RETL(t);
        ENDCONS;
        ENDCONS;
        return preFixAllHexp;
    }
    
    const LexType& numberParser()
    {
        
        const static LexType posNumber = {[](const TexStream::PtrType& inp)->LexType::Result{
            char ahead1 = inp->lookAhead(1);
            char cChar = inp->lookAhead(0);
            if (cChar == '0' &&  (ahead1 == 'x' || ahead1 == 'X') ) {
                return posHexnumberParser().fun(inp);
            }
            else
            {
                return posNumberParser().fun(inp);
            }
            
        },""};
        const static auto optionSign = CPS::Option(satParser([](char c){ return c == '-';}));
        static auto np = CONSLF(optionSign, s)
        CONSL(posNumber, nt)
        if (s == '-' ) {
            Token nnt;
            nnt.t = tk_number;
            nnt.value = "-";
            nnt.value += nt.value;
            RETL(nnt);
        }
        else
        {
            RETL((Token)nt);
        }
        ENDCONS;
        ENDCONS;
        static std::string _ = np.label = "parser number";
        return np;
    }
    
}