//
// Created by Yao-Wen Mei on 2022/10/1.
//


#include <iostream>
#include <fstream>
#include "DatabaseFileIO.h"
#include <bitset>
#include <tuple>
#include <deque>
#include <stdexcept>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <map>
#include <unistd.h> // lseek
#include "../BPlusTree_lib/BPlusTree.h"
#include "FinalPrintOut.h"

using namespace std;

double f2() {

    cout << " this is inside of another cpp file2" << endl;
    return 10000000000;
}
void saveVectorOfField1ToDisk(vector<std::string> *vec, std::string field_one_in_order)  {
    ofstream FileToWrite(field_one_in_order, ofstream::trunc); // erase the content before write
    for(string content: *vec){
        FileToWrite<<content<<endl;
    }
    FileToWrite.close();
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
        cout << "Insert key : " << key << endl;
        node->keys.push_back(key);
    }

    short numOfChildren = CharShort(str[idx], str[idx + 1]).num;
    idx = idx + 2;
//    cout<<"num of children are  "<<numOfKeys<<endl;
    for (int i = 0; i < numOfChildren; ++i) {
        short child = CharShort(str[idx], str[idx + 1]).num;
        idx = idx + 2;
//        cout<< "child is: "<<child<<endl;
        node->children.push_back(vec->at(child));
    }

    short numOfvalues = CharShort(str[idx], str[idx + 1]).num;
    idx = idx + 2;

//    cout << "num of values are  " << numOfvalues << endl;
    for (int i = 0; i < numOfvalues; ++i) {
        string valStr = {str[idx], str[idx + 1], str[idx + 2], str[idx + 3]};
        idx = idx + 4;
        node->values.push_back(valStr);
    }
}

void deseralizeNodeFromArray(char *p, vector<Node<string> *> *vec, int nodeNum) {
    Node<string> *node = vec->at(nodeNum);
    short leaf = *p;
    node->isLeaf = leaf;
//    cout << "IsLeaf?: " << node->isLeaf << endl;

    short m = CharShort(*(p + 1), *(p + 2)).num;
    node->MAX_SIZE = m;
//    cout<<"Max_Size is: "<<node->MAX_SIZE<<endl;

    short next = CharShort(*(p + 3), *(p + 4)).num;
    node->next = vec->at(next);

    short parent = CharShort(*(p + 5), *(p + 6)).num;
    node->parent = vec->at(parent);
//    cout<<"parent is  "<<parent<<endl;

    short numOfKeys = CharShort(*(p + 7), *(p + 8)).num;
//    cout<<"num of keys are  "<<numOfKeys<<endl;

    p = p + 9;
    for (int i = 0; i < numOfKeys; ++i) {
        string key = "";
        for (int j = 0; j < 9; ++j) {
            key = key + *p;
            p = p + 1;
        }
//        cout<<"Insert key : "<<key<<endl;
        node->keys.push_back(key);
    }

    short numOfChildren = CharShort(*p, *(p + 1)).num;
    p++;
    p++;
//    cout<<"num of children are  "<<numOfKeys<<endl;
    for (int i = 0; i < numOfChildren; ++i) {
        short child = CharShort(*p, *(p + 1)).num;
        p++;
        p++;
//        cout<< "child is: "<<child<<endl;
        node->children.push_back(vec->at(child));
    }

    short numOfvalues = CharShort(*p, *(p + 1)).num;
    p++;
    p++;

//    cout << "num of values are  " << numOfvalues << endl;
    for (int i = 0; i < numOfvalues; ++i) {
        string valStr = {*p, *(p + 1), *(p + 2), *(p + 3)};
        p = p + 4;
        node->values.push_back(valStr);
    }
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

// This is a very fast method to initialize all the btree nodes;
void readFileFromDiskByBlockReturnArrayWithoutFileClose(ifstream& fin, string filename, vector<Node<string> *> *vec, int blockNum, int blockSize) {
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
    }
    deseralizeNodeFromArray(content, vec, blockNum);
    return;
}

void readFileFromDiskByBlockReturnArray(string filename, vector<Node<string> *> *vec, int blockNum, int blockSize) {
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
    }
    fin.close();
    deseralizeNodeFromArray(content, vec, blockNum);
    return;
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
            string content = serializationOfNode(currNode, &bTreeMap);
            const char *carray = content.c_str();
            FileToWrite.write(carray, 512);
            for (auto *child: currNode->children) {
                q.push_back(child);
            }
        }
    }
    FileToWrite.close();
}

