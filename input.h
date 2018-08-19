//
//  input.h
//  search
//
//  Created by Young Kim on 9/9/17.
//  Copyright © 2017 Young Kim. All rights reserved.
//

#ifndef input_h
#define input_h

#include <vector>
#include <string>

struct Input {
    std::string type;
    int n;
    int p;
    std::vector<int> tree;
};

void readFile(std::string fileName, Input &input);
void writeFile(std::string fileName, std::string &output);

#endif /* input_h */
