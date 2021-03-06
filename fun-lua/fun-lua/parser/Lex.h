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
    
    LexType::Result numberParser(const TexStream::PtrType& inp);
    LexType::Result parserToken(const TexStream::PtrType& inp );
    
    inline LexType::Result parserTokenString(const std::string& str)
    {
        auto stream = TexStream::fromString(str);
        return parserToken(stream);
    }

}

#endif /* defined(__fun_lua__Lex__) */
