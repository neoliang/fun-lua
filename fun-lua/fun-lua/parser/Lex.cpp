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
    
    int hexavalue (char c) {
        if (isnumber(c)) return c - '0';
        else return tolower(c) - 'a' + 10;
    }
    
#define CONSL(e1,r) CONS(Token,TexStream::PtrType ,e1,r)
#define CONSLF(e1,r) CONSF(Token,TexStream::PtrType ,e1,r)
#define RETL(e) RET(e,TexStream::PtrType)
#define RETLF(e) RETF(e,TexStream::PtrType)
    
#define None(errMsg) CPS::None<Token, TexStream::PtrType>(errMsg)
#define SomeT(t,ninp) CPS::Some<Token,TexStream::PtrType>(t,ninp)
    
    std::string createErroMsg(const TexStream::PtrType& inp)
    {
        std::string errorMsg = "in line";
        errorMsg +=  util::TtoStr(inp->lineNum());
        errorMsg += " col";
        errorMsg += util::TtoStr(inp->colNum());
        return errorMsg;
    }
    const LexType& defaultParser();

    LexType::Result idp(const TexStream::PtrType& inp);
    
    
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
    bool isSpace(char c)
    {
        return  c == ' ' || c == '\t' || c == '\n' || c== 'r';
    }

    LexType::Result parserString (const TexStream::PtrType& inp) {
#define PStrError(e) None(std::string(e) + createErroMsg(inp))
        char del = inp->get();
        unsigned int current = 1;
        std::string str;
        char cChar = inp->lookAhead(current);
        while (cChar != del) {
            if (cChar == -1 || cChar == '\n' || cChar == '\r') {
                return PStrError("unfinished string end of file");
            }
            else if (cChar != '\\')
            {
                str.push_back(cChar);
                ++current;
            }
            else{
                ++current;
                char c = inp->lookAhead(current);
                bool saveValue = true;
                switch (c) {
                    case 'a': c = '\a'; break;
                    case 'b': c = '\b'; break;
                    case 'f': c = '\f'; break;
                    case 'n': c = '\n'; break;
                    case 'r': c = '\r'; break;
                    case 't': c = '\t'; break;
                    case 'v': c = '\v'; break;
                    case 'x':{
                        char l1 = inp->lookAhead(current+1);
                        char l2 = inp->lookAhead(current+2);
                        if (ishexnumber(l1) && ishexnumber(l2) ) {
                            c = (hexavalue(l1) << 4) + hexavalue(l2);
                        }
                        else
                        {
                            return PStrError("unfinished string hex");
                        }
                        current += 2;
                        break;
                    }
                    case -1:
                        return PStrError("unfinished string escaped end of file");
                    case '\\': case '\"': case '\'':
                        break;
                    case 'z': {  /* zap following span of spaces */
                        while (isSpace(inp->lookAhead(current+1))) {
                            ++current;
                        }
                        saveValue = false;
                        break;
                    }
                    default: {
                        
                        if (isnumber(c))
                        {
                            int n = 0;
                            for (int i = 0; i<3; ++i) {
                                char l1 = inp->lookAhead(current);
                                if (isnumber(l1)) {
                                    n = n* 10 + l1 - '0';
                                }
                                else{
                                    break;
                                }
                                ++current;
                            }
                            --current;
                            if (n >= UCHAR_MAX) {
                                return PStrError("decimal escape too large");
                            }
                            else
                            {
                                c = (char)n;
                            }
                        }
                        else{
                            return PStrError("unfinished string escaped number");
                        }
                        break;
                    }
                }
                if (saveValue) {
                    str.push_back(c);
                }
                
                ++current;
            }
            cChar = inp->lookAhead(current);
        }
        return SomeT(Token(tk_string,str), inp->next(current+1));
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
                return start;
            }
            return start + 1;
        }
    }

    LexType::Result parserToken(const TexStream::PtrType& inp )
    {
        if (inp->empty()) {
            return CPS::Some<Token,TexStream::PtrType>(Token(tk_eof),inp);
        }
        const char c = inp->get();
        if (isnumber(c)) {
            return numberParser(inp);
        }
        else if (c == '=' && inp->lookAhead(1) == '=')
        {
            return SomeT(Token(tk_equal),inp->next(2));
        }
        else if (c == '_' or isalpha(c))
        {
            return idp(inp);
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
                return numberParser(inp);
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
        else if (c == '-' && isdigit(inp->lookAhead(1)))
        {
            return numberParser(inp);
        }
        else if (c == '\'' || c == '\"')
        {
            return parserString(inp);
        }
        else//default
        {
            return CPS::Some<Token,TexStream::PtrType>(Token(c),inp->next());
        }
    }
    LexType::Result idp(const TexStream::PtrType& inp)
    {
        std::string id;
        id.push_back(inp->get());
        unsigned int i = 1;
        char nc = inp->lookAhead(i);
        while (nc == '_' || isalnum(nc)) {
            id.push_back(nc);
            ++i;
            nc = inp->lookAhead(i);
        }
        auto keyWordT = lookupKeyword(id);
        if(keyWordT != tk_invalid) {
            return SomeT(Token(keyWordT), inp->next(i));
        }
        else
        {
            return SomeT(Token(tk_identifier,id), inp->next(i));
        }
        
    }
    
    const CPS::CPSType<std::string, TexStream::PtrType>& digitsp(std::function<bool(char)> isDig)
    {
        const static CPS::CPSType<std::string, TexStream::PtrType> r = {
            [=] (const TexStream::PtrType& inp)->CPS::CPSType<std::string, TexStream::PtrType>::Result{
                std::string digits;
                unsigned int i = 0;
                char nc = inp->lookAhead(i);
                while (isDig(nc)) {
                    digits.push_back(nc);
                    ++i;
                    nc = inp->lookAhead(i);
                }
                return CPS::Some(digits, inp->next(i));
            },
            ""
        };
        return r;
    }
    
    CPS::CPSType<std::string, TexStream::PtrType> posnumberP(std::function<bool(char)> isDig,std::function<bool(char)> isE)
    {
        const static auto optiondig = CPS::Option(digitsp(isDig));
        auto dot = satParser([](char c){return c == '.';});
        const static auto dotDig = CONSF(std::string, TexStream::PtrType,dot , _)
        CONS(std::string, TexStream::PtrType, digitsp(isDig), dig)
        auto dotdigs  = std::string(".") + dig;
        RET(dotdigs, TexStream::PtrType);
        ENDCONS;
        ENDCONS;
        const static auto optionDotDig = CPS::Option(dotDig);
        
        static const auto eP = satParser(isE);
        static const auto optionSign = CPS::Option(satParser([](char c){return c == '+' || c == '-';}));
        static const auto eNP = CONSF(std::string, TexStream::PtrType, eP, e)
        CONS(std::string, TexStream::PtrType, optionSign, sig)
        CONS(std::string, TexStream::PtrType,digitsp(isDig), ePart)
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
        const static auto optionE = CPS::Option(eNP);
        
        CONS(std::string, TexStream::PtrType,optiondig,intPart)
        CONS(std::string, TexStream::PtrType,optionDotDig,dotFloat)
        CONS(std::string, TexStream::PtrType,optionE,ePart)
        if (intPart == "" && dotFloat == "") {
            return CPS::CPSType<std::string, TexStream::PtrType>::failure("parser number error");
        }
        else
        {
            RET(intPart + dotFloat + ePart, TexStream::PtrType);
        }
        ENDCONS;
        ENDCONS;
        ENDCONS;
    }

    LexType::Result numberParser(const TexStream::PtrType& inp)
    {
        const static auto posHexP = posnumberP(ishexnumber, [](char c){return c == 'P' || c == 'p';});
        const static auto posDecP = posnumberP(isdigit,[](char c){return c == 'E' || c == 'e';});
        bool isNeg = false;
        bool isHex = false;
        int offset = 0;
        if (inp->get() == '-') {
            isNeg = true;
            ++offset;
        }
        if (inp->lookAhead(offset) == '0' && (inp->lookAhead(offset + 1) == 'x' || inp->lookAhead(offset + 1) == 'X') ) {
            isHex = true;
            offset += 2;
        }
        CPS::CPSType<std::string, TexStream::PtrType>::Result r = nullptr;
        if (isHex) {
            r = posHexP.fun(inp->next(offset));
        }
        else
        {
            r = posDecP.fun(inp->next(offset));
        }
        if (r->isNone()) {
            return None(r->errorMsg());
        }
        std::string strVal;
        if (isNeg) {
            strVal.push_back('-');
        }
        if (isHex) {
            strVal.push_back('0');
            strVal.push_back('x');
        }
        strVal += r->value();
        return SomeT(Token(tk_number,strVal), r->remain());
    }
    
}