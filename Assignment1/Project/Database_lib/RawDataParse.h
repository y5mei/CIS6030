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

void readRawDataAndGenerateDataBaseFile(string fileName = "../A1_data.txt");

// search a key from a BTree, and return and print the result;
string search(string key, string databaseFileName ="bTree_file.txt", string btreeFileName="bTree_file.txt");
void insert(string key, string databaseFileName ="bTree_file.txt", string btreeFileName="bTree_file.txt");
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

void deseralizeNodeFromStr(string str, vector<Node<string>*>* vec, int idx);

// Generate a dict of short for each B+Tree nodes
// TODO: How to return a pointer of a map from this function?
template<class T>
map<Node<T>*, short> generateBTreeNodeDict(BPlusTree<T>* bPlusTree);

string readFileFromDiskByBlock(string fileName, int blockNum, int blockSize);
void writefileToDiskByBlock(string fileName, int blockNum, int blockSize, string content);
void deleteFile(string fileName);
void coutShortToBits(short num);
void coutStringToBits(string str);
#endif //PROJECT_RAWDATAPARSE_H
