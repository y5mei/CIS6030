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
#include <cmath>
#include <sys/stat.h>
#include <unistd.h>
#include "HashTableBlock.h"
#include "BytesOperation.h"
#include "BlockListNode.h"
#include "Record.h"

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

long GetFileSize(std::string filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

unsigned short getNumOfBlocksFromHardDiskFile(const std::string &filename, int blockSize) {

    long totalSize = GetFileSize(filename);
    unsigned short result = totalSize/blockSize;
    return result;
//    ifstream fin(filename);
//    char buffer[blockSize];
//    unsigned short cnt = 0;
//    while (!fin.eof()) {
//        fin.read((char *) &buffer, sizeof(buffer));
//        if (fin.peek() == '\n') break; // avoid read the last line twice
//        cnt++;
//    }
//    fin.close();
//    return cnt;
}

// insert a record into a database file, and then save the block # and record # into hashmap file
void insertDataBase(string record_str, string databaseFileName, string btreeFileName) {
    //1) get how many blocks in this file, then the new data is going to be inserted in block+1 and record 1
    //2) after insert to the database file, we need to pass these two value formed string to HashmapDisk insert method
    //====================== Step-1 ===========================================
    // save to database file
    Record inputRecord = Record(record_str);
    string key = inputRecord.field1;
    int size = getNumOfBlocksFromHardDiskFile(databaseFileName, 1024);
    unsigned short newBlockNum = size + 1;
    BlockListNode newblock = BlockListNode(1024);
    newblock.insertRecordStringToBlock(record_str);
    char *newContent = newblock.block;
    writefileToDiskByBlock(databaseFileName, newBlockNum, 1024, newContent);
    cout << " The new record for " << key << " has been inserted into Block# " << newBlockNum
         << " and Record# 1 in the database file." << endl;
    //====================== Step-2 ===========================================
    cout<<"Inserting the key-value pair to the hashamp......"<<endl;
    HashTableFromDisk htDisk = HashTableFromDisk();
    string value = StingShort(newBlockNum, 1).str;
    htDisk.insert(key, value,getHashmapKey);

}

void HashTable::insert(const std::string &key, std::string value,
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
            cout << "Found a match for dfzwsfgog" << endl;
            string record_val = get<1>(r);
            cout << record_val.size() << endl;
            result = record_val;
            break;
        }
    }
    // if didn't find a match in the current bucket, we have also check its previous unsplited bucket;
    if (result == "-1") {
        cout << " The key :" << key << " is in its non split parent bucket" << endl;
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
    ofstream fin(fileName, ofstream::trunc); // erase the content before write

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
            memcpy(recordBuff + 2, &nextOverflow, 2);
            int idx = 4;
            int recordSize = currBucket.records.size();
            for (int k = 0; k < recordSize; ++k) {
                tuple<std::string, std::string> r = currBucket.records.at(k);
                string field1 = get<0>(r);
                string location = get<1>(r);
                memcpy(recordBuff + idx, field1.c_str(), 9);
                memcpy(recordBuff + idx + 9, location.c_str(), 4);
                idx = idx + 13;
            }
            // write to file
            fin.seekp(getShift(currBLKnum, 200));
            fin.write(recordBuff, 200);
//            unsigned int microsecond = 500;
//            usleep(3 * microsecond);//sleeps for 3 second

        } else {
            int numRecordsToWrite = currBucket.records.size();
            int numOfOverflowBuckets = ceil(numRecordsToWrite / 10.0) - 1;
            // write the 1st 10 records to the normall block:
            unsigned short numRecords = 10;
            unsigned short nextOverflow = overflowsize++;
            memcpy(recordBuff, &numRecords, 2);
            memcpy(recordBuff + 2, &nextOverflow, 2);
            int idx = 4;
            for (int k = 0; k < 10; ++k) {
                tuple<std::string, std::string> nr = currBucket.records.at(numRecordsToWrite - 1);
                string field1 = get<0>(nr);
                string location = get<1>(nr);
                memcpy(recordBuff + idx, field1.c_str(), 9);
                memcpy(recordBuff + idx + 9, location.c_str(), 4);
                idx = idx + 13;
                numRecordsToWrite--;
            }
            // write to file
            fin.seekp(getShift(currBLKnum, 200));
            fin.write(recordBuff, 200);

            // write the rest to overflow bucket
            while (numOfOverflowBuckets > 0) {
//                cout << get<0>(currBucket.records.at(numRecordsToWrite - 1))
//                     << " is on the overflow bucket at bucket number " << (overflowsize - 1) << endl;
                char overflowRecordBuff[200] = {'\0'};
                numRecords = min(10, numRecordsToWrite); // todo: Is this a safe way to cast double to ushort?
                if (numRecordsToWrite > 10) {
                    nextOverflow = overflowsize++; // do we need another overflow bucket?
                } else {
                    nextOverflow = 0;
                }
                memcpy(overflowRecordBuff, &numRecords, 2);
                memcpy(overflowRecordBuff + 2, &nextOverflow, 2);
                idx = 4;
                for (int k = 0; k < numRecords; ++k) {
                    auto ofr = currBucket.records.at(numRecordsToWrite - 1);
                    string field1 = get<0>(ofr);
                    string location = get<1>(ofr);
                    memcpy(overflowRecordBuff + idx, field1.c_str(), 9);
                    memcpy(overflowRecordBuff + idx + 9, location.c_str(), 4);
                    idx = idx + 13;
                    numRecordsToWrite--;
                }
                numOfOverflowBuckets--;
                // write to file
                fin.seekp(getShift(overflowsize - 1, 200));
                fin.write(overflowRecordBuff, 200);
            }
        }
    }
    // Now need to update the header again for the last updated overflow position
    // header file contains i, n, r, overflow shift (overflow will start from block 15000)
    // block size 200 kb, # records (2 bytes) + overflow jump (2 bytes) 10 records (9+4)x10 = 150 bytes
    buffer[200] = {'\0'};
    char *bb = buffer;
    // write the header
    currBLKnum = 1;
    // Use memcpy copy to write to a file!
    memcpy(bb, &this->i, sizeof(currBLKnum));
    memcpy(bb + 2, &this->n, sizeof(currBLKnum));
    memcpy(bb + 4, &this->r, sizeof(currBLKnum));
    memcpy(bb + 6, &overflowsize, sizeof(overflowsize));

    // write to file
    fin.seekp(getShift(currBLKnum, 200));
    fin.write(buffer, 200);
    fin.close();
    cout << "=====================================================================================\n"
            "==== The Hash table file is saved on disk as :         " << fileName << "    ======\n"
                                                                                     "====================================================================================="
         << endl;

}

