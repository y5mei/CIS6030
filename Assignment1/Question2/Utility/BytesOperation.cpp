//
// Created by Yao-Wen Mei on 2022/10/3.
//

#include <iostream>
#include <algorithm>
#include <bitset>

#include "BytesOperation.h"
using namespace std;

// printout a short as bits;
void coutShortToBits(short num) {
    bitset<16> x(num);
    cout << "The bits for " << num << " is: " << endl;
    cout << x << endl;
}

// printout a string of chars as bits;
void coutStringToBits(const string& str) {
    cout << "Convert str to bits:" << str << endl;
    for (char c: str) {
        bitset<8> x(c);
        cout << x << " ";
    }
    cout << "." << endl;
}