void readRawFile(string fileName) {
    cout << "\n>> Input Path is: " << fileName << endl;
    vector<Record> records = readRawTxtFile(fileName);
    cout << ">> Reading data file from " << fileName << ", found " << records.size() << " records." << endl;
    if (records.size() == 0) {
        string e = "";
        e += "ERROR:  You need to input the CORRECT path to the A1_6030.txt file. ";
        e += "0 record has been read from " + fileName;
        throw invalid_argument(e);
    }
    // let's load all the records into the block linked list:
    BlockListNode *b = new BlockListNode();
    BlockListNode *dummyHead = b;

    for (auto r: records) {
        b = b->insertRecordStringToBlock(r.content);
    }
    //1) Build a database file and save to disk; block size 1024;
    BlockListNode::saveToDisk(dummyHead, "database_file.txt");
    cout << "=====================================================================================" << endl;
    cout << "==== The input txt file is saved on disk as a database file: database_file.txt ======" << endl;
    //2.1) Build a B+Tree in RAM,
    b = dummyHead;
    BPlusTree<string> bTree = BPlusTree<string>(8);
    vector<string> block_record; // the ptr values saved on BTree leaf nodes
    vector<string> field_1s; // the keys saved on BTree leaf nodes;

    short block_cnt = 0;
    while (b != nullptr) {
        block_cnt++;
        for (int i = 0; i < b->getNumOfRecord(); ++i) {
            Record r = Record(b->getRecordAsString(i + 1));
            //1 based BlockNum RecordNum seperated by space,
            // convert two short into a byte
            short recordNum = i + 1;
            string val = StingShort(block_cnt, recordNum).str;
            string key = r.field1;
            field_1s.push_back(key);
            block_record.push_back(val);
            bTree.insert(key, val);
        }
        b = b->next;
    }

    cout << "==== totally " << records.size() << " records are saved on " << block_cnt
         << " blocks. ================================" << endl;
    cout << "==== Please wait while this program is generating the B+Tree file ......       ======" << endl;
    // save the B+Tree into Disk
    saveBTreeNodesOnDisk(&bTree, "bTree_file.txt");
    cout << "==== The B+Tree file is generated on the hard disk with name:  bTree_file.txt  ======" << endl;
    saveVectorOfField1ToDisk(&field_1s, "field_one_file.txt");
    cout << "==== All field ones are saved on the hard disk with name:  field_one_file.txt  ======" << endl;
    cout << "=====================      Program 1 Part 1 has finished        =====================" << endl;
}

// assignment q1 part 2-1
string searchDataBase(string key, string databaseFileName, string btreeFileName) {
    if (key.size() != 9) {
        string error;
        error.append("the search key must has length of 9, but you are given key with length ");
        error.append(to_string(key.size()));
        throw invalid_argument(error);
    }

    cout << "=======================================" << endl;
    cout << " Search For Key = " << key << endl;
    cout << "=======================================" << endl;
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
        string blockContent = readFileFromDiskByBlock(databaseFileName, value.block, 1024);
        BlockListNode bl = BlockListNode(blockContent);
        Record r = Record(bl.getRecordAsString(value.record));
        int numRecords = bl.getNumOfRecord();
        cout << ">> Reading a data block to RAM with " << bl.getEmptyBytes() << "/1024 empty Bytes, and "
             << to_string(numRecords) << " keys: ";
        for (int i = 1; i <= numRecords; ++i) {
            string s = bl.getRecordAsString(i);
            Record rr = Record(s);
            cout << rr.field1 << " ";
        }
        cout << "." << endl;
        cout << ">> After " << diskReadCnt + 1 << " times disk read (" << diskReadCnt
             << " times TreeNode read + 1 Data Block read), we found the key: " << key <<
             " at Block_Num " << value.block << ", and Record_Num " << value.record << ": " << endl;
        cout << endl;
        cout << r << endl;
        return r.field1;
    }
}

