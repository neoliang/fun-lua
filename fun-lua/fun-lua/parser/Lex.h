//
//  Lex.h
//  fun-lua
//
//  Created by neo on 15-11-20.
//  Copyright (c) 2015年 user. All rights reserved.
//

#ifndef __fun_lua__Lex__
#define __fun_lua__Lex__

#include "CPS.h"
#include "TexStream.h"
#include "Token.h"
namespace Parser{
    
    typedef CPS::CPSType<Token, TexStream::PtrType> LexType;
    typedef CPS::CPSType<char, TexStream::PtrType> LexTypeChar;
    
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
    
    inline LexTypeChar satParser(std::function<bool(char)> f,const std::string& lable)
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
        return CPS::Bind<char,char,TexStream::PtrType>({item,lable}, funx);
    }
    
    const LexType& digitParserFun();
}

#endif /* defined(__fun_lua__Lex__) */
