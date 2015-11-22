//
//  KeywordsHelper.h
//  fun-lua
//
//  Created by neo on 15-11-22.
//  Copyright (c) 2015年 user. All rights reserved.
//

#ifndef __fun_lua__KeywordsHelper__
#define __fun_lua__KeywordsHelper__
#include "Token.h"

namespace Parser {
    TokenType lookupKeyword(const std::string& keyword);
}

#endif /* defined(__fun_lua__KeywordsHelper__) */
