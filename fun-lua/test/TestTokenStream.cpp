//
//  TestTokenStream.cpp
//  fun-lua
//
//  Created by neo on 15-11-21.
//  Copyright (c) 2015年 user. All rights reserved.
//

#include "TestTokenStream.h"
#include "TokenStream.h"
#include "rapidcheck.h"
void TestTokenStream()
{
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
        auto tstream = Parser::TokenStream::create(stream);
        
        std::vector<Parser::Token> tokens;
        while (!tstream->empty()) {
            tokens.push_back(tstream->get());
            tstream = tstream->next();
        }

        RC_ASSERT(tokens.size() == nks.size() * 2);
        for (unsigned int i =0; i<nks.size(); ++i) {
            auto idx = 2 * i;
            auto idx1 = idx + 1;
            RC_ASSERT( Parser::lookupKeyword(nks[i])  == tokens[idx].t);
            RC_ASSERT(tokens[idx1].t == ' ');
        }
    }
    
}