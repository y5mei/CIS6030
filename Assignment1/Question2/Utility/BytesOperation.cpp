//
// Created by Yao-Wen Mei on 2022/10/3.
//

#include <iostream>
#include <bitset>
#include <functional> //for std::hash

#include "BytesOperation.h"

using namespace std;

// printout a short as bits;
void coutShortToBits(short num) {
    bitset<16> x(num);
    cout << "The bits for " << num << " is: " << endl;
    cout << x << endl;
}

// printout a string of chars as bits;
void coutStringToBits(const string &str) {
    cout << "Convert str to bits:" << str << endl;
    for (char c: str) {
        bitset<8> x(c);
        cout << x << " ";
    }
    cout << "." << endl;
}

unsigned short convertStringToShort(const string &key) {
    std::hash<std::string> hasher;
    unsigned long hashed = hasher(key); //returns std::size_t
    return hashed % 701;
}

unsigned short getRightMostIbits(unsigned short num, short i) {
    // i cannot larger than 16 (2 bytes)
    unsigned short mask = 0xFFFF;
    mask = mask >> (16 - i);
//    coutShortToBits(mask);
    return num & mask;
}

unsigned short getHashmapKey(const string &key, short i) {
    return getRightMostIbits(convertStringToShort(key), i);
}

unsigned short getHashmapKeyForTest(const std::string &key, short i){
    unsigned short x = 0;
    if (key[3] == '1'){
        x = x+1;
    }
    if (key[2] == '1'){
        x = x+2;
    }
    if (key[1] == '1'){
        x = x+4;
    }
    if (key[0] == '1'){
        x = x+8;
    }
    return getRightMostIbits(x, i);
}
unsigned short getBlockNumNeedToSplit(unsigned short newBlockNum) {
    return newBlockNum ^ 1 << (31 - __builtin_clz(newBlockNum));
}

