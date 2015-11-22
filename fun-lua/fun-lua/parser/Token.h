//
//  Token.h
//  fun-lua
//
//  Created by neo on 15-11-20.
//  Copyright (c) 2015年 user. All rights reserved.
//

#ifndef __fun_lua__Token__
#define __fun_lua__Token__
#include <string>

namespace Parser {
    enum TokenType
    {
        tk_invalid = -1,
        //0 ~ 255 single char
        //keywords
        tk_and = 256,tk_not,tk_or,tk_local,tk_return,tk_function,tk_end,tk_do,tk_break,tk_if,tk_then,
        tk_else,tk_elseif,tk_false,tk_true,tk_nil,tk_for,tk_in,tk_repeat,tk_until,tk_while,
        
        //operator
        tk_equal,
        tk_notequal,
        tk_le , //<=
        tk_ge ,
        tk_concat,
        tk_3dots,
        //others
        tk_number,
        tk_string,
        tk_identifier,
        tk_eof,
    };
    struct Token
    {
        int t;
        std::string value;
        
        Token(){}
        Token(int vt,const std::string& v="")
        :t(vt),value(v){};
    };
}


#endif /* defined(__fun_lua__Token__) */
