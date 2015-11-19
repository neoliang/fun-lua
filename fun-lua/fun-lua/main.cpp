//
//  main.cpp
//  fun-lua
//
//  Created by neo on 15-11-19.
//  Copyright (c) 2015年 user. All rights reserved.
//

#include <iostream>
#include "CPSParser.h"

inline Parser::ParserType<char,std::string>::Result item(const  std::string& inp)
{
    if (inp.empty()) {
        return Parser::None<char,std::string>("end of file");
    }
    else
    {
        return Parser::Some(inp.front(), inp.substr(1));
    }
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}
