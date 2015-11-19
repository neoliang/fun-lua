//
//  TestStream.h
//  fun-lua
//
//  Created by neo on 15-11-19.
//  Copyright (c) 2015年 user. All rights reserved.
//

#ifndef fun_lua_TestStream_h
#define fun_lua_TestStream_h
#include "TexStream.h"

inline void TestStream(){
    
    rc::check("stream",[](const std::string& msg){
        auto s = Parser::TexStream::fromString(msg);
        auto iter = msg.begin();
        while(iter != msg.end())
        {
            RC_ASSERT(s->get() == *iter);
            s = s->next();
            ++iter;
        }
    });
}

#endif
