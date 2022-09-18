//
// Created by Yao-Wen Mei on 2022/9/16.
//

#ifndef PROJECT_DATABASEFILEIO_H
#define PROJECT_DATABASEFILEIO_H


class DatabaseFileIO {

};

double f1();

class BlockListNode {
public:
    static const short MAX_SIZE = 1024;
    char block[MAX_SIZE];
    BlockListNode *next;

    short getNumOfRecord();

    BlockListNode();

    short getJumpOfRecord(short n);

    std::tuple<char, char> convertJumpToChars(short j);

    void setJumpOfRecord(short n, short j);

    void insertRecordStringToBlock(std::string str);

    void setNumOfRecord(short n);
};

#endif //PROJECT_DATABASEFILEIO_H
