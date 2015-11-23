//
//  Util.h
//  fun-lua
//
//  Created by neo on 15-11-20.
//  Copyright (c) 2015年 user. All rights reserved.
//

#ifndef fun_lua_Util_h
#define fun_lua_Util_h

#include <string>
#include <iomanip>
#include <stdlib.h>
#include <sstream>
#include <iostream>
namespace util {
    
    template<class T>
    inline std::string TtoStr(T v)
    {
        
        std::stringstream TToStr;
        std::string tempStr;
        TToStr << v;
        TToStr >> tempStr;
        return tempStr;
    }
    
    inline std::string TtoStr(int v,int base = 10)
    {
        
        std::stringstream TToStr;
        std::string tempStr;
        TToStr << std::setbase(base) << v;
        TToStr >> tempStr;
        return tempStr;
    }
    inline std::string TtoStr(unsigned int v,int base = 10)
    {
        
        std::stringstream TToStr;
        std::string tempStr;
        TToStr << std::setbase(base) << v;
        TToStr >> tempStr;
        return tempStr;
    }
    inline std::string TtoStr(bool v)
    {
        if (v)
        {
            return "true";
        }
        else
        {
            return "false";
        }
    }
    
    template<class T>
    inline T StrTot(const std::string& v)
    {
        std::stringstream TToStr;
        T tempT;
        TToStr << v;
        TToStr >> tempT;
        return tempT;
    }
    
    template<>
    inline float StrTot(const std::string& v)
    {
        return (float)atof(v.c_str());
    }
    
    template<>
    inline bool StrTot(const std::string& v)
    {
        return (v =="true");
    }
    inline std::string bin2hex(const char* bin, unsigned int binLength)
    {
        static const char* hextable = "0123456789abcdef";
        
        unsigned int hexLength = binLength * 2 + 1;
        std::string hex;
        hex.resize(hexLength);
        
        int ci = 0;
        for (unsigned int i = 0; i < binLength; ++i)
        {
            unsigned char c = bin[i];
            hex[ci++] = hextable[(c >> 4) & 0x0f];
            hex[ci++] = hextable[c & 0x0f];
        }
        
        return hex;
    }
    
    inline int randomRange(int min,int max)
    {
        if (min == max)
        {
            return min;
        }
        else if (min < max) {
            int len = max - min;
            int seed = rand() % len;
            return seed + min;
        }
        else
        {
            return randomRange(max, min);
        }
        
        
    }
    template<class iterator>
    inline void RandomListRange(iterator begin,iterator end)
    {
        if (end - begin <= 1) {
            return;
        }
        
        for (iterator iter = begin; iter != (end-1); ++iter) {
            int next = randomRange(1, end - iter);
            std::swap(*(iter), *(iter+next));
        }
    }
}

#endif
