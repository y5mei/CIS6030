//
// Created by Yao-Wen Mei on 2022/9/16.
//
#include <iostream>
#include "DatabaseFileIO.h"
#include <bitset>
#include<tuple>

using namespace std;

double f1() {

    cout << " this is inside of another cpp file" << endl;
    return 10000000000;
}

/**
 * Definition for singly-linked list.
 * block has 1024 bytes, the 1st char represents how many records in the block, and can be read directly as short (only the lower 8 bits of short will be used anyways);
 * The next n chars are representing the end position of each records;
 */
//class BlockListNode {
//    char block[1024] = {'\0'};
//    BlockListNode *next;
//
//    short getNumOfRecord();
//
//public:
//    BlockListNode() : next(nullptr) {}
////
////    BlockListNode(int x) : val(x), next(nullptr) {}
////
////    BlockListNode(int x, BlockListNode *next) : val(x), next(next) {}
//};

BlockListNode::BlockListNode() {
    block[0] = '\0'; // initially, there are 0 records in the block
}

short BlockListNode::getNumOfRecord() {
    return block[0];
}

// set the num of records to n, n is short, but n is always smaller than 113, so we only use the lower 8 bits;
void BlockListNode::setNumOfRecord(short n) {
    block[0] = n;
}

// read two bytes (2 chars) and return a short that represents where is the end of the n's record.
short BlockListNode::getJumpOfRecord(short n) {
    unsigned char higher = block[2 * n - 1];
    unsigned char lower = block[2 * n];
    short result = higher << 8;
    result = result | lower;
    bitset<16> ss1(result);
    cout << ss1 << endl;
    return result;
}

// converts a jump number (1 to 1023) to two bytes (two chars)
tuple<char, char> BlockListNode::convertJumpToChars(short j) {
    char high;
    char low;
    low = j;
    j = j >> 8;
    high = j;
    return tuple<char, char>(high, low);
}

// set the n's jump number to be j, the nth jump number should be stored at index 2n-1 and 2n;
void BlockListNode::setJumpOfRecord(short n, short j){
    tuple<char, char> charTuple = convertJumpToChars(j);
    block[2*n-1] = get<0>(charTuple);
    block[2*n] = get<1>(charTuple);
}

// insert a string into the block
void BlockListNode::insertRecordStringToBlock(string str){
    // TODO: Refuse to insert into the current block if the empty space left is not enough to hold the new str
    // need to create a new block, and link to it.
    short numRecord = getNumOfRecord();
    short prevRecordStartIdx;
    if (numRecord==0){
        prevRecordStartIdx = MAX_SIZE + 1;
    }else{
        prevRecordStartIdx = getJumpOfRecord(numRecord);
    }
    short endIdx = prevRecordStartIdx - 1;
    short startIdx = prevRecordStartIdx - str.size();
    short i = startIdx;
    for (char c: str){
        block[i++] = c;
    }
    // increase cnt
    setNumOfRecord(getNumOfRecord()+1);
    // set jump
    setJumpOfRecord(getNumOfRecord(), startIdx);
}

//calculate how many empty bytes are available in the current block.
short BlockListNode::sizeOfEmptyBytes(){
    short numRecord = getNumOfRecord();
    if (numRecord == 0){
        return MAX_SIZE-1;
    }else{
        short prevRecordStartIdx = getJumpOfRecord(numRecord);
        short sizeOfHeader = 1 + 2*numRecord;
        short sizeOfContent = MAX_SIZE - prevRecordStartIdx + 1;
        short emptyBytes = MAX_SIZE - sizeOfHeader - sizeOfContent;
        return emptyBytes;
    }
}