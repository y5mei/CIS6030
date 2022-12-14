//
// Created by Yao-Wen Mei on 2022/9/16.
//
#ifndef PROJECT_DATABASEFILEIO_H
#define PROJECT_DATABASEFILEIO_H
#include <vector>
using namespace std;

double f1();

class BlockListNode {
public:
    static const short MAX_SIZE = 1024;
    short ACTUAL_SIZE;
    char block[MAX_SIZE];
    BlockListNode* next = nullptr;

    short getNumOfRecord(); // only use 1 byte to store the number of records in block
    void setNumOfRecord(short n);

    BlockListNode(short size = MAX_SIZE);
    // generate a BlockListNode instance from a string read from hard disk,
    // assume the first byte of the string is num of record
    // also need to find out the num of emptybytes (how about the second bytes?)
    BlockListNode(std::string blockContent);

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

class Record{
public:
    std::string field1;
    std::string field2;
    std::string field3;
    std::string content;

    Record(std::string str);
    Record();
    friend ostream &operator<<(ostream &os, Record const &record) {
        string s = "";
        s.append("Field 1: ");
        s.append(record.field1+'\n');
        s.append("Field 2: ");
        s.append(record.field2+'\n');
        s.append("Field 3: ");
        s.append(record.field3+'\n');
        return os << s;
    }
private:
    short idxField2End;
    // returns the end idx of the second field (the space),
    // the start idx of the third field will be idx + 1
    // Example: a b c def, this method will return 4
    int endOfField2(std::string str);
};

BlockListNode* readFileFromHardDisk(std::string filename, int blockSize = 1024);

BlockListNode* readBlockWithLSeek(std::string filename, int blockNum, int blockSize = 1024);

int getNumOfBlocksFromHardDiskFile(std::string filename, int blockSize = 1024);

// read a file and get vector of sorted records
vector<Record> readRawTxtFile(string fileName = "../A1_data.txt");


//class Block{
//    short prev =
//};
#endif //PROJECT_DATABASEFILEIO_H