HashTableFromDisk::HashTableFromDisk(const string &fileName) {
    // read a Btree file, and generate a Disk hash table.
    char buffer[200];
    readFileFromDiskByBlock(fileName, 1, 200, buffer);
    this->i = *(unsigned short *) &buffer[0];
    this->n = *(unsigned short *) &buffer[2];
    this->r = *(unsigned short *) &buffer[4];
    this->overflowShift = *(unsigned short *) &buffer[6];
//    unsigned short numBlocks = getNumOfBlocksFromHardDiskFile(fileName);
    cout << "=====================================================================================\n" << endl;
    cout << "==== Read Hashmap File Header Block to get i, n, r, overflow_index===================\n" << endl;
    cout << *this << endl;
}

std::string HashTableFromDisk::searchForKeyWithHashedKey(const string &key, unsigned short bucketKey,
                                                         unsigned short (*fhash)(const std::string &, short),
                                                         const string &fileName, int diskReadCnter) {

    // the caller will provide a key, a hashed int, and a diskReadCnter
    int diskReadCount = diskReadCnter;
    // get the HashTable block from the hashtable
    std::string result = "-1";
    unsigned short realSearchKey;

    cout << ">> Didn't find the key in this original bucket. Now need to double check the key's non-split parent: "
         << endl;
    // where to get it's non-splited parent?
    realSearchKey = getBlockNumNeedToSplit(bucketKey);

    // if didn't find it in the current realSearchKey, we also need to check the non-splited one, in case if it is there.

    char buffer[200];
    readFileFromDiskByBlock(fileName, realSearchKey + 2, 200, buffer);
    unsigned short numRecords = *(unsigned short *) &buffer[0];
    unsigned short overflowBucketPosition = *(unsigned short *) &buffer[2];
    cout << ">> Read Disk at BlockNum: " << realSearchKey + 2 << " found that: there are " << to_string(numRecords)
         << " records in this block ";
    if (overflowBucketPosition != 0) {
        cout << "and this bucket has an overflow bucket at block number " << overflowBucketPosition;
    }
    cout << "." << endl;
    vector<string> field1s;
    vector<string> values;
    int start = 4;
    while (numRecords > 0) {
        string f;
        string v;
        for (int i = start; i < start + 9; i++) {
            f += buffer[i];
        }
        for (int j = start + 9; j < start + 13; ++j) {
            v += buffer[j];
        }
        field1s.push_back(f);
        values.push_back(v);
        start = start + 13;
        numRecords--;
    }
    // printout all the keys:
    cout << "    ----The keys in this buckets are: ";
    for (const string &s: field1s) {
        cout << s << " ";
    }
    cout << endl;
    cout << "=====================================================================================" << endl;
    // see if we really have this key:
    int limit = field1s.size();
    for (int k = 0; k < limit; ++k) {
        if (field1s.at(k) == key) {
            cout << " After " << diskReadCount + 1
                 << " times disk reads (1 time Hashtable file header read, 1 time database file read + "
                 << diskReadCount - 1 << " times Hashmap bucket reads), We have found the key " << key
                 << " in this non-split parent bucket above." << endl;
            return values.at(k);
        }
    }
    // TODO: if there is an overflow block, we need to check the overflow bucket with a while loop, if still nothing, return a -1;

    while (overflowBucketPosition != 0) {
        diskReadCount++;
        // keep checking overflow buckets:
        char overflowBuffer[200];
        readFileFromDiskByBlock(fileName, overflowBucketPosition, 200, buffer);
        numRecords = *(unsigned short *) &overflowBuffer[0];
        overflowBucketPosition = *(unsigned short *) &overflowBuffer[2];
        cout << ">> Read Disk at OVERFLOW BlockNum: " << overflowBucketPosition << " found that: there are "
             << to_string(numRecords) << " records in this over flow block ";
        if (overflowBucketPosition != 0) {
            cout << "and this bucket has an overflow bucket at block number " << overflowBucketPosition;
        }
        cout << "." << endl;
        field1s.clear();
        values.clear();
        start = 4;
        while (numRecords > 0) {
            string f;
            string v;
            for (int i = start; i < start + 9; i++) {
                f += buffer[i];
            }
            for (int j = start + 9; j < start + 13; ++j) {
                v += buffer[j];
            }
            field1s.push_back(f);
            values.push_back(v);
            start = start + 13;
            numRecords--;
        }
        // printout all the keys:
        cout << "    ----The keys in this buckets are: ";
        for (const string &s: field1s) {
            cout << s << " ";
        }
        cout << endl;
        cout << "=====================================================================================" << endl;
        int fieldSize = field1s.size();
        for (int k = 0; k < fieldSize; ++k) {
            if (field1s.at(k) == key) {
                cout << " After " << diskReadCount + 1
                     << " times disk reads (1 time Hashtable file header read, 1 time database file read + "
                     << diskReadCount - 1 << " times Hashmap bucket reads), We have found the key " << key
                     << " in this OVERFLOW bucket of the non-split parent above." << endl;
                return values.at(k);
            }
        }
    }

    cout << "Error: the Key: " << key << " does not exist after " << diskReadCount
         << " times disk reads (1 time Hashtable file header read + " << diskReadCount - 1
         << " times Hashmap bucket reads)." << endl;
    return "-1";
}


