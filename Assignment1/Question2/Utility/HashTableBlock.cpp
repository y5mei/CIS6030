//
// Created by Yao-Wen Mei on 2022/10/4.
//

#include "HashTableBlock.h"
#include "BytesOperation.h"
#include <iostream>
using namespace std;

void HashTable::insert(string key, string value) {
    // find out which block to put the record
    unsigned short bucketKey = getHashmapKeyForTest(key, i);
    cout<<"The bucketKey for "<<key << " is "<<bucketKey<<endl;
    // put it there
    if(buckets.size()<=bucketKey){
        unsigned modifiedBucketKey = getBlockNumNeedToSplit(bucketKey);
        buckets.at(modifiedBucketKey).insertToBlock(key, value);
    }else{
        buckets.at(bucketKey).insertToBlock(key, value);
    }
    r++;
    double currThreshold = (r+0.0)/(n+0.0);
    cout<<" The curren threshold is r/n: "<<currThreshold<<endl;
    if(currThreshold > threshold){
        cout<<" the threshold is overflow, need to create new bucket"<<endl;
        this->buckets.emplace_back();
        n++;
        // check to see if we need to increase i now
        unsigned short limit = 1<<i;
        cout<<" n > 2^^i: "<< n <<" >?" << limit <<endl;
        if(n > limit){
            i++; // when n exceeds 2^i, increase i by one;
        }
        // check to see if we need to split now;
        coutShortToBits(n);
        cout<<" the bucket with this num has been added"<<endl;
        unsigned short splitBucketNum = getBlockNumNeedToSplit(n-1);
        coutShortToBits(splitBucketNum);
        cout<<" need to be splited"<<endl;
        auto* oldBucket = &(this->buckets.at(splitBucketNum));
        auto* newBucket = &(this->buckets.at(n-1));
        vector<tuple<string, string>> leftover;
        for(tuple<string, string> r: oldBucket->records){

            coutShortToBits(getHashmapKeyForTest(get<0>(r), i));
            coutShortToBits(splitBucketNum);
            if(getHashmapKeyForTest(get<0>(r), i)==splitBucketNum){
                leftover.push_back(r);
            }else{
                cout<<"There are keys shifted: "<<get<0>(r)<<endl;
                newBucket->records.push_back(r);
            }
        }
        // the records in bucket old Bucket are distributed into two buckets, these records stays;
        oldBucket->records = leftover;


    }

}

HashTable::HashTable(unsigned short i, unsigned short n, unsigned short r){
    this->i = i;
    this->n = n;
    this->r = r;
    this->buckets.emplace_back();
    this->buckets.emplace_back();
}

void HashTableBlock::insertToBlock(std::string key, std::string location) {
    this->records.push_back(make_tuple(key, location));
}
