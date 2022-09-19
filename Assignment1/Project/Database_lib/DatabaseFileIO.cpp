//
// Created by Yao-Wen Mei on 2022/9/16.
//
#include <iostream>
#include "DatabaseFileIO.h"
#include <bitset>
#include <tuple>
#include <stdexcept>

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

BlockListNode::BlockListNode(short size) {
    block[0] = '\0'; // initially, there are 0 records in the block
    ACTUAL_SIZE = size;
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
//    bitset<16> ss1(result);
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
void BlockListNode::setJumpOfRecord(short n, short j) {
    tuple<char, char> charTuple = convertJumpToChars(j);
    block[2 * n - 1] = get<0>(charTuple);
    block[2 * n] = get<1>(charTuple);
}

// insert a string into the block
BlockListNode* BlockListNode::insertRecordStringToBlock(string str) {
    // TODO: Refuse to insert into the current block if the empty space left is not enough to hold the new str
    // need to create a new block, and link to it.

    // We do not handle the case where the input record is longer than the size of a block;
    if (ACTUAL_SIZE < (str.size() + 2)) {
        throw invalid_argument("not enough empty space to hold new str, need to create a linked list");
    }
    // if the size of the input record is larger than the available empty bytes, we need to create a new node, and link the new node as next of the prev node
    if (sizeOfEmptyBytes() < (str.size() + 2)){
        BlockListNode* nextNode = generateNextNode(str);
        return nextNode;
    }

    short numRecord = getNumOfRecord();
    short prevRecordStartIdx;
    if (numRecord == 0) {
        prevRecordStartIdx = ACTUAL_SIZE;
    } else {
        prevRecordStartIdx = getJumpOfRecord(numRecord);
    }
    short startIdx = prevRecordStartIdx - str.size();
    short i = startIdx;
    for (char c: str) {
        block[i++] = c;
    }
    // increase cnt
    setNumOfRecord(getNumOfRecord() + 1);
    // set jump
    setJumpOfRecord(getNumOfRecord(), startIdx);
    return this;
}

BlockListNode *BlockListNode::generateNextNode(string str) {
    short newSize = ACTUAL_SIZE;
    BlockListNode *ptr = new BlockListNode(newSize); // block size are not 1K for unit test purpose
    // The following code does not work, bad_ptr always point to the same address;
    //    BlockListNode n = BlockListNode(newSize);
    //    BlockListNode* bad_ptr = &n;
    BlockListNode* prevTail = next;
    this->next = ptr;
    ptr->next = prevTail;
    ptr->insertRecordStringToBlock(str);
//    cout << "current node is: " << this << endl;
//    cout << "the next node is: " << ptr << endl;
    return ptr;
}

//calculate how many empty bytes are available in the current block.
short BlockListNode::sizeOfEmptyBytes() {
    short numRecord = getNumOfRecord();
    if (numRecord == 0) {
        return ACTUAL_SIZE - 1;
    } else {
        short prevRecordStartIdx = getJumpOfRecord(numRecord);
        short sizeOfHeader = 1 + 2 * numRecord;
        short sizeOfContent = ACTUAL_SIZE - prevRecordStartIdx;
        short emptyBytes = ACTUAL_SIZE - sizeOfHeader - sizeOfContent;
        return emptyBytes;
    }
}

string BlockListNode::getRecordAsString(short n) {
    short numRecord = getNumOfRecord();
    short start;
    short end = ACTUAL_SIZE - 1;

    if (n > numRecord) {
        throw invalid_argument("Does not have this record yet!");
    }
    if (numRecord == 0) {
        return "";
    } else if (n == 1) {
        start = getJumpOfRecord(n);
    } else {
        start = getJumpOfRecord(n);
        end = getJumpOfRecord(n - 1) - 1;
    }
    char result[end - start + 2];
    for (short i = start; i <= end; i++) {
        result[i - start] = block[i];
    }
    result[end - start + 1] = '\0';
    return string(result);
}