// assignment q1 part 2-2
void insertDataBase(string record_str, string databaseFileName, string btreeFileName) {
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
    int limit = BTreeVector.size();
    ifstream fin;
    fin.open(btreeFileName);
    for (int i = 1; i < limit; ++i) {
        readFileFromDiskByBlockReturnArrayWithoutFileClose(fin, btreeFileName, &BTreeVector, i, 512);
    }
    fin.close();
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

    cout << "Trying to insert the new record, Please wait..." << endl;
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
    while (oldNode != nullptr) {
        for (int i = 1; i <= oldNode->getNumOfRecord(); ++i) {
            if (Record(oldNode->getRecordAsString(i)).field1 == inputRecord.field1) {
                newRecordNum = i;
                oldNode = nullptr;
                break;
            }
        }
        if (oldNode != nullptr) {
            oldNode = oldNode->next;
            newBlockNum += 1;
        }
    }
    cout << "Going to insert the new record: " << inputRecord.field1 << " at Block, Record: " << newBlockNum << "  "
         << newRecordNum << endl;
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
//            short blockNum = block_cnt;
            short recordNum = i + 1;
            string record_val = StingShort(block_cnt, recordNum).str;
            string record_key = r.field1;
//            if (field_1s.size() > 0 && field_1s.back() >= record_key) {
//                string e = "";
//                e += "Block is invalid";
//                e += field_1s.back();
//                e += " " + record_key;
//                throw invalid_argument(e);
//            }
            field_1s.push_back(record_key);

            block_record.push_back(record_val);
        }
        b = b->next;
    }
    int cnter = 0;

    while (ptr != nullptr) {
        int curr_node_size = ptr->keys.size();
        for (int i = 0; i < curr_node_size; ++i) {
//            if (field_1s.at(cnter) != ptr->keys.at(i)) {
//                string e = "";
//                e += "leaf node value does not match block record sequence! \n At Index: ";
//                e += to_string(cnter);
//                e += " Field1 in Block is ";
//                e += field_1s.at(cnter);
//                e += ". Field1 in BTree is ";
//                e += ptr->keys.at(i);
//                throw invalid_argument(e);
//            }
            // update all the values and save back to the disk
            ptr->values[i] = block_record[cnter];
            cnter++;
        }
        ptr = ptr->next;
    }

    saveBTreeNodesOnDisk(&bPlusTree, "bTree_file.txt");
    cout << ">> Insert Success, the record for " << inputRecord.field1 << " has been inserted to Block#: "
         << newBlockNum << " and Record#: " << newRecordNum << endl;
    saveVectorOfField1ToDisk(&field_1s, "field_one_file.txt");
    cout<<">> All the field ones has been saved to file, field_one_file.txt, for verification purpose.\n";
}


string searchDataBaseWithoutPrintTerminal(string key, string databaseFileName, string btreeFileName) {
    if (key.size() != 9) {
        string error;
        error.append("the search key must has length of 9, but you are given key with length ");
        error.append(to_string(key.size()));
        throw invalid_argument(error);
    }


    HardDiskNode *h = new HardDiskNode();
    string str = readFileFromDiskByBlock(btreeFileName, 1, 512);
    h->deseralizeHardDiskNodeFromStr(str);


    while (!h->isLeaf) {
        short blockNum = h->searchNodeAtNonLeafNode(key);
        str = readFileFromDiskByBlock(btreeFileName, blockNum, 512);
        h->deseralizeHardDiskNodeFromStr(str, false);
    }
    str = h->searchValueOnLeafNode(key);
    string result;
    if (str == "-1") {
        string error;
        error.append("The Delete Key: " + key + " does not exist in the current B+Tree! Please check your input!");
        throw invalid_argument(error);
    } else {
        StingShort value = StingShort(str);
        cout << "Going to delete Record: " << key << " From Block Num: " << value.block << ", Record Num: "
             << value.record << endl;
        cout << "Please wait ..." << endl;
        result = value.str;
    }
    return result;
}

void deleteDataBase(string key, string databaseFileName, string btreeFileName) {
    string location = searchDataBaseWithoutPrintTerminal(key, databaseFileName, btreeFileName);
    // read all the blocks into RAM, and then delete one record from RAM's record
    // generate a new B+Tree to overwrite the old one;
    StingShort value = StingShort(location);

    //====================== Step-4 ===========================================
    //New a new Block Linked List, and read all the records from the block into a vector, insert the new record into it
    // sort, and write everything back to this block node again
    BlockListNode *b = new BlockListNode(readFileFromDiskByBlock("database_file.txt", 1, 1024));
    BlockListNode *insertNode = b;
    BlockListNode *dummyHead = b;
    vector<BlockListNode *> vecOfBlocks;
    vecOfBlocks.push_back(b);

    int blockSize = getNumOfBlocksFromHardDiskFile("database_file.txt", 1024);
    for (int j = 1; j < blockSize; ++j) {
        string currBlock = readFileFromDiskByBlock("database_file.txt", j + 1, 1024);
        b->next = new BlockListNode(currBlock);
        b = b->next;
        vecOfBlocks.push_back(b);
        if (value.block == (j + 1)) {
            insertNode = b; // save the ptr to the block that we want to modify!!!
        }
    }

    int numRecords = insertNode->getNumOfRecord();
    // get all the records from this block in a vector, delete the target one, and sort the rest, and put them back;
    if (numRecords <= 1) {
        if (value.block == 1) {
            // if the first node is empty after delete, we assign the second node as head, then we are done;
            dummyHead = dummyHead->next;
        } else {
            // otherwise, we can just delete the current node
            BlockListNode *prev = vecOfBlocks.at(value.block - 2);
            prev->next = insertNode->next;
        }
    } else {
        // if there are at least one record in the block after delete:
        vector<Record> records;
        for (int m = 1; m <= numRecords; ++m) {
            Record currRecord = Record(insertNode->getRecordAsString(m));
            if (currRecord.field1 != key) {
                records.push_back(currRecord);
            }
        }
        sort(records.begin(), records.end(), [](Record a, Record b) { return (a.field1 < b.field1); });
        insertNode->setNumOfRecord(0); // clear the whole block
        // insert all the records back
        for (Record rr: records) {
            insertNode = insertNode->insertRecordStringToBlock(rr.content);
        }
        // 1) Build a database file and save to disk; block size 1024;
    }

    BlockListNode::saveToDisk(dummyHead, "database_file.txt");
    // Collect all the records from the RAM, and save them as key-value paris, need to insert them to B+Tree latter;
    vector<string> block_record;
    vector<string> field_1s;
    BPlusTree<string> bPlusTree = BPlusTree<string>(8);

    int block_cnt = 0;
    b = dummyHead;
    while (b != nullptr) {
        block_cnt++;
        for (int i = 0; i < b->getNumOfRecord(); ++i) {
            Record r = Record(b->getRecordAsString(i + 1));
            //1 based BlockNum RecordNum seperated by space,
            // convert two short into a byte
//            short blockNum = block_cnt;
            short recordNum = i + 1;
            string record_val = StingShort(block_cnt, recordNum).str;
            string record_key = r.field1;
            if (field_1s.size() > 0 && field_1s.back() >= record_key) {
                string e = "";
                e += "Block is invalid";
                e += field_1s.back();
                e += " " + record_key;
                throw invalid_argument(e);
            }
            field_1s.push_back(record_key);
            block_record.push_back(record_val);
            bPlusTree.insert(record_key, record_val);
        }
        b = b->next;
    }
    saveBTreeNodesOnDisk(&bPlusTree, "bTree_file.txt");
    cout << "Delete Record: " << key << " Successfully." << endl;
    saveVectorOfField1ToDisk(&field_1s, "field_one_file.txt");
    cout<<">> All the field ones has been saved to file, field_one_file.txt, for verification purpose.\n";
}

