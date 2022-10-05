#include <iostream>
#include <string>
#include <tuple>
#include <algorithm>    // std::max
#include "Utility/BytesOperation.h"
#include "Utility/HashTableBlock.h"
#include "Utility/Record.h"
#include "FileParse/ReadRawFile.h"

using namespace std;

void buildDatabaseFileAndHashTableOnDisk(){
    // read a file, save all the records to hard disk as a database file, save the block num and record number to Hashmap in ram
    // save the Hashmap from Ram to Disk
    vector<Record> records = readRawTxtFile();
}

int main() {
    // read all the records and save to database file;
    std::cout << "Hello, World!" << std::endl;
    readRawFile(); // read raw file and save two files on disk

    // try to read it back?

    HashTableFromDisk htDisk = HashTableFromDisk();
    cout<<htDisk<<endl;
    htDisk.searchForKey("gpxiqvkuy", getHashmapKey);

//            gpxiqvkuy
//
////    Able to get the right most i bits as an unsigned short;
//    HashTable ht = HashTable();
//    for(Record r: records){
//        ht.insert(r.field1, r.content, getHashmapKey);
//    }
//    cout<<ht<<endl;
//    cout<<ht.buckets.size()<<endl;
//
//    int maxBucketSize = 0;
//    int minBucketSize = 10000;
//    int numOfEmptyBuckets = 0;
//    for(auto b:ht.buckets){
//        int curr = b.records.size();
//        maxBucketSize = max(curr, maxBucketSize);
//        if(curr != 0){
//            minBucketSize = min(curr, minBucketSize);
//        }else{
//            numOfEmptyBuckets++;
//        }
//
//    }
//
//    cout<<maxBucketSize<<endl;
//    cout<<minBucketSize<<endl;
//    cout<<numOfEmptyBuckets<<endl;
//
//    for(Record r: records){
//        ht.search(r.field1,getHashmapKey);
//    }


//    ht.insert("0000","0000", getHashmapKeyForTest);
//    ht.insert("1010","1010",getHashmapKeyForTest);
//    ht.insert("1111","1111",getHashmapKeyForTest);
//    ht.insert("0101","0101",getHashmapKeyForTest);
//    ht.insert("0001","0001",getHashmapKeyForTest);
////    cout<<ht.i<<" "<<ht.n<<" "<<ht.r<<endl;
//    ht.insert("0111","0111",getHashmapKeyForTest);
//    cout<<ht.i<<" "<<ht.n<<" "<<ht.r<<endl;
//    cout<<"==================="<<endl;
//    for(auto r: ht.buckets.front().records){
//        cout<<get<0>(r)<<endl;
//    }
//    cout<<"==================="<<endl;
//    for(auto r: ht.buckets.at(1).records){
//        cout<<get<0>(r)<<endl;
//    }
//    cout<<"==================="<<endl;
//    for(auto r: ht.buckets.at(2).records){
//        cout<<get<0>(r)<<endl;
//    }
//    cout<<"==================="<<endl;
//    for(auto r: ht.buckets.at(3).records){
//        cout<<get<0>(r)<<endl;
//    }
}
