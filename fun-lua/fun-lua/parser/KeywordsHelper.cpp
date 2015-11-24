//
//  KeywordsHelper.cpp
//  fun-lua
//
//  Created by neo on 15-11-22.
//  Copyright (c) 2015年 user. All rights reserved.
//

#include "KeywordsHelper.h"
#include <map>

namespace Parser {
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
    
    
    std::string lookupTokenString(TokenType t)
    {
        for (auto iter = _keywords.begin(); iter != _keywords.end(); ++iter) {
            if (iter->second == t) {
                return iter->first;
            }
        }
        return "";
    }
    TokenType lookupKeyword(const std::string& keyword)
    {
        auto iter = _keywords.find(keyword);
        if (iter != _keywords.end()) {
            return iter->second;
        }
        return tk_invalid;
    }

}
