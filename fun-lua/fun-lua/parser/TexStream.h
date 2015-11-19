//
//  TexStream.h
//  fun-lua
//
//  Created by neo on 15-11-19.
//  Copyright (c) 2015年 user. All rights reserved.
//

#ifndef __fun_lua__TexStream__
#define __fun_lua__TexStream__
#include <string>
#include <memory>

namespace Parser {
    class TexStream
    {
    public:
        typedef std::shared_ptr<TexStream> PtrType;
    public:
        virtual ~TexStream(){}
        
        virtual bool empty()const = 0;
        virtual char get()const = 0;
        virtual unsigned int lineNum()const = 0;
 
        virtual unsigned int colNum()const = 0;
        virtual PtrType next()const = 0;
        
    public:
        static PtrType fromFile(const std::string& file);
        static PtrType fromString(const std::string& file);
    };
}

#endif /* defined(__fun_lua__TexStream__) */
