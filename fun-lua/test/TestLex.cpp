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
#include "KeywordsHelper.h"
void TestLex(){
    
    //test HexnumberParser
    rc::check("HexnumberParser",[](const unsigned int& i,const unsigned int& d,const bool& sign){
        const bool hex =true;
        std::string hexStr;
        int base = 10;
        if (hex) {
            hexStr = "0x";
            base = 16;
        }
        if (i != 0 ) {
             hexStr += util::TtoStr(i,base);
        }
        std::string digit;
        if (d != 0) {
            hexStr += ".";
            hexStr += util::TtoStr(d,base);
        }
        if (sign) {
            hexStr = "-" + hexStr;
        }
        if (hexStr == "") {
            return ;
        }
        auto stream = Parser::TexStream::fromString(hexStr);
        auto r = Parser::numberParser().fun(stream);
        if (i == 0 && d == 0 ) {
            RC_ASSERT(r->isNone());
        }
        else{
            RC_ASSERT(!r->isNone());
            RC_ASSERT(r->value().value == hexStr);
        }
    });
    
    rc::check("numberParser",[](const unsigned int& i,const unsigned int& d,const bool& sign){
        const bool hex =false;
        std::string hexStr;
        int base = 10;
        if (hex) {
            hexStr = "0x";
            base = 16;
        }
        if (i != 0 ) {
            hexStr += util::TtoStr(i,base);
        }
        std::string digit;
        if (d != 0) {
            hexStr += ".";
            hexStr += util::TtoStr(d,base);
        }
        if (sign) {
            hexStr = "-" + hexStr;
        }
        if (hexStr == "") {
            return ;
        }
        auto stream = Parser::TexStream::fromString(hexStr);
        auto r = Parser::numberParser().fun(stream);
        if (i == 0 && d == 0 ) {
            RC_ASSERT(r->isNone());
        }
        else{
            RC_ASSERT(!r->isNone());
            RC_ASSERT(r->value().value == hexStr);
        }
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