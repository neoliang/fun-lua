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
#include <stdio.h>
#include <iostream>
inline void TestFileStream()
{
    FILE *fp = fopen("/tmp/test.lua", "r");
    fseek(fp,0,SEEK_END);
    long s = ftell(fp);
    fseek(fp,0,SEEK_SET);
    char *pBuffer  = new  char[s];
    s = fread(pBuffer,sizeof(char), s,fp);
    fclose(fp);
    std::string str(pBuffer,pBuffer +s);
    
    auto fstream = Parser::TexStream::fromFile("/tmp/test.lua");
    std::string str1;
    while (!fstream->empty()) {
        str1.push_back(fstream->get());
        fstream = fstream->next();
    }
    RC_ASSERT(str1 == str);
}

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
    TestFileStream();
}

#endif
