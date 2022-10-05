//
// Created by Yao-Wen Mei on 2022/10/4.
//

#ifndef QUESTION2_HASHTABLEBLOCK_H
#define QUESTION2_HASHTABLEBLOCK_H

#include <vector>
#include <string>
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
    double threshold = 1.7;

    explicit HashTable(unsigned short i = 1, unsigned short n = 2, unsigned short r = 0) : i(i), n(n), r(r) {
        this->buckets.emplace_back();
        this->buckets.emplace_back();
    }

    void insert(std::string key, std::string value, unsigned short (*fhash)(const std::string &key, short i));

    friend ostream &operator<<(ostream &os, HashTable const &ht) {
        string s;
        s.append("i value is (valid right most bits): ");
        s.append(to_string(ht.i)+'\n');
        s.append("n value is (number of buckets): ");
        s.append(to_string(ht.n)+'\n');
        s.append("r value is (number of records)");
        s.append(to_string(ht.r)+'\n');
        return os << s;
    }
};

#endif //QUESTION2_HASHTABLEBLOCK_H
