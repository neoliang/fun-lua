//
//  TestLex.cpp
//  fun-lua
//
//  Created by neo on 15-11-20.
//  Copyright (c) 2015年 user. All rights reserved.
//

#include "Lex.h"
#include "rapidcheck.h"
#include "Util.h"

void TestLex(){
    
    rc::check("digits",[](const unsigned int& i){
        auto str = util::TtoStr(i);
        
        auto r = Parser::digitParserFun().fun(Parser::TexStream::fromString(str));
        RC_ASSERT( r->value().t == Parser::tk_number);
        RC_ASSERT(r->value().value == str);
        RC_ASSERT(r->remain()->empty());
        
    });
    
}