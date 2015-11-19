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

inline CPS::CPSType<char,std::string>::Fun satParser(std::function<bool(char)> f)
{
    auto funx = [f](char x)->typename CPS::CPSType<char,std::string>::Fun{
        if (f(x)) {
            return CPS::CPSType<char,std::string>::ret(x);
        }
        else
        {
            std::string msg = "parser ";
            msg.push_back(x);
            msg += "error";
            
            return CPS::CPSType<char,std::string>::failure("");
        }
    };
    return CPS::Bind<char,char,std::string>(item, funx);
}
inline CPS::CPSType<char,std::string>::Fun charParser(char c)
{
    return satParser([c](char x)->bool {return c == x;});
}

inline CPS::CPSType<std::string, std::string>::Fun strParser(const std::string& str)
{
    if (str.empty()) {
        return CPS::CPSType<std::string, std::string>::ret("");
    }
    auto x = charParser(str.front());
    auto xs = strParser(str.substr(1));
    return CPS::Bind<char, std::string, std::string>(x, [ xs,str](const char&)->CPS::CPSType<std::string, std::string>::Fun {
        return CPS::Bind<std::string, std::string, std::string>(xs,[str](const std::string&)->CPS::CPSType<std::string, std::string>::Fun{
            return CPS::CPSType<std::string, std::string>::ret(str);
        });
    });
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
        auto p = CPS::Many<char, std::string>(item);
        if (!msg.empty()) {
            auto r = p(msg);
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
        auto r = p(msg);
        RC_ASSERT( r->value() == msg);
        RC_ASSERT(r->remain() == "");
    });
}


#endif