void rangeSearch(string key1, string key2, std::string databaseFileName, std::string btreeFileName) {
    // get the two keys
    searchDataBaseWithOutPrint(key1, key2, databaseFileName,btreeFileName );
}

// assignment q1 part 2-1
string searchDataBaseWithOutPrint(string key, string keyEnd, string databaseFileName, string btreeFileName) {

    HardDiskNode *h = new HardDiskNode();
    string str = readFileFromDiskByBlock(btreeFileName, 1, 512);
    h->deseralizeHardDiskNodeFromStr(str, false); // get the btree node without print


    while (!h->isLeaf) {
        short blockNum = h->searchNodeAtNonLeafNode(key);
        str = readFileFromDiskByBlock(btreeFileName, blockNum, 512);
        h->deseralizeHardDiskNodeFromStr(str, false);
    }
    if (h->searchValueOnLeafNode(key) == "-1") {
        cout << ">> Error: Range Search Failed, Please read the error msg below, and retry:" << endl;
        cout << ">> The first input key: " << key << " does not exist in the BTree" << endl;
        return "-1";
    }
    string startStr = h->values.front();

    string str2 = readFileFromDiskByBlock(btreeFileName, 1, 512);
    h->deseralizeHardDiskNodeFromStr(str2, false); // get the btree node without print

    while (!h->isLeaf) {
        short blockNum = h->searchNodeAtNonLeafNode(keyEnd);
        str2 = readFileFromDiskByBlock(btreeFileName, blockNum, 512);
        h->deseralizeHardDiskNodeFromStr(str2, false);
    }
    if (h->searchValueOnLeafNode(keyEnd) == "-1") {
        cout << ">> Error: Range Search Failed, Please read the error msg below, and retry:" << endl;
        cout << ">> The second input key: " << key << " does not exist in the BTree" << endl;
        return "-1";
    }
    string endStr = h->values.back();


    int numOfBlocksToRead = StingShort(endStr).block - StingShort(startStr).block;
    vector<Record*> recordsToPrint;
    short starBlockNum = StingShort(startStr).block;
    while (numOfBlocksToRead>=0){
        BlockListNode bl = BlockListNode(readFileFromDiskByBlock(databaseFileName, starBlockNum, 1024));
        //loop
        int limit = bl.getNumOfRecord();
        for (int i = 1; i <= limit; ++i) {
            Record* r = new Record(bl.getRecordAsString(i));
            if(r->field1>=key && r->field1<=keyEnd){
                recordsToPrint.push_back(r);
            }
        }
        starBlockNum++;
        numOfBlocksToRead--;
    }
    for(Record* rr: recordsToPrint){
        cout<<"---------------------------------------"<<endl;
        cout<<*rr<<endl;
        delete rr; // delete all the obj has been newed before??? I guess this is how it works?
    }
    if (recordsToPrint.empty()){
        throw invalid_argument("There is no record matches the range search. ");
    }
    return "";
}




