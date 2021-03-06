//
//  main.cpp
//  Test
//
//  Created by neo on 15-10-28.
//  Copyright (c) 2015年 user. All rights reserved.
//

#include "rapidcheck.h"

#include "TestParserResult.h"
#include "TestCPSParser.h"
#include "TestStream.h"
#include "TestTokenStream.h"
#include <fstream>
#include <iostream>
using namespace std;
void TestLex();

int main() {
    
    TestParserResult();
    TestCPSParser();
    TestStream();
    TestLex();
    TestTokenStream();
    return 0;
}
