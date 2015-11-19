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
#include <fstream>
#include <iostream>
using namespace std;


int main() {
    TestParserResult();
    TestCPSParser();
    TestStream();
    return 0;
}
