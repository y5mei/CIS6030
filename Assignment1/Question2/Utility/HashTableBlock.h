//
// Created by Yao-Wen Mei on 2022/10/4.
//

#ifndef QUESTION2_HASHTABLEBLOCK_H
#define QUESTION2_HASHTABLEBLOCK_H

#include <vector>
#include <string>
#include <cstring>
#include <algorithm>    // std::max
#include <tuple> // Required for std::tuple
#include "BytesOperation.h"

using namespace std;

class HashTableBlock {
public:
    std::vector<std::tuple<std::string, std::string>> records;

    void insertToBlock(std::string key, std::string location) {
        this->records.emplace_back(key, location);
    }

};

class HashTable {
public:
    std::vector<HashTableBlock> buckets;
    unsigned short i;
    unsigned short n;
    unsigned short r;
    double threshold = 5; // on average, 5 records per bucket, actually, max is 17, min is one, hash is not very uniform with mod 60K prime;

    explicit HashTable(unsigned short i = 1, unsigned short n = 2, unsigned short r = 0) : i(i), n(n), r(r) {
        this->buckets.emplace_back();
        this->buckets.emplace_back();
    }

    void insert(const std::string &key, std::string value, unsigned short (*fhash)(const std::string &key, short i));

    std::string search(const std::string &key, unsigned short (*fhash)(const std::string &hashkey, short hashi));

    void saveToDisk(const string &fileName = "linked_hashtable.txt");

    friend ostream &operator<<(ostream &os, HashTable const &ht) {
        // verify this hashtable works, then develop a write to disk method for it!
        int maxBucketSize = 0;
        int minBucketSize = 10000;
        int numOfEmptyBuckets = 0;
        for (const auto &b: ht.buckets) {
            int curr = b.records.size();
            maxBucketSize = max(curr, maxBucketSize);
            if (curr != 0) {
                minBucketSize = min(curr, minBucketSize);
            } else {
                numOfEmptyBuckets++;
            }
        }

        string s;
        s.append("=====================================================================================\n");
        s.append("====================== Summary of the Generated Hash Table ==========================\n");
        s.append("=====================================================================================\n");
        s.append("i value (valid right most bits) is : ");
        s.append(to_string(ht.i) + '\n');
        s.append("n value (number of buckets) is : ");
        s.append(to_string(ht.n) + '\n');
        s.append("r value (number of records) is : ");
        s.append(to_string(ht.r) + '\n');
        s.append("The Largest bucket contains " + to_string(maxBucketSize) +
                 " records in it and it overflow buckets (if any).\n");
        s.append("The Smallest Non Empty bucket contains " + to_string(minBucketSize) + " records in it.\n");
        s.append("Additionally, there are " + to_string(numOfEmptyBuckets) +
                 " empty buckets in the linear hash table.\n");
        return os << s;
    }
};

class HashTableFromDisk {
public:
    unsigned short i;
    unsigned short n;
    unsigned short r;
    double threshold = 5; // on average, 5 records per bucket, actually, max is 17, min is one, hash is not very uniform with mod 60K prime;
    unsigned short overflowShift;

    explicit HashTableFromDisk(const string &fileName = "linked_hashtable.txt");

    std::string searchForKey(const string &key, unsigned short (*fhash)(const std::string &hashkey, short hashi),
                             const string &fileName = "linked_hashtable.txt");

    static std::string searchForKeyWithHashedKey(const string &key, unsigned short hashedKey,
                                                 unsigned short (*fhash)(const std::string &hashkey, short hashi),
                                                 const string &fileName = "linked_hashtable.txt",
                                                 int diskReadCnter = 0);

    void insert(const std::string &key, std::string value, unsigned short (*fhash)(const std::string &key, short i),
                const string &fileName = "linked_hashtable.txt");

    // start from a known existing block in a fileName, writes all the vec content into the block, if the block is full, try to write to its overflow buckets, if there is no overflow buckets,
    // this method will create overflow buckets for it, and update the HashTable header block;
    void writeVectorOfValuesToBuckets(vector<string> *vec, unsigned short bucketNumStartWritting,
                                      const string &fileName = "linked_hashtable.txt");

    void insertToDiskBlock(const std::string &key, const std::string &value, unsigned short baseOneIndexToRead,
                           const string &fileName = "linked_hashtable.txt");