std::string
HashTableFromDisk::searchForKey(const string &key, unsigned short (*fhash)(const std::string &hashkey, short hashi),
                                const string &fileName) {
    int diskReadCount = 2;
    unsigned short bucketKey = fhash(key, i); //suppose key is 10
    // get the HashTable block from the hashtable
    std::string result = "-1";
    unsigned short realSearchKey;

    // where to get it?
    if (this->n <= bucketKey) {
        realSearchKey = getBlockNumNeedToSplit(bucketKey);
    } else {
        realSearchKey = bucketKey;
    }
    unsigned short origionalBucketKey = realSearchKey;

    // if didn't find it in the current realSearchKey, we also need to check the non-splited one, in case if it is there.
    cout << "=====================================================================================" << endl;
    cout << "============================== Start Searching ======================================" << endl;
    char buffer[200];
    readFileFromDiskByBlock(fileName, realSearchKey + 2, 200, buffer);
    unsigned short numRecords = *(unsigned short *) &buffer[0];
    unsigned short overflowBucketPosition = *(unsigned short *) &buffer[2];
    cout << ">> Read Disk at BlockNum: " << realSearchKey + 2 << " found that: there are " << to_string(numRecords)
         << " records in this block ";
    if (overflowBucketPosition != 0) {
        cout << "and this bucket has an overflow bucket at block number " << overflowBucketPosition;
    }
    cout << "." << endl;
    vector<string> field1s;
    vector<string> values;
    int start = 4;
    while (numRecords > 0) {
        string f;
        string v;
        for (int i = start; i < start + 9; i++) {
            f += buffer[i];
        }
        for (int j = start + 9; j < start + 13; ++j) {
            v += buffer[j];
        }
        field1s.push_back(f);
        values.push_back(v);
        start = start + 13;
        numRecords--;
    }
    // printout all the keys:
    cout << "    ----The keys in this buckets are: ";
    for (const string &s: field1s) {
        cout << s << " ";
    }
    cout << endl;
    cout << "=====================================================================================" << endl;
    // see if we really have this key:
    int fieldSize =  field1s.size();
    for (int k = 0; k < fieldSize; ++k) {
        if (field1s.at(k) == key) {
            cout << " After " << diskReadCount + 1
                 << " times disk reads (1 time Hashtable file header read, 1 time database file read + "
                 << diskReadCount - 1 << " times Hashmap bucket reads), We have found the key " << key
                 << " in this bucket above." << endl;
            return values.at(k);
        }
    }
    // TODO: if there is an overflow block, we need to check the overflow bucket with a while loop, if still nothing, return a -1;

    while (overflowBucketPosition != 0) {
        diskReadCount++;
        // keep checking overflow buckets:
        char overflowBuffer[200];
        readFileFromDiskByBlock(fileName, overflowBucketPosition, 200, buffer);
        numRecords = *(unsigned short *) &overflowBuffer[0];
        overflowBucketPosition = *(unsigned short *) &overflowBuffer[2];
        cout << ">> Read Disk at OVERFLOW BlockNum: " << overflowBucketPosition << " found that: there are "
             << to_string(numRecords) << " records in this over flow block ";
        if (overflowBucketPosition != 0) {
            cout << "and this bucket has an overflow bucket at block number " << overflowBucketPosition;
        }
        cout << "." << endl;
        field1s.clear();
        values.clear();
        start = 4;
        while (numRecords > 0) {
            string f;
            string v;
            for (int i = start; i < start + 9; i++) {
                f += buffer[i];
            }
            for (int j = start + 9; j < start + 13; ++j) {
                v += buffer[j];
            }
            field1s.push_back(f);
            values.push_back(v);
            start = start + 13;
            numRecords--;
        }
        // printout all the keys:
        cout << "    ----The keys in this buckets are: ";
        for (const string &s: field1s) {
            cout << s << " ";
        }
        cout << endl;
        cout << "=====================================================================================" << endl;
        int fieldSizeLimit = field1s.size();
        for (int k = 0; k < fieldSizeLimit; ++k) {
            if (field1s.at(k) == key) {
                cout << " After " << diskReadCount + 1
                     << " times disk reads (1 time Hashtable file header read, 1 time database file read + "
                     << diskReadCount - 1 << " times Hashmap bucket reads), We have found the key " << key
                     << " in this OVERFLOW bucket above." << endl;
                return values.at(k);
            }
        }
    }
    // calculate the non-split parent
    result = this->searchForKeyWithHashedKey(key, origionalBucketKey,
                                             (*fhash), fileName, diskReadCount + 1);
    return result;
}

