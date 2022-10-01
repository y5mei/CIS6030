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
//    int blockNum = 1;
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
            FileToWrite.write(carray, 512);
//            writefileToDiskByBlock(filename, blockNum++, 512, content);
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
string serializationOfNode(Node<T> *node, map<Node<T> *, short> *map) {
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
    for (string value: node->values) {
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
    for (int n = 0; n < 512; n++) {
        s = s + (content[n]);
    }
    return s;
}

void deseralizeNodeFromStr(string str, vector<Node<string> *> *vec, int nodeNum) {
    Node<string> *node = vec->at(nodeNum);
    short leaf = str[0];
    node->isLeaf = leaf;
//    cout << "IsLeaf?: " << node->isLeaf << endl;

    short m = CharShort(str[1], str[2]).num;
    node->MAX_SIZE = m;
//    cout<<"Max_Size is: "<<node->MAX_SIZE<<endl;

    short next = CharShort(str[3], str[4]).num;
    node->next = vec->at(next);
//
    short parent = CharShort(str[5], str[6]).num;
    node->parent = vec->at(parent);
//    cout<<"parent is  "<<parent<<endl;

    short numOfKeys = CharShort(str[7], str[8]).num;
//    cout<<"num of keys are  "<<numOfKeys<<endl;

    int idx = 9;
    for (int i = 0; i < numOfKeys; ++i) {
        string key = "";
        for (int j = 0; j < 9; ++j) {
            key = key + str[idx];
            idx++;
        }
//        cout<<"Insert key : "<<key<<endl;
        node->keys.push_back(key);
    }

    short numOfChildren = CharShort(str[idx++], str[idx++]).num;
//    cout<<"num of children are  "<<numOfKeys<<endl;
    for (int i = 0; i < numOfChildren; ++i) {
        short child = CharShort(str[idx++], str[idx++]).num;
//        cout<< "child is: "<<child<<endl;
        node->children.push_back(vec->at(child));
    }

    short numOfvalues = CharShort(str[idx++], str[idx++]).num;

//    cout << "num of values are  " << numOfvalues << endl;
    for (int i = 0; i < numOfvalues; ++i) {
        string valStr = {str[idx++], str[idx++], str[idx++], str[idx++]};
        node->values.push_back(valStr);
    }
}

// assignment q1 part 1
void readRawDataAndGenerateDataBaseFile(string fileName) {
    vector<Record> records = readRawTxtFile(fileName);

    // let's load all the records into the block linked list:
    BlockListNode *b = new BlockListNode();
    BlockListNode *dummyHead = b;

    for (auto r: records) {
        b = b->insertRecordStringToBlock(r.content);
    }

    cout << "The first record is: " << dummyHead->getRecordAsString(1) << endl; //aaagbmhha
    cout << "Block 1 has totally # of records: " << dummyHead->getNumOfRecord() << endl;
    cout << "The second record in block 2 is: " << dummyHead->next->getRecordAsString(2) << endl; //aaieqtkeh
    cout << "Block 2 has totally # of records: " << dummyHead->next->getNumOfRecord() << endl;
    cout << "The 4th record in block 4 is: " << dummyHead->next->next->next->getRecordAsString(4) << endl; //aaujxfrwk
    cout << "Block 4has totally # of records: " << dummyHead->next->next->next->getNumOfRecord() << endl;
    cout << "The last record is: " << b->getRecordAsString(b->getNumOfRecord()) << endl; //zzzjzagrk
    cout << "The record before the last last record is: " << b->getRecordAsString(b->getNumOfRecord() - 1)
         << endl; //zzzjzagrk
    cout << records.size() << endl; //46883
    //1) Build a database file and save to disk; block size 1024;
    BlockListNode::saveToDisk(dummyHead, "database_file.txt");

    cout << "==== The input txt file is saved on disk as a database file: database_file.txt ======" << endl;
    cout << "==== Please wait while this program is generating the B+Tree file ...... ======" << endl;

    //2.1) Build a B+Tree in RAM,
    b = dummyHead;
    BPlusTree<string> bTree = BPlusTree<string>(8);
    vector<string> block_record; // the ptr values saved on BTree leaf nodes
    vector<string> field_1s; // the keys saved on BTree leaf nodes;

    int block_cnt = 0;
    while (b != nullptr) {
        block_cnt++;
        for (int i = 0; i < b->getNumOfRecord(); ++i) {
            Record r = Record(b->getRecordAsString(i + 1));
            //1 based BlockNum RecordNum seperated by space,
            // convert two short into a byte
            short blockNum = block_cnt;
            short recordNum = i + 1;
            string val = StingShort(block_cnt, recordNum).str;
            string key = r.field1;
            field_1s.push_back(key);
            block_record.push_back(val);
            bTree.insert(key, val);
        }
        b = b->next;
    }
//    cout<<nn->keys[0]<<endl;

    // save the B+Tree into Disk
    saveBTreeNodesOnDisk(&bTree, "bTree_file.txt");
    cout << "==== The B+Tree file is generated on the hard disk with name:  bTree_file.txt ======" << endl;
    cout << "=== Program 1 Part 1 has finished ===========" << endl;


    // Let's do a validation for keys in BTree and Records in Blocks
    int cnter = 0;
    auto ptr = bTree.root;
    while (!ptr->isLeaf) {
        ptr = ptr->children.front();
    }
    while (ptr != nullptr) {
        int curr_node_size = ptr->keys.size();
        for (int i = 0; i < curr_node_size; ++i) {
            // validate keys
            if (field_1s.at(cnter) != ptr->keys.at(i)) {
                string e = "";
                e += "leaf node value does not match block record sequence! \n At Index: ";
                e += to_string(cnter);
                e += " Field1 in record is ";
                e += field_1s.at(cnter);
                e += ". Field1 in BTree is ";
                e += ptr->keys.at(i);
                throw invalid_argument(e);
            }
//            ptr->values[i] = block_record[cnter];
            cnter++;
        }
        ptr = ptr->next;
    }
//    cout<<"BTree leaf node Validation Passed"<<endl;
}

void writefileToDiskByBlock(string fileName, int blockNum, int blockSize, string content) {
    fstream fin;
    fin.open(fileName);
    // if file does not exist, create a new one; if it is already exist, append new blocks into it;
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
    long shift;
    if (blockNum == 1) {
        shift = 0L;
    } else {
        shift = (blockNum - 1) * blockSize;
    }
    fin.seekg(shift, ios::beg);

    char content[blockSize];
    if (fin.good()) {
        fin.read((char *) &content, sizeof(content));
        bool isLeaf = content[0];
    }
    fin.close();

    int i;
    string str = "";
    for (i = 0; i < blockSize; i++) {
        str = str + content[i];
    }

    // Todo: why cannot use constructor to return a str?
//    string str(content);
//    while (str.size() > blockSize) {
//        str.pop_back(); // remove the ending \x1
//    }
    return str;
}

void deleteFile(string fileName) {
    const char *fname = fileName.c_str();
    if (remove(fname) != 0) {
        perror("Error deleting the file");
    }
}

// assignment q1 part 2-1
string search(string key, string databaseFileName, string btreeFileName) {
    if (key.size() != 9) {
        string error;
        error.append("the search key must has length of 9, but you are given key with length ");
        error.append(to_string(key.size()));
        throw invalid_argument(error);
    }

    cout<<"======================================="<<endl;
    cout<<" Search For Key = "<<key<<endl;
    cout<<"======================================="<<endl;
    int diskReadCnt = 1;

    HardDiskNode *h = new HardDiskNode();
    string str = readFileFromDiskByBlock(btreeFileName, 1, 512);
    h->deseralizeHardDiskNodeFromStr(str);

    while (!h->isLeaf) {
        short blockNum = h->searchNodeAtNonLeafNode(key);
        str = readFileFromDiskByBlock(btreeFileName, blockNum, 512);
        diskReadCnt++;
        h->deseralizeHardDiskNodeFromStr(str);
    }
    str = h->searchValueOnLeafNode(key);
    if (str == "-1") {
        cout << ">> After " << diskReadCnt << " times disk read, we found that: " << endl;
        cout << ">> The input key: " << key << " does not exist in the BTree" << endl;
        return "-1";
    } else {
        StingShort value = StingShort(str);
//        cout << value.block << endl;
//        cout << value.record << endl;
        string blockContent = readFileFromDiskByBlock("database_file.txt", value.block, 1024);
        BlockListNode bl = BlockListNode(blockContent);
        Record r = Record(bl.getRecordAsString(value.record));
        int numRecords = bl.getNumOfRecord();
        cout << ">> Reading a data block to RAM with "<< bl.getEmptyBytes() <<"/1024 empty Bytes, and "<<to_string(numRecords)<<" keys: ";
        for (int i = 1; i <= numRecords; ++i) {
            string s = bl.getRecordAsString(i);
            Record rr = Record(s);
            cout << rr.field1 << " ";
        }
        cout << "." << endl;
        cout << ">> After " << diskReadCnt + 1 << " times disk read (" << diskReadCnt
             << " times TreeNode read + 1 Data Block read), we found the key: " << key <<
             " at Block_Num " << value.block << ", and Record_Num " << value.record << ": " << endl;
        cout<<endl;
        cout << r << endl;
        return r.field1;
    }

}

// assignment q1 part 2-2
void insert(string record_str, string databaseFileName, string btreeFileName) {
    // search a key
    // 1. validate if the key is length of 9
    // 2. read the B+Tree in RAM, build the tree, and search for the block num and value num
    // 3. New a new BTree instance, set up the size m = 8, and repalce the root to the 1st element in the vector (0 is nullptr)
    // 4. Read the enitre datablocks into the RAM, and now report where to insert the new key;

    //====================== Step-1 ===========================================
    Record inputRecord = Record(record_str);
    string key = inputRecord.field1;

    //====================== Step-2 ===========================================
    // build a vector of treenode
    Node<string> curr = Node<string>();
    int size = getNumOfBlocksFromHardDiskFile(btreeFileName, 512);
    //Construct the Entire B+Tree
    vector<Node<string> *> BTreeVector;
    BTreeVector.push_back(nullptr); // 0 index is nullptr;
    while (size > 0) {
        BTreeVector.push_back(new Node<string>());
        size--;
    }
    for (int i = 1; i < BTreeVector.size(); ++i) {
        auto *node = BTreeVector[i];
        string str = readFileFromDiskByBlock(btreeFileName, i, 512);
        deseralizeNodeFromStr(str, &BTreeVector, i);
    }
    // New a BTree Instance, repalce the root node
    auto *root = BTreeVector.at(1);
    BPlusTree<string> bPlusTree = BPlusTree<string>(8);
    bPlusTree.root = root;

    //====================== Step-3 ===========================================
    //====================== Find out the node where to insert the new record =
    // Search the key and cout the result;
    string successorValue = bPlusTree.searchInOrderSuccessor(key);

    // if the key is smaller than the current smallest key in the Tree, we need to insert this value to the front of the block;
    // which is block 1, record does not really matter, as we will rewrite all the records back!
    StingShort value = StingShort({'\0', '\0', '\0', '\0'});
    if (successorValue == key) {
        CharShort cs = CharShort(1);
        value = StingShort(cs.num, cs.num);
    } else {
        value = StingShort(successorValue);
    }

//    string blockContent = readFileFromDiskByBlock("database_file.txt", value.block, 1024);
//    BlockListNode bl = BlockListNode(blockContent);
//    Record r = Record(bl.getRecordAsString(value.record));

    //====================== Step-4 ===========================================
    //New a new Block Linked List, and read all the records from the block into a vector, insert the new record into it
    // sort, and write everything back to this block node again
    BlockListNode *b = new BlockListNode(readFileFromDiskByBlock("database_file.txt", 1, 1024));
    BlockListNode *insertNode = b;
    BlockListNode *dummyHead = b;
    int blockSize = getNumOfBlocksFromHardDiskFile("database_file.txt", 1024);
    for (int j = 1; j < blockSize; ++j) {
        string currBlock = readFileFromDiskByBlock("database_file.txt", j + 1, 1024);
        b->next = new BlockListNode(currBlock);
        b = b->next;
        if (value.block == (j + 1)) {
            insertNode = b; // save the ptr to the block that we want to modify!!!
        }
    }

    int numRecords = insertNode->getNumOfRecord();
    // get all the records from this block in a vector and sort it, and put them back;
    vector<Record> records;
    records.push_back(inputRecord);
    for (int m = 1; m <= numRecords; ++m) {
        records.push_back(Record(insertNode->getRecordAsString(m)));
    }
    sort(records.begin(), records.end(), [](Record a, Record b) { return (a.field1 < b.field1); });
//    cout << "======================================" << endl;
//    cout << " Empty Space in the current Node is " << insertNode->getEmptyBytes() << endl;
//    cout << "======================================" << endl;
    insertNode->setNumOfRecord(0); // clear the whole block

    // insert all the records back, calcualte the new Block Num, Record Num for the inserted record;
    int newBlockNum = value.block;
    int newRecordNum = 1;
    auto oldNode = insertNode;

    for (Record rr: records) {
        insertNode = insertNode->insertRecordStringToBlock(rr.content);
    }
    // find out where is the new record is inserted into
    while (oldNode!= nullptr){
        for(int i = 1; i<=oldNode->getNumOfRecord(); ++i){
            if (Record(oldNode->getRecordAsString(i)).field1==inputRecord.field1){
                newRecordNum = i;
                oldNode = nullptr;
                break;
            }
        }
        if (oldNode!= nullptr){
            oldNode = oldNode->next;
            newBlockNum +=1;
        }
    }
//    cout << "Going to insert the new record: " << inputRecord.field1 << " at Block, Record: " << newBlockNum << "  "
//         << newRecordNum << endl;
//
//
//    // save the new inserted record to BPlust Tree in RAM;
    bPlusTree.insert(inputRecord.field1, StingShort(newBlockNum, newRecordNum).str);

    //1) Build a database file and save to disk; block size 1024;
    BlockListNode::saveToDisk(dummyHead, "database_file.txt");


    // move all the way to the 1st leaf node, and update all the leaf node values;
    auto ptr = bPlusTree.root;
    while (!ptr->isLeaf) {
        ptr = ptr->children.front();
    }
    vector<string> block_record;
    vector<string> field_1s;
    int block_cnt = 0;
    b = dummyHead;
    while (b != nullptr) {
        block_cnt++;
        for (int i = 0; i < b->getNumOfRecord(); ++i) {
            Record r = Record(b->getRecordAsString(i + 1));
            //1 based BlockNum RecordNum seperated by space,
            // convert two short into a byte
            short blockNum = block_cnt;
            short recordNum = i + 1;
            string record_val = StingShort(block_cnt, recordNum).str;
            string record_key = r.field1;
            if(field_1s.size()>0 && field_1s.back()>=record_key){
                string e = "";
                e += "Block is invalid";
                e += field_1s.back();
                e += " "+record_key;
                throw invalid_argument(e);
            }
            field_1s.push_back(record_key);

            block_record.push_back(record_val);
        }
        b = b->next;
    }
    int cnter = 0;
//    vector<string> bTreeKeys;
    while (ptr != nullptr) {
        int curr_node_size = ptr->keys.size();
        for (int i = 0; i < curr_node_size; ++i) {
            // validate keys
//            bTreeKeys.push_back(ptr->keys[i]);
            if (field_1s.at(cnter) != ptr->keys.at(i)) {
                string e = "";
                e += "leaf node value does not match block record sequence! \n At Index: ";
                e += to_string(cnter);
                e += " Field1 in Block is ";
                e += field_1s.at(cnter);
                e += ". Field1 in BTree is ";
                e += ptr->keys.at(i);
                throw invalid_argument(e);
            }
            // update all the values and save back to the disk
            ptr->values[i] = block_record[cnter];
            cnter++;
        }
        ptr = ptr->next;
    }

//    cout<<bTreeKeys.size()<<" "<<field_1s.size()<<endl;
    saveBTreeNodesOnDisk(&bPlusTree, "bTree_file.txt");


//    //2.1) Build a B+Tree in RAM,
//    b = dummyHead;
//    BPlusTree<string> bTree = BPlusTree<string>(8);
//
//    int block_cnt = 0;
//    while (b != nullptr) {
//        block_cnt++;
//        for (int i = 0; i < b->getNumOfRecord(); ++i) {
//            Record r = Record(b->getRecordAsString(i + 1));
//            //1 based BlockNum RecordNum seperated by space,
//            // convert two short into a byte
//            short blockNum = block_cnt;
//            short recordNum = i + 1;
//            string val = StingShort(block_cnt, recordNum).str;
//            string key = r.field1;
//            bTree.insert(key, val);
//        }
//        b = b->next;
//    }
//    saveBTreeNodesOnDisk(&bTree, "bTree_file.txt");
}


// printout a short as bits;
void coutShortToBits(short num) {
    bitset<16> x(num);
    cout << "The bits for " << num << " is: " << endl;
    cout << x << endl;
}

// printout a string of chars as bits;
void coutStringToBits(string str) {
    cout << "Convert str to bits:" << str << endl;
    for (char c: str) {
        bitset<8> x(c);
        cout << x << " ";
    }
    cout << "." << endl;
}

