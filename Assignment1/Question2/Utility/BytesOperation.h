//
// Created by Yao-Wen Mei on 2022/10/3.
//

#ifndef QUESTION2_BYTESOPERATION_H
#define QUESTION2_BYTESOPERATION_H

#include <cmath>
#include <bitset>

using namespace std;

// convert a char to two short
struct CharShort {
    char high;
    char low;
    unsigned short num;

    explicit CharShort(short j) {
        num = j;
        low = j;
        j = j >> 8;
        high = j;
    };

    CharShort(char h, char l) {
        high = h;
        low = l;
        num = h << 8;
        num = num | (unsigned char) l; // make sure the high 8 bits of l is 0 nor 1
    }
};

// covert 4 chars to 4 shorts
struct StingShort {
    unsigned short block;
    unsigned short record;
    string str;

    // the input str need to be 4 char in lengh
    explicit StingShort(string s) {
        str = s;
        // block
        CharShort cs1 = CharShort(s[0], s[1]);
        block = cs1.num;
        // record
        auto cs2 = CharShort(s[2], s[3]);
        record = cs2.num;
    };

    StingShort(short blockNum, short recordNum) {
        block = blockNum;
        record = recordNum;
        auto b = CharShort(blockNum);
        auto r = CharShort(recordNum);
        str = {b.high, b.low, r.high, r.low};
    }
};

void coutShortToBits(short num);
void coutStringToBits(const string& str);


// need a hash function convert xixmvtrrb to an unsigned short?
// I have 50000 records, each bin target to put 100 records, I will need 500 bins;
unsigned short getHashValue(const string& key);
unsigned short getRightMostIbits(unsigned short num, short i);
#endif //QUESTION2_BYTESOPERATION_H
