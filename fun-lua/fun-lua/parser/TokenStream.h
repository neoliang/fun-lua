//
//  TokenStream.h
//  fun-lua
//
//  Created by neo on 15-11-20.
//  Copyright (c) 2015年 user. All rights reserved.
//

#ifndef __fun_lua__TokenStream__
#define __fun_lua__TokenStream__
#include "TexStream.h"
#include "Token.h"
#include "Lex.h"
#include <memory>
#include <vector>

namespace Parser {
    
    class TokenStream{
        mutable TexStream::PtrType _textStream;
        mutable std::shared_ptr<std::vector<Token>> _tokens;
        unsigned int _cpos;
        
    public:
        TokenStream(TexStream::PtrType textStream)
        :_textStream(textStream)
        {
            _tokens = std::shared_ptr<std::vector<Token>>(new std::vector<Token>());
            _cpos = 0;
        }

    public:
        typedef std::shared_ptr<TokenStream> PtrType;
    public:
        
        bool empty()const;
        const Token& get()const;
        PtrType next()const;
        
        static PtrType create(TexStream::PtrType textStream)
        {
            return PtrType(new TokenStream(textStream));
        }
        
    };
}

#endif /* defined(__fun_lua__TokenStream__) */