    friend ostream &operator<<(ostream &os, HashTableFromDisk const &ht) {
        string s;
        s.append("=====================================================================================\n");
        s.append("=========== Summary of the Generated Hash Table From Disk  ==========================\n");
        s.append("=====================================================================================\n");
        s.append("i value (valid right most bits) is : ");
        s.append(to_string(ht.i) + '\n');
        s.append("n value (number of buckets) is : ");
        s.append(to_string(ht.n) + '\n');
        s.append("r value (number of records) is : ");
        s.append(to_string(ht.r) + '\n');
        s.append("last overflow bucket should be inserted to block# : ");
        s.append(to_string(ht.overflowShift) + '\n');
        return os << s;
    }

};

void writefileToDiskByBlock(const string &fileName, int blockNum, int blockSize, char *content);

string readFileFromDiskByBlockReturnString(string filename, int blockNum, int blockSize);

void readFileFromDiskByBlock(const string &fileName, int blockNum, int blockSize, char *content);

unsigned short getNumOfBlocksFromHardDiskFile(const std::string &filename, int blockSize = 200);

void insertDataBase(string record_str, string databaseFileName = "database_file.txt",
                    string btreeFileName = "linked_hashtable.txt");

// A thin wrapper of a block of hashtable object presented as char[200] in RAM;
class HashTableBucketInRam {
public:
    char *content;
    unsigned short numRecords;
    unsigned short overFlowBlock;

    HashTableBucketInRam(char *buffer) {
        content = buffer;
        this->numRecords = *(unsigned short *) &buffer[0];
        this->overFlowBlock = *(unsigned short *) &buffer[2];
    }

    HashTableBucketInRam(char *buffer, unsigned short blockNumGetDirectFromHash) {
        readFileFromDiskByBlock("linked_hashtable.txt", blockNumGetDirectFromHash + 2, 200, buffer);
        content = buffer;
        this->numRecords = *(unsigned short *) &buffer[0];
        this->overFlowBlock = *(unsigned short *) &buffer[2];
    }

    // one based idx to get a record out as string, this string is actually 14 char long, need to get rid of the last char later;
    string getRecord(int i) {
//        if(i > numRecords) return "Record index overflow";
        int idx = 4 + (i - 1) * 13;
        char result[13];
        memcpy(result, content + idx, sizeof(result) / sizeof(result[0]));
        string r = result;
//        r.pop_back(); // remove the \0 at the end
        return r;
    }

    void setNumRecords(unsigned short newNum) {
        this->numRecords = newNum;
        memcpy(content, &numRecords, 2);
    }

    void setOverFlowBlock(unsigned short newOverflow) {
        this->overFlowBlock = newOverflow;
        memcpy(content + 2, &overFlowBlock, 2);
    }

    void printAllRecords() {
        for (unsigned short i = 1; i <= numRecords; ++i) {
            cout << getRecord(i) << endl;
        }
    }

    void insertRecordAtIdx(unsigned short idx, string str) {
        int cursor = 4 + (idx - 1) * 13;
        for (int i = 0; i < 13; ++i) {
            content[cursor + i] = str[i];
        }
    }

    void insert(string str) {
        if (numRecords == 10) {
            throw invalid_argument(
                    "Error: This bucket is currently full already. Cannot insert any more records into it.");
        }
        unsigned short newRecordIdx = numRecords + 1;
        setNumRecords(newRecordIdx);
        insertRecordAtIdx(newRecordIdx, str);
    }

    vector<string> getAllRecordsIncludingOverFlowBuckets(){
        vector<string> allRecords;
        for (unsigned short i = 1; i <= numRecords; ++i) {
            allRecords.push_back(getRecord(i));
        }
        unsigned short next = overFlowBlock;
        // recursively visit all the overflow blocks, and collect all the records.
        while(next!=0){
            char nextBlock[200];
            HashTableBucketInRam nextBucketObj = HashTableBucketInRam(nextBlock, overFlowBlock);
            cout<<"Visiting overflowblock #: "<<overFlowBlock<<endl;
            next = nextBucketObj.overFlowBlock;
            for(unsigned short j = 1; j<=nextBucketObj.numRecords; ++j){
                allRecords.push_back(nextBucketObj.getRecord(j));
            }
        }
        return allRecords;
    }


};


#endif //QUESTION2_HASHTABLEBLOCK_H
