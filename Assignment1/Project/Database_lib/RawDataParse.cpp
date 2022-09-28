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

//    cout<<"Dic of nodes generated"<<endl;
    deque<Node<T> *> q;
    int cnt = 0;
    q.push_back(bPlusTree->root);
    while (!q.empty()) {
        int size = q.size();
        for (int i = 0; i < size; ++i) {
            Node<T> *currNode = q.front();
            q.pop_front();
            // serializae currNode as a char[512]
            // save this block to a file
            string content = serializationOfNode(currNode, &bTreeMap);
            const char *carray = content.c_str();
//            cout<<"Saving num of node to disk: "<<++cnt<<endl;
            FileToWrite.write(carray, content.size());
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
string serializationOfNode(Node<T> *node, map<Node<T> *, short>* map) {
    char content[512];
    vector<char> v;
    // put isLeaft in;
    v.push_back(node->isLeaf);
    // put m, next, parent pointers in
    pushShortToCharVector(node->MAX_SIZE, &v);
    pushShortToCharVector(map->at(node->next), &v);
    pushShortToCharVector(map->at(node->parent), &v);
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
        pushShortToCharVector(map->at(child), &v);
    }
    // put values
    short k = node->values.size();
    pushShortToCharVector(k, &v);
    for(string value: node->values){
        StingShort ss = StingShort(value);
        pushShortToCharVector(ss.block, &v);
        pushShortToCharVector(ss.record, &v);
    }

    // save to block of char[512]
    int l = v.size();
    for (int m = 0; m < l; ++m) {
        content[m] = v[m];
    }
    // cast to string to make sure this is a deep copy???

    string s = "";
    for(int n = 0; n<512; n++){
        s = s+(content[n]);
    }
    return s;
}

template<class T>
void deseralizeNodeFromStr(string str, Node<T>* node){
    short leaf = str[0];
    node->isLeaf = leaf;
    cout<<"IsLeaf?: "<<node->isLeaf<<endl;

    short m = CharShort(str[1], str[2]).num;
    node->MAX_SIZE = m;
//    cout<<"Max_Size is: "<<node->MAX_SIZE<<endl;

    short next = CharShort(str[3], str[4]).num;
//    cout<<"next is  "<<next<<endl;
    //todo: setup next and parent via dict

    short parent = CharShort(str[5], str[6]).num;
    cout<<"parent is  "<<parent<<endl;

    short numOfKeys = CharShort(str[7], str[8]).num;
    cout<<"num of keys are  "<<numOfKeys<<endl;

    int idx = 9;
    for (int i = 0; i < numOfKeys; ++i) {
        string key = "";
        for (int j = 0; j < 9; ++j) {
            key  = key + str[idx];
            idx++;
        }
//        cout<<"Insert key : "<<key<<endl;
        node->keys.push_back(key);
    }

    short numOfChildren = CharShort(str[idx++], str[idx++]).num;
//    cout<<"num of children are  "<<numOfKeys<<endl;
    for (int i = 0; i < numOfChildren; ++i){
        short child = CharShort(str[idx++], str[idx++]).num;
//        cout<< "child is: "<<child<<endl;
        // Todo: need to link up the children
    }

    short numOfvalues = CharShort(str[idx++], str[idx++]).num;
    cout<<"num of values are  "<<numOfvalues<<endl;
    for (int i = 0; i < numOfvalues; ++i){
        string valStr = {str[idx++], str[idx++], str[idx++], str[idx++]};
        short blockValue = StingShort(valStr).block;
        short recordValue = StingShort(valStr).record;
        cout<< "BlockNum is: "<<blockValue<<endl;
        cout<<"RecordNum is"<<recordValue<<endl;
        node->values.push_back(valStr);
        // Todo: need to link up the children
    }

}

void readRawDataAndGenerateDataBaseFile(string fileName) {
    vector<Record> records = readRawTxtFile(fileName);

    // let's load all the records into the block linked list:
    BlockListNode *b = new BlockListNode();
    BlockListNode *dummyHead = b;

    for (auto r: records) {
        b = b->insertRecordStringToBlock(r.content);
    }

//    cout << "The first record is: " << dummyHead->getRecordAsString(1) << endl; //aaagbmhha
//    cout << "Block 1 has totally # of records: " << dummyHead->getNumOfRecord() << endl;
//    cout << "The second record in block 2 is: " << dummyHead->next->getRecordAsString(2) << endl; //aaieqtkeh
//    cout << "Block 2 has totally # of records: " << dummyHead->next->getNumOfRecord() << endl;
//    cout << "The 4th record in block 4 is: " << dummyHead->next->next->next->getRecordAsString(4) << endl; //aaujxfrwk
//    cout << "Block 4has totally # of records: " << dummyHead->next->next->next->getNumOfRecord() << endl;
//    cout << "The last record is: " << b->getRecordAsString(b->getNumOfRecord()) << endl; //zzzjzagrk
//    cout << records.size() << endl; //46883
    //1) Build a database file and save to disk; block size 1024;
    BlockListNode::saveToDisk(dummyHead, "database_file.txt");

    cout << "==== The input txt file is saved on disk as a database file: database_file.txt ======" << endl;
    cout << "==== Please wait while this program is generating the B+Tree file ...... ======" << endl;

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
            short blockNum = block_cnt;
            short recordNum = i+1;
            string val = StingShort(block_cnt, recordNum).str;
            string key = r.field1;
            bTree.insert(key, val);
        }
        b = b->next;
    }

    // save the B+Tree into Disk
    saveBTreeNodesOnDisk(&bTree, "bTree_file.txt");
    cout << "==== The B+Tree file is generated on the hard disk with name:  bTree_file.txt ======" << endl;

//    string val = bTree.search("aaujxfrwk");
//    cout << val << endl;
//
//    auto r = bTree.root;
//    auto tail = r->searchNode("zzzjzagrk");
//
//    map<Node<string> *, short> bTreeMap = generateBTreeNodeDict(&bTree);
//    cout << bTreeMap.at(r) << endl;
//    cout << bTreeMap.at(tail) << endl;
//    cout << bTreeMap.at(nullptr) << endl;
//    string test = serializationOfNode(tail->parent, bTreeMap);
//    cout << test << endl;

    // read back a node
//    Node<string>* readBack = new Node<string>();
//    deseralizeNodeFromStr(test, readBack);

    cout << "=== Program 1 Part 1 has finished ===========" << endl;

    //TODO:
    // Read a file from HardDisk and put it back as B+Tree;
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