void
HashTableFromDisk::insert(const string &key, std::string value, unsigned short (*fhash)(const std::string &, short),
                          const string &fileName) {

    unsigned short bucketKey = fhash(key, i); //suppose key is 10
// todo: check do we need to put in its nonsplit parent!
//    insert key, value to the block (hash+i)
    unsigned short actualBucketKey;
//    unsigned short lastNumOfBlocksInDisk = n + 1;
//    unsigned short numOfBlocksWantToInsert = bucketKey + 2;
    if (this->n <= bucketKey) {
//        cout<<" ========= Need to increase bucket size =============="<<to_string( buckets.size())<<endl;
        unsigned short modifiedBucketKey = getBlockNumNeedToSplit(bucketKey);
        actualBucketKey = modifiedBucketKey;
    } else {
        actualBucketKey = bucketKey;
    }
    // HashMapBucket b1 = char[200] at actualBucketKey place;
    // insert the record str into it (if not over limit of 10, just insert it, otherwise, insert it to overflow place)
    insertToDiskBlock(key, value, actualBucketKey+2, fileName);
    r++;

    cout<<"==========================================="<<endl;
    cout<<"==========================================="<<endl;
    cout<<" The new key has been inserted "<<endl;
    cout<<"==========================================="<<endl;
    cout<<"==========================================="<<endl;
    cout<<"==========================================="<<endl;
    double currThreshold = (r + 0.0) / (n + 0.0);
    if (currThreshold > threshold) {
//        cout << " the threshold is overflow, need to create new bucket" << endl;
        // a new bucket here is a buffer of 200, 2 bytes num records, 2 bytes overflow, 13 bytes records .... to the end;
        // then n++
        // then suppose we already write n+1 bucket in disk, check if we need to increase i;

        char *newBucketContent[200];
        n++;
//        cout << " ========= Need to increase bucket size ==============" << to_string(buckets.size()) << endl;
        // check to see if we need to increase i now
        unsigned short limit = 1 << i;
        if (n > limit) {
            i++; // when n exceeds 2^i, increase i by one;
        }
        // check to see if we need to split now;
        unsigned short splitBucketNum = getBlockNumNeedToSplit(n - 1);
        // TODO:
        // say hash is 11 (3), then the actual block need to be read is the #hash bucket in vector,
        // because hard disk file has one header, and is 1 indexed, so we need to read #hash+2 block from the hard disk;
        // When we write, we also need to write the new block to (hash + 2) block in the database file

//        coutShortToBits(splitBucketNum);
//        cout << " need to be splited" << endl;
//        auto *oldBucket = &(this->buckets.at(splitBucketNum));
//        auto *newBucket = &(this->buckets.at(n - 1));
//        vector<tuple<string, string>> leftover;
//        for (tuple<string, string> r: oldBucket->records)
//        {
//            if (fhash(get<0>(r), i) == splitBucketNum) {
//                leftover.push_back(r);
//            } else if (fhash(get<0>(r), i) == (n - 1)) {
//                newBucket->records.push_back(r);
//            } else {
//                throw invalid_argument("These is a key does not belong to any after split!");
//            }
//        }
        // the records in bucket old Bucket are distributed into two buckets, these records stays;
//        oldBucket->records = leftover;
    }
}

