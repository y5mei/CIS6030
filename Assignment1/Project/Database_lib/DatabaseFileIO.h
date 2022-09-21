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

class Record{
public:
    std::string field1;
    std::string field2;
    std::string field3;
    short idxField2End;

    // returns the end idx of the second field (the space),
    // the start idx of the third field will be idx + 1
    // Example: a b c def, this method will return 4
    int endOfField2(std::string str);
    Record(std::string str);
    Record();
};

#endif //PROJECT_DATABASEFILEIO_H
