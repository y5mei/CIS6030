//
// Created by Yao-Wen Mei on 2022/10/4.
//

#ifndef QUESTION2_HASHTABLEBLOCK_H
#define QUESTION2_HASHTABLEBLOCK_H

#include <vector>
#include <string>
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
    static std::string searchForKeyWithHashedKey(const string &key, unsigned short hashedKey, unsigned short (*fhash)(const std::string &hashkey, short hashi),
                             const string &fileName = "linked_hashtable.txt",int diskReadCnter = 0 );

    void insert(const std::string &key, std::string value, unsigned short (*fhash)(const std::string &key, short i), const string &fileName = "linked_hashtable.txt");

    void writeVectorOfValuesToBuckets(vector<string>* vec, unsigned short bucketNumStartWritting, const string &fileName = "linked_hashtable.txt");

    void insertToDiskBlock(const std::string &key, const std::string& value, unsigned short baseOneIndexToRead, const string &fileName = "linked_hashtable.txt");
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

void insertDataBase(string record_str, string databaseFileName="database_file.txt", string btreeFileName="linked_hashtable.txt");
#endif //QUESTION2_HASHTABLEBLOCK_H
