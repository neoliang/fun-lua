//
//  TestCPSParser.h
//  fun-lua
//
//  Created by neo on 15-11-19.
//  Copyright (c) 2015年 user. All rights reserved.
//

#ifndef fun_lua_TestCPSParser_h
#define fun_lua_TestCPSParser_h

#include "rapidcheck.h"
#include "CPS.h"
#include <string>



inline CPS::CPSType<char,std::string>::Result item(const  std::string& inp)
{
    if (inp.empty()) {
        return CPS::None<char,std::string>("end of file");
    }
    else
    {
        return CPS::Some(inp.front(), inp.substr(1));
    }
}

template<typename T>
inline CPS::CPSType<T,std::string> satParser(std::function<bool(const T&)> f,const std::string& lable)
{
    auto funx = [f](const T& x)->CPS::CPSType<T,std::string>{
        if (f(x)) {
            return CPS::CPSType<T,std::string>::ret(x);
        }
        else
        {
            return CPS::CPSType<char,std::string>::failure("");
        }
    };
    return CPS::Bind<T,T,std::string>({item,lable}, funx);
}
inline CPS::CPSType<char,std::string> charParser(char c)
{
    std::string lable = "parser ";
    lable.push_back(c);
    return satParser<char>([c](char x)->bool {return c == x;},lable);
}

inline CPS::CPSType<std::string, std::string> strParser(const std::string& str)
{
    if (str.empty()) {
        return CPS::CPSType<std::string, std::string>::ret("");
    }
    std::string lable = "parser ";
    lable += str;
    auto x = charParser(str.front());
    auto xs = strParser(str.substr(1));
    return CPS::Bind<char, std::string, std::string>(x, [ xs,str](const char&)->CPS::CPSType<std::string, std::string> {
        return CPS::Bind<std::string, std::string, std::string>(xs,[str](const std::string&)->CPS::CPSType<std::string, std::string>{
            return CPS::CPSType<std::string, std::string>::ret(str);
        });
    },lable);
}

inline void TestCPSParser(){
    
    rc::check("item",[](const std::string& msg){
        auto r = item(msg);
        if (msg.empty()) {
            RC_ASSERT(r->isNone());
        }
        else
        {
            RC_ASSERT(r->value() == msg.front());
            RC_ASSERT(r->remain() == msg.substr(1));
        }
    });
    
    //many item
    rc::check("many item",[](const std::string& msg){
        auto p = CPS::Many<char, std::string>({item,""});
        if (!msg.empty()) {
            auto r = p.fun(msg);
            auto str = std::string(r->value().begin(),r->value().end());
            RC_ASSERT( str == msg);
            RC_ASSERT(r->remain() == "");
        }
    });
    //char
    rc::check("char",[](const char& c){
        auto p = charParser(c);
        std::string msg ;
        msg.push_back(c);
        
        auto r = item(msg);
        RC_ASSERT(r->value() == c);
    });
    //string
    rc::check("string",[](const std::string& msg){
        auto p = strParser(msg);
        auto r = p.fun(msg);
        RC_ASSERT( r->value() == msg);
        RC_ASSERT(r->remain() == "");
    });
}


#endif
