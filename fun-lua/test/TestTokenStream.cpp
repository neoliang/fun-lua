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
#include "KeywordsHelper.h"
using namespace std;
void TestTokenStream()
{
    
    auto fstream = Parser::TexStream::fromFile("/tmp/test.lua");
    auto tstream = Parser::TokenStream::create(fstream);
    std::vector<Parser::Token> alltokens;
    while (!tstream->empty()) {
        alltokens.push_back(tstream->get());
        tstream = tstream->next();
    }
    
    for (auto iter = alltokens.begin(); iter != alltokens.end(); ++iter) {
        if (iter->t < 256) {
            cout << (char)iter->t ;
        }
        if (iter->t == Parser::tk_string) {
            cout << "\"" << iter->value << "\"";
        }
        else{
            std::string str = Parser::lookupTokenString((Parser::TokenType)iter->t);
            if (str != "") {
                cout << str ;
            }
            else
            {
                cout << iter->value;
            }
        }
    }
    
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