void HashTableFromDisk::insertToDiskBlock(const string &key, const std::string& value, unsigned short baseOneIndexToRead, const string &fileName) {

    char buffer[200];
    readFileFromDiskByBlock(fileName, baseOneIndexToRead, 200, buffer);
    unsigned short numOfRecord = *(unsigned short *) &buffer[0];
    unsigned short posOfOverFlowBucket = *(unsigned short *) &buffer[2];
    if (numOfRecord < 10) {
        //we have enough room to insert this new record
        unsigned short idxToInsert = 4 + numOfRecord * 13;
        numOfRecord++;
        memcpy(buffer, &numOfRecord, 2);
        memcpy(buffer + idxToInsert, key.c_str(), 9);
        memcpy(buffer + idxToInsert + 9, value.c_str(), 4);
        // write it back to baseOneIndexToRead
        return writefileToDiskByBlock(fileName, baseOneIndexToRead, 200, buffer); // the only way to get out of this loop

    } else if (numOfRecord >= 10 and posOfOverFlowBucket != 0) {
        // if it has an overflow bucket, and currtly is full, insert into it;
        return insertToDiskBlock(key, value, posOfOverFlowBucket, fileName);
    }else{
        // if there is no overflow bucket setted, but it is already full now, set the overflow bucket first;
        memcpy(buffer+2, &this->overflowShift, 2);
        overflowShift++;
        // write it back to baseOneIndexToRead and recursivly call
        writefileToDiskByBlock(fileName, baseOneIndexToRead, 200, buffer);
        return insertToDiskBlock(key, value, overflowShift-1, fileName);
    }

}

