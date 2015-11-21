//
//  TokenStream.cpp
//  fun-lua
//
//  Created by neo on 15-11-20.
//  Copyright (c) 2015年 user. All rights reserved.
//

#include "TokenStream.h"
namespace Parser{
    bool TokenStream::empty()const
    {
        return get().t == tk_eof;
    }
    const Token& TokenStream:: get()const
    {
        while (_cpos >= _tokens->size()) {
            auto r = parserToken(_textStream);
            (*_tokens).push_back(r->value());
            _textStream = r->remain();
        }
        return (*_tokens)[_cpos];
    }
    
    TokenStream::PtrType TokenStream::next()const
    {
        auto temp = new TokenStream(*this);
        temp->_cpos += 1;
        return PtrType(temp);
    }
}