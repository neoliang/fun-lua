//
//  Lex.cpp
//  fun-lua
//
//  Created by neo on 15-11-20.
//  Copyright (c) 2015年 user. All rights reserved.
//

#include "Lex.h"
namespace Parser{
    
    const LexType& digitParserFun()
    {
        static auto dig = satParser([](char c)->bool
                             {
                                 return isnumber(c);
                             }, "");
        static auto digs = CPS::Many1(dig);
        static auto digString = CPS::fmap<std::list<char>, Token, TexStream::PtrType>(digs, [](const std::list<char>& lchar)->Token{
            Token t;
            t.value = std::string(lchar.begin(),lchar.end());
            t.t = tk_number;
            return t;
        },"Parser digits");
        return digString;
    }
    
}