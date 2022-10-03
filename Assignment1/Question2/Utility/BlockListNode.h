//
// Created by Yao-Wen Mei on 2022/10/3.
//

#ifndef QUESTION2_BLOCKLISTNODE_H
#define QUESTION2_BLOCKLISTNODE_H
#include <vector>
using namespace std;


class BlockListNode {
public:
    static const short MAX_SIZE = 1024;
    short ACTUAL_SIZE;
    char block[MAX_SIZE];
    BlockListNode* next = nullptr;

    short getNumOfRecord(); // only use 1 byte to store the number of records in block
    void setNumOfRecord(short n);

    explicit BlockListNode(short size = MAX_SIZE);
    // generate a BlockListNode instance from a string read from hard disk,
    // assume the first byte of the string is num of record
    // also need to find out the num of emptybytes (how about the second bytes?)
    explicit BlockListNode(std::string blockContent);

    void setBlock(string s);

    short getJumpOfRecord(short n);
    void setJumpOfRecord(short n, short j);

    std::tuple<char, char> convertJumpToChars(short j);
    BlockListNode * insertRecordStringToBlock(std::string str);

    short getEmptyBytes();

    BlockListNode *generateNextNode(std::string str);

    std::string getRecordAsString(short n);

    // save all the blocks from head to tail in a database file;
    static void saveToDisk(BlockListNode* head, std::string filename = "databasefile");
};


#endif //QUESTION2_BLOCKLISTNODE_H
