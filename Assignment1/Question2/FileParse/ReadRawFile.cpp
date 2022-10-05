//
// Created by Yao-Wen Mei on 2022/10/3.
//
#include <iostream>
#include <fstream>
#include <tuple>
#include <stdexcept>
#include <vector>
#include <filesystem>
#include <algorithm>

#include "../Utility/BytesOperation.h"
#include "../Utility/BlockListNode.h"
#include "../Utility/Record.h"
#include "../Utility/HashTableBlock.h"
#include "ReadRawFile.h"

using namespace std;

// read a file and get vector of sorted records
vector<Record> readRawTxtFile(const string& fileName) {
    string inputText;
    // Read from the text file
    ifstream MyReadFile(fileName);
    vector<Record> records;
    while (getline(MyReadFile, inputText)) {
        // Output the text from the file
        Record r = Record(inputText);
        records.push_back(r);
    }
//    sort(records.begin(), records.end(), [](const Record& a, const Record& b) { return (a.field1 < b.field1); });
    return records;
}


void readRawFile(const string& fileName) {
    cout << "\n>> Input Path is: " << fileName << endl;
    vector<Record> records = readRawTxtFile(fileName);
    cout << ">> Reading data file from " << fileName << ", found " << records.size() << " records." << endl;
    if (records.empty()) {
        string e;
        e += "ERROR:  You need to input the CORRECT path to the A1_6030.txt file. ";
        e += "0 record has been read from " + fileName;
        throw invalid_argument(e);
    }
    // let's load all the records into the block linked list:
    auto *b = new BlockListNode();
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
        }
        b = b->next;
    }

    cout << "==== totally " << records.size() << " records are saved on " << block_cnt
         << " blocks. ================================" << endl;
    cout<<"=====================================================================================\n";
    // save this two vectors in a BTree in Ram:
    HashTable ht = HashTable();
    for(Record r: records){
        ht.insert(r.field1, r.content, getHashmapKey);
    }
    cout<<ht<<endl;
    ht.saveToDisk();
    cout<<"================== The first part (saving) of A1Q2 is finished ======================\n";
    cout<<"=====================================================================================\n";
}