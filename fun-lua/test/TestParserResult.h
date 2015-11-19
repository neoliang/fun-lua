//
//  TestParserResult.h
//  fun-lua
//
//  Created by neo on 15-11-19.
//  Copyright (c) 2015年 user. All rights reserved.
//

#ifndef fun_lua_TestParserResult_h
#define fun_lua_TestParserResult_h

#include "CPSResult.h"
#include "rapidcheck.h"


inline void TestParserResult(){
    rc::check("None",[](const std::string& msg){
        auto n = CPS::None<int, int>(msg);
        RC_ASSERT(n->isNone());
        RC_ASSERT(n->errorMsg() == msg);
        try {
            auto v = n->value();
            RC_ASSERT(v && false);
        } catch (std::exception e) {
            RC_ASSERT(true);
        }
        try {
            auto z = n->remain();
            RC_ASSERT(z && false);
        } catch (std::exception) {
            RC_ASSERT(true);
        }
    });
    
    rc::check("Some", [ ](const int x,const bool b){
        auto v = std::shared_ptr<bool>(new bool(b));
        auto n = CPS::Some(x, v);
        RC_ASSERT(!n->isNone());
        RC_ASSERT(n->value() == x);
        RC_ASSERT(*n->remain() == b);
    });
}
#endif
