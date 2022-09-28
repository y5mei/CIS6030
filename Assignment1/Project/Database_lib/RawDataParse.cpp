//
// Created by Yao-Wen Mei on 2022/9/26.
//

#include "RawDataParse.h"
#include "DatabaseFileIO.h"
#include "../BPlusTree_lib/BPlusTree.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <bitset>
#include <cstddef>

using namespace std;

template<class T>
map<Node<T> *, short> generateBTreeNodeDict(BPlusTree<T> *bPlusTree) {
    // generate a dict key is pointer of node, value is a short
    map<Node<T> *, short> bPlusTreeMap;
    bPlusTreeMap[nullptr] = 0; // nodes num are 1 based, so we can use 0 for nullptr;
    deque<Node<T> *> q;
    q.push_back(bPlusTree->root);
    short cnt = 1;

    while (!q.empty()) {
        int size = q.size();
        for (int i = 0; i < size; ++i) {
            Node<T> *currNode = q.front();
            q.pop_front();
            bPlusTreeMap[currNode] = cnt++; // save each of the tree node and their index;
            for (auto *child: currNode->children) {
                q.push_back(child);
            }
        }
    }
    return bPlusTreeMap;
}

template<class T>
void saveBTreeNodesOnDisk(BPlusTree<T> *bPlusTree, string filename) {
    ofstream FileToWrite(filename, ofstream::trunc); // erase the content before write
    map<Node<string> *, short> bTreeMap = generateBTreeNodeDict(bPlusTree);
    deque<Node<T> *> q;
    q.push_back(bPlusTree->root);
    while (!q.empty()) {
        int size = q.size();
        for (int i = 0; i < size; ++i) {
            Node<T> *currNode = q.front();
            q.pop_front();
            // serializae currNode as a char[512]
            // save this block to a file

            for (auto *child: currNode->children) {
                q.push_back(child);
            }
        }
    }
    FileToWrite.close();
}

void pushShortToCharVector(short j, vector<char> *v) {
    v->push_back(CharShort(j).high);
    v->push_back(CharShort(j).low);
}

template<class T>
string serializationOfNode(Node<T> *node, map<Node<T> *, short> map) {
    char content[512];
    vector<char> v;
    v.push_back(node->isLeaf);
    pushShortToCharVector(node->MAX_SIZE, &v);
    pushShortToCharVector(map.at(node->next), &v);
    pushShortToCharVector(map.at(node->parent), &v);
    // put keys
    short i = node->keys.size();
    pushShortToCharVector(i, &v);
    for (string k: node->keys) {
        for (char c: k) {
            v.push_back(c);
        }
    }
    // put childrens
    short j = node->children.size();
    pushShortToCharVector(j, &v);
    for (auto *child: node->children) {
        pushShortToCharVector(map.at(child), &v);
    }
    // put values
    short k = node->values.size();
    pushShortToCharVector(k, &v);
    // todo: find out how to insert values
}


// TODO: Also need to save the B+Tree in RAM;
void readRawDataAndGenerateDataBaseFile(string fileName) {
    vector<Record> records = readRawTxtFile(fileName);

    // let's load all the records into the block linked list:
    BlockListNode *b = new BlockListNode();
    BlockListNode *dummyHead = b;

    for (auto r: records) {
        b = b->insertRecordStringToBlock(r.content);
    }

    cout << "The first record is: " << dummyHead->getRecordAsString(1) << endl;
    cout << "Block 1 has totally # of records: " << dummyHead->getNumOfRecord() << endl;
    cout << "The second record in block 2 is: " << dummyHead->next->getRecordAsString(2) << endl;
    cout << "Block 2 has totally # of records: " << dummyHead->next->getNumOfRecord() << endl;
    cout << "The 4th record in block 4 is: " << dummyHead->next->next->next->getRecordAsString(4) << endl;
    cout << "Block 4has totally # of records: " << dummyHead->next->next->next->getNumOfRecord() << endl;
    cout << "The last record is: " << b->getRecordAsString(b->getNumOfRecord()) << endl;

    //TODO: construct a B+Tree from the database file?
    cout << records.size() << endl;
    //1) Build a database file and save to disk; block size 1024;
    BlockListNode::saveToDisk(dummyHead, "database_file.txt");

    //2.1) Build a B+Tree in RAM,
    b = dummyHead;
    BPlusTree<string> bTree = BPlusTree<string>(8);

    int block_cnt = 0;
    while (b != nullptr) {
        block_cnt++;
        for (int i = 0; i < b->getNumOfRecord(); ++i) {
            Record r = Record(b->getRecordAsString(i + 1));
            //1 based BlockNum RecordNum seperated by space,
            // convert two short into a byte
            string value = to_string(block_cnt) + " " + to_string(i + 1);
            string key = r.field1;
            bTree.insert(key, value);
        }
        b = b->next;
    }

    string val = bTree.search("aaujxfrwk");
    cout << val << endl;

    auto r = bTree.root;
    auto tail = r->searchNode("zzzjzagrk");

    map<Node<string> *, short> mymap4 = generateBTreeNodeDict(&bTree);
    cout << mymap4.at(r) << endl;
    cout << mymap4.at(tail) << endl;
    cout << mymap4.at(nullptr) << endl;
    saveBTreeNodesOnDisk(&bTree, "bTreeFile.dat");
    cout << "==========" << endl;



    //TODO: Write the B+Tree in HardDisk
    // Read a file from HardDisk and put it back as B+Tree;
    // In order to do this, I need to first creat a vector of Nodes, and then initialize them and and link them with pointer;
}


void writefileToDiskByBlock(string fileName, int blockNum, int blockSize, string content) {
    fstream fin;
    fin.open(fileName);
    if (!fin.is_open()) {
        fin.open(fileName, fstream::in | fstream::out | fstream::app);
    }

    long shift = (blockNum - 1) * blockSize;
    if (blockNum == 1) {
        shift = 0L;
    }
    fin.seekp(shift);

    if (fin.good()) {
        const char *carray = content.c_str();
        fin.write(carray, blockSize);
    }
    fin.close();

}

string readFileFromDiskByBlock(string filename, int blockNum, int blockSize) {
    ifstream fin;
    fin.open(filename);

    long shift = (blockNum - 1) * blockSize;
    if (blockNum == 1) {
        shift = 0L;
    }
    fin.seekg(shift, ios::beg);

    char buffer[blockSize];
    if (fin.good()) {
        fin.read((char *) &buffer, sizeof(buffer));
    }
    fin.close();

    string str(buffer);
    while (str.size() > blockSize) {
        str.pop_back(); // remove the ending \x1
    }

    return str;
}

void deleteFile(string fileName) {
    const char *fname = fileName.c_str();
    if (remove(fname) != 0) {
        perror("Error deleting the file");
    }
}



