//
// Created by Yao-Wen Mei on 2022/10/4.
//
#include <iostream>
#include <algorithm>    // std::max
#include <cstdint>
#include <cstring>
#include <fstream>
#include <vector>
#include <bitset>
#include <unistd.h>
#include "HashTableBlock.h"
#include "BytesOperation.h"

using namespace std;

void writefileToDiskByBlock(const string &fileName, int blockNum, int blockSize, char *buffer) {
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
//        cout<<"======== make sure saving is correct ====="<<endl;
//        unsigned short x = *(unsigned short *)&buffer[0];
//        unsigned short y = *(unsigned short *)&buffer[2];
//        unsigned short z = *(unsigned short *)&buffer[4];
//        unsigned short w = *(unsigned short *)&buffer[6];
//        cout<<x<<endl;
//        cout<<y<<endl;
//        cout<<z<<endl;
//        cout<<w<<endl;
        fin.write(buffer, blockSize);
    }
    fin.close();
}

string readFileFromDiskByBlockReturnString(string filename, int blockNum, int blockSize) {
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
//        bool isLeaf = content[0];
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

void readFileFromDiskByBlock(const string &fileName, int blockNum, int blockSize, char *buffer) {
    ifstream fin;
    fin.open(fileName);
    long shift;
    if (blockNum == 1) {
        shift = 0L;
    } else {
        shift = (blockNum - 1) * blockSize;
    }
    fin.seekg(shift, ios::beg);

    if (fin.good()) {
        fin.read(buffer, blockSize);
//        cout<<"======== make sure read is correct ====="<<endl;
//        unsigned short x = *(unsigned short *)&buffer[0];
//        unsigned short y = *(unsigned short *)&buffer[2];
//        unsigned short z = *(unsigned short *)&buffer[4];
//        unsigned short w = *(unsigned short *)&buffer[6];
//        cout<<x<<endl;
//        cout<<y<<endl;
//        cout<<z<<endl;
//        cout<<w<<endl;
    }
    fin.close();
}

unsigned short getNumOfBlocksFromHardDiskFile(const std::string& filename, int blockSize) {
    ifstream fin(filename);
    char buffer[blockSize];
    unsigned short cnt = 0;
    while (!fin.eof()) {
        fin.read((char *) &buffer, sizeof(buffer));
        if (fin.peek() == '\n') break; // avoid read the last line twice
        cnt++;
    }
    fin.close();
    return cnt;
}

void HashTable::insert(const std::string& key, std::string value,
                       unsigned short (*fhash)(const std::string &hashkey, short hashi)) {
//    std::cout << key << std::endl;
    // find out which block to put the record
    unsigned short bucketKey = fhash(key, i); //suppose key is 10
//    cout<<"Insert to bucket num "<<bucketKey<<" with i ="<< i <<endl;
//    cout << "The bucketKey for " << key << " is " << bucketKey << endl;
    // put it there
    if (buckets.size() <= bucketKey) {
//        cout<<" ========= Need to increase bucket size =============="<<to_string( buckets.size())<<endl;
        unsigned short modifiedBucketKey = getBlockNumNeedToSplit(bucketKey);
        buckets.at(modifiedBucketKey).insertToBlock(key, value);
    } else {
        buckets.at(bucketKey).insertToBlock(key, value);
    }
    r++;
    double currThreshold = (r + 0.0) / (n + 0.0);
//    cout << " The curren threshold is r/n: " << currThreshold << endl;
    // if we need to create a new bucket to reduce the threshold;
    if (currThreshold > threshold) {
//        cout << " the threshold is overflow, need to create new bucket" << endl;
        this->buckets.emplace_back();
//        n++;
        n = buckets.size();
//        cout << " ========= Need to increase bucket size ==============" << to_string(buckets.size()) << endl;
        // check to see if we need to increase i now
        unsigned short limit = 1 << i;
//        cout << " n > 2^^i: " << n << " >?" << limit << endl;
        if (n > limit) {
            i++; // when n exceeds 2^i, increase i by one;
        }
        // check to see if we need to split now;
//        coutShortToBits(n);
//        cout << " the bucket with this num has been added" << endl;
        unsigned short splitBucketNum = getBlockNumNeedToSplit(n - 1);
//        coutShortToBits(splitBucketNum);
//        cout << " need to be splited" << endl;
        auto *oldBucket = &(this->buckets.at(splitBucketNum));
        auto *newBucket = &(this->buckets.at(n - 1));
        vector<tuple<string, string>> leftover;
        for (tuple<string, string> r: oldBucket->records) {

//            coutShortToBits(fhash(get<0>(r), i));
//            coutShortToBits(splitBucketNum);
            if (fhash(get<0>(r), i) == splitBucketNum) {
                leftover.push_back(r);
            } else if (fhash(get<0>(r), i) == (n - 1)) {
                newBucket->records.push_back(r);
            } else {
                throw invalid_argument("These is a key does not belong to any after split!");
//                cout << "There are keys shifted: " << get<0>(r) << endl;
            }
        }
        // the records in bucket old Bucket are distributed into two buckets, these records stays;
        oldBucket->records = leftover;
    }
}

std::string
HashTable::search(const std::string &key, unsigned short (*fhash)(const std::string &hashkey, short hashi)) {
    unsigned short bucketKey = fhash(key, i); //suppose key is 10
    // get the HashTable block from the hashtable
    std::string result = "-1";
    unsigned short realSearchKey;
    // put it there
    if (buckets.size() <= bucketKey) {
        realSearchKey = getBlockNumNeedToSplit(bucketKey);
    } else {
        realSearchKey = bucketKey;
    }

    HashTableBlock htb = this->buckets.at(realSearchKey);
    // print all the records in the buckets until find a match
    auto rs = htb.records;
    for (auto r: rs) {
        string record_key = get<0>(r);
        if (record_key == key) {
            string record_val = get<1>(r);
            result = record_val;
            break;
        }
    }
    // if didn't find a match in the current bucket, we have also check its previous unsplited bucket;
    if (result == "-1") {
        unsigned short modifiedBucketKey = getBlockNumNeedToSplit(realSearchKey);
        htb = this->buckets.at(modifiedBucketKey);
        rs = htb.records;
        for (auto r: rs) {
            string record_key = get<0>(r);
            if (record_key == key) {
                string record_val = get<1>(r);
                result = record_val;
                break;
            }
        }
    }

    // if cannot find a match, throw the error message; otherwise, return the value
    if (result == "-1") {
        throw invalid_argument("Input key does not exist in the hashmap, please check your input");
    }
    return result;

}

long getShift(int blockNum, int blockSize) {
    long shift = (blockNum - 1) * blockSize;
    if (blockNum == 1) {
        shift = 0L;
    }
    return shift;
}

void HashTable::saveToDisk(const string &fileName) {
    fstream fin;
    fin.open(fileName);
    // if file does not exist, create a new one; if it is already exist, append new blocks into it;
    if (!fin.is_open()) {
        fin.open(fileName, fstream::in | fstream::out | fstream::app);
    }


    // header file contains i, n, r, overflow shift (overflow will start from block 15000)
    // block size 200 kb, # records (2 bytes) + overflow jump (2 bytes) 10 records (9+4)x10 = 150 bytes
    char buffer[200] = {'\0'};
    char *b = buffer;
    // write the header
    unsigned short currBLKnum = 1;
    // Use memcpy copy to write to a file!
    memcpy(b, &this->i, sizeof(currBLKnum));
    memcpy(b + 2, &this->n, sizeof(currBLKnum));
    memcpy(b + 4, &this->r, sizeof(currBLKnum));
    unsigned short overflowsize = 15000;
    memcpy(b + 6, &overflowsize, sizeof(overflowsize));

    // write to file
    fin.seekp(getShift(currBLKnum, 200));
    fin.write(buffer, 200);


    int limit = this->buckets.size() + 1;
    for (int j = 2; j <= limit; ++j) {
        currBLKnum++;
        char recordBuff[200] = {'\0'};
        HashTableBlock currBucket = this->buckets.at(j - 2);
        // if this can be fit in one bucket, write all the records into the bucket;
        if (currBucket.records.size() <= 10) {
            unsigned short numRecords = currBucket.records.size();
            unsigned short nextOverflow = 0;
            memcpy(recordBuff, &numRecords, 2);
            memcpy(recordBuff+2, &nextOverflow, 2);
            int idx = 4;
            for (int k = 0; k < currBucket.records.size(); ++k) {
                tuple<std::string, std::string> r = currBucket.records.at(k);
                string field1 = get<0>(r);
                string location = get<1>(r);
                memcpy(recordBuff + idx, field1.c_str(), 9);
                memcpy(recordBuff + idx + 9, location.c_str(), 2);
                idx = idx + 11;
//                if( field1 == "gpxiqvkuy"){
//                    cout<<"============== found key gpxiqvkuy ==========="<<currBLKnum<<endl;
//                    cout<<currBucket.records.size()<<endl;
//                    for(int i = 0; i<55; i++){
//                        cout<<recordBuff[i+4];
//                    }
//                    cout<<endl;
//                }
            }
            if(currBLKnum == 3398){
//                field1 == "gpxiqvkuy"
                cout<<"============== found key gpxiqvkuy ==========="<<currBLKnum<<endl;
//                cout<<field1<<endl;
                cout<<currBucket.records.size()<<endl;
                for(int i = 0; i<55; i++){
                    cout<<recordBuff[i+4];
                }
                cout<<endl;
            }
            // write to file
            fin.seekp(getShift(currBLKnum, 200));
            fin.write(recordBuff, 200);
//            unsigned int microsecond = 500;
//            usleep(3 * microsecond);//sleeps for 3 second

        } else {
            //todo: create a overlfow bucket after 15000, updpate header as well;
        }
    }
    fin.close();
}

HashTableFromDisk::HashTableFromDisk(const string &fileName) {
    // read a Btree file, and generate a Disk hash table.
    char buffer[200];
    readFileFromDiskByBlock(fileName, 1, 200, buffer);
    this->i = *(unsigned short *) &buffer[0];
    this->n = *(unsigned short *) &buffer[2];
    this->r = *(unsigned short *) &buffer[4];
    this->overflowShift = *(unsigned short *) &buffer[6];
    unsigned short numBlocks = getNumOfBlocksFromHardDiskFile(fileName);
    cout<<"Totally found "<<to_string(numBlocks)<<" blocks in the hashmap disk file (including one file header block)"<<endl;
}

void HashTableFromDisk::searchForKey(const string &key, unsigned short (*fhash)(const std::string &hashkey, short hashi), const string &fileName){
    unsigned short bucketKey = fhash(key, i); //suppose key is 10
    // get the HashTable block from the hashtable
    std::string result = "-1";
    unsigned short realSearchKey;

    // put it there
    if (this->n <= bucketKey) {
        realSearchKey = getBlockNumNeedToSplit(bucketKey);
    } else {
        realSearchKey = bucketKey;
    }

    char buffer[200];
    readFileFromDiskByBlock(fileName, realSearchKey+2, 200, buffer);
    unsigned short numRecords = *(unsigned short *)&buffer[0];
    cout<<">> Read Disk Found that: there are "<<to_string(numRecords)<<" in the block."<<endl;
    vector<string> field1s;
    int start = 4;
    while (numRecords>0){
        string f = "";
        for(int i = start; i<start+9; i++){
            f += buffer[i];
        }
        field1s.push_back(f);
        start = start + 11;
        numRecords--;
    }

    for(string s: field1s){
        cout<<s<<endl;
    }
    cout<<endl;
    //        unsigned short x = *(unsigned short *)&buffer[0];

    // read the block and get the totally number of records:


}
