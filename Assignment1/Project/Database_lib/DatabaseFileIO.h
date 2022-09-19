//
// Created by Yao-Wen Mei on 2022/9/16.
//

#ifndef PROJECT_DATABASEFILEIO_H
#define PROJECT_DATABASEFILEIO_H

double f1();

class BlockListNode {
public:
    static const short MAX_SIZE = 1024;
    short ACTUAL_SIZE;
    char block[MAX_SIZE];
    BlockListNode* next = nullptr;

    short getNumOfRecord();

    BlockListNode(short size = MAX_SIZE);

    short getJumpOfRecord(short n);

    std::tuple<char, char> convertJumpToChars(short j);

    void setJumpOfRecord(short n, short j);

    BlockListNode * insertRecordStringToBlock(std::string str);

    void setNumOfRecord(short n);

    short sizeOfEmptyBytes();

    BlockListNode *generateNextNode(std::string str);

    std::string getRecordAsString(short n);
};

#endif //PROJECT_DATABASEFILEIO_H
