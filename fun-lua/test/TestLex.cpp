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
#include <vector>

void TestLex(){
    
    rc::check("digits",[](const unsigned int& i){
        auto str = util::TtoStr(i);
        
        auto r = Parser::numberParser().fun(Parser::TexStream::fromString(str));
        RC_ASSERT( r->value().t == Parser::tk_number);
        RC_ASSERT(r->value().value == str);
        RC_ASSERT(r->remain()->empty());
        
    });
    
    //check identifer
    std::vector<std::string> keywords = {
        "and", "break", "do", "else", "elseif",
        "end", "false", "for", "function", "if",
        "in", "local", "nil", "not", "or", "repeat",
        "return", "then", "true", "until", "while",
    };
    for (int i = 0; i<10; ++i) {
        std::vector<std::string> nks = keywords;
        std::random_shuffle(nks.begin(), nks.end());
        std::string str;
        for (auto iter = nks.begin(); iter != nks.end(); ++iter) {
            str += *iter;
            str += " ";
        }
        auto stream = Parser::TexStream::fromString(str);
        std::vector<Parser::Token> tokens;
        auto r = parserToken(stream);
        tokens.push_back(r->value());
        while (r->value().t != Parser::tk_eof) {
            r = parserToken(r->remain());
            tokens.push_back(r->value());
        }
        RC_ASSERT(tokens.size() == nks.size() * 2 + 1);
        RC_ASSERT(tokens.back().t == Parser::tk_eof);
        for (unsigned int i =0; i<nks.size(); ++i) {
            auto idx = 2 * i;
            auto idx1 = idx + 1;
            RC_ASSERT( Parser::lookupKeyword(nks[i])  == tokens[idx].t);
            RC_ASSERT(tokens[idx1].t == ' ');
        }
    }
    
}