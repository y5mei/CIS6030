//
// Created by Yao-Wen Mei on 2022/9/26.
//

#ifndef PROJECT_RAWDATAPARSE_H
#define PROJECT_RAWDATAPARSE_H
#include <vector>
#include <string>
#include "DatabaseFileIO.h"
#include "../BPlusTree_lib/BPlusTree.h"

using namespace std;


void readRawDataAndGenerateDataBaseFile(string fileName = "../A1_data.txt");

class BTreeNodeBlock{
public:
    vector<char> content; // the max block size of BTree is 512;

    // serialization (Node -> char[512])
    BTreeNodeBlock();

    // deserialization
};

string readFileFromDiskByBlock(string fileName, int blockNum, int blockSize);
void writefileToDiskByBlock(string fileName, int blockNum, int blockSize, string content);
void deleteFile(string fileName);

#endif //PROJECT_RAWDATAPARSE_H
