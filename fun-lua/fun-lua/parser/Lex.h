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
    

    const LexType& posHexnumberParser();
    
    const LexType& numberParser();
    LexType::Result parserToken(const TexStream::PtrType& inp );

}

#endif /* defined(__fun_lua__Lex__) */
