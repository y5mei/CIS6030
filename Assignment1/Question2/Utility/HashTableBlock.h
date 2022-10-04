//
// Created by Yao-Wen Mei on 2022/10/4.
//

#ifndef QUESTION2_HASHTABLEBLOCK_H
#define QUESTION2_HASHTABLEBLOCK_H

#include <vector>
#include <string>

class HashTableBlock {
public:
    std::vector<std::tuple<std::string, std::string>> records;
    void insertToBlock(std::string key, std::string location);
};

class HashTable {
public:
    std::vector<HashTableBlock> buckets;
    unsigned short i;
    unsigned short n;
    unsigned short r;
    double threshold = 1.7;

    explicit HashTable(unsigned short i=1, unsigned short n=2, unsigned short r=0);

    void insert(std::string key, std::string value);
};

#endif //QUESTION2_HASHTABLEBLOCK_H
