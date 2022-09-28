//
// Created by Yao-Wen Mei on 2022/9/26.
//

#ifndef PROJECT_RAWDATAPARSE_H
#define PROJECT_RAWDATAPARSE_H
#include <vector>
#include <string>
#include <map>
#include <deque>
#include <bitset>
#include "DatabaseFileIO.h"
#include "../BPlusTree_lib/BPlusTree.h"

using namespace std;


struct CharShort {
    char high;
    char low;
    short num;

    CharShort(short j) {
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

struct StingShort {
    short block;
    short record;
    string str;

    StingShort(string s) {
        str = s;
        // block
        CharShort cs1 = CharShort(s[0], s[1]);
        block = cs1.num;
        // record
        CharShort cs2 = CharShort(s[2], s[3]);
        record = cs2.num;
    };

    StingShort(short blockNum, short recordNum) {
        block = blockNum;
        record = recordNum;
        CharShort b = CharShort(blockNum);
        CharShort r = CharShort(recordNum);
        str = {b.high, b.low, r.high, r.low};
    }
};

void readRawDataAndGenerateDataBaseFile(string fileName = "../A1_data.txt");

class BTreeNodeBlock{
public:
    vector<char> content; // the max block size of BTree is 512;

    // serialization (Node -> char[512])
    BTreeNodeBlock();

    // deserialization
};

// save a BTree into a disk file via level order transversal
template<class T>
void saveBTreeNodesOnDisk(BPlusTree<T>* bPlusTree, string filename);

template<class T>
string serializationOfNode(Node<T>* node, map<Node<T>*, short>* map);

// Generate a dict of short for each B+Tree nodes
// TODO: How to return a pointer of a map from this function?
template<class T>
map<Node<T>*, short> generateBTreeNodeDict(BPlusTree<T>* bPlusTree);

string readFileFromDiskByBlock(string fileName, int blockNum, int blockSize);
void writefileToDiskByBlock(string fileName, int blockNum, int blockSize, string content);
void deleteFile(string fileName);

#endif //PROJECT_RAWDATAPARSE_H
