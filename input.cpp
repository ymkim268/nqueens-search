//
//  input.cpp
//  search
//
//  Created by Young Kim on 9/9/17.
//  Copyright © 2017 Young Kim. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <stdlib.h> // atoi

#include "input.h"

using namespace std;

void readFile(string fileName, Input &input) {
    // open input file to read
    ifstream infile;
    infile.open(fileName.c_str(), fstream::in);
    
    // parse input file
    if(infile.is_open()) {
        string type, n, p;
        
        getline(infile, type); // instruction type
        getline(infile, n); // n x n square
        getline(infile, p); // num of lizards
        
        input.type = type;
        input.n = atoi(n.c_str()); // stoi(n) not working in vocareum
        input.p = atoi(p.c_str());
        
        int i = 0;
        // search tree loc
        for(string line; getline(infile, line);) {
            for(int j = 0; j < input.n; j++) {
                if(line[j] == '2') {
                    input.tree.push_back(i);
                }
                i++;
            }
        }
    } else {
        cout << "ERROR: Failed to open file!" << endl;
    }
    
    infile.close();
}


void writeFile(string fileName, string &output) {
    
    // open output file to write
    ofstream outfile(fileName.c_str());
    if(outfile.is_open()) {
        outfile << output;
    } else {
        cout << "ERROR: Failed to write file!" << endl;
    }
}
