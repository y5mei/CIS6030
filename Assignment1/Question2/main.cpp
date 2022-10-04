#include <iostream>
#include <string>
#include "Utility/BytesOperation.h"
#include "Utility/HashTableBlock.h"
#include "Utility/Record.h"
#include "FileParse/ReadRawFile.h"


int main() {
    // read all the records and save to database file;
//    std::cout << "Hello, World!" << std::endl;
//    coutShortToBits(154);
//    vector<Record> records = readRawTxtFile();
//
//    // design a hash function, and take mode of 701;
//    short summary[702] = {0};
//    cout<<records.size()<<endl;
//    unsigned short min = 10000;
//    unsigned short max = 0;
//    unsigned int sum = 0;
//
//    for(Record r: records){
//        unsigned short binNum = convertStringToShort(r.field1);
//        summary[binNum] +=1;
//    }
//    for (int i = 0; i < 701; ++i) {
//        sum = sum + summary[i];
//        if(summary[i]<min){
//            min = summary[i];
//        }
//
//        if(summary[i]>max){
//            max = summary[i];
//        }
//
//    }
//
//    cout<<"After hash field1 to long and take mode of 701, the max bin size is "<<max<<", and the min bin size is "<<min<<endl;
//    cout<<"There are totally "<<sum<<" records. Ideally, each bin should have "<<sum/701<<" records. "<<endl;

    // able to get the right most i bits as an unsigned short;
//    unsigned short a = 700;
//    coutShortToBits(a);
//
//    for (int i = 0; i < 11; ++i) {
//        unsigned n = getRightMostIbits(a, i);
//        coutShortToBits(n);
//    }
//    // build a linear hashmap class, start from i = 1, n = ?, ratioal = ?, build in RAM;
//    unsigned short xxx =2;
//    coutShortToBits(xxx);
//
//    unsigned short yyy= getBlockNumNeedToSplit(xxx);
//    coutShortToBits(yyy);

    HashTable ht = HashTable();
    ht.insert("0000","0000");
    ht.insert("1010","1010");
    ht.insert("1111","1111");
    ht.insert("0101","0101");
    ht.insert("0001","0001");
    cout<<ht.i<<" "<<ht.n<<" "<<ht.r<<endl;
    ht.insert("0111","0111");
    cout<<ht.i<<" "<<ht.n<<" "<<ht.r<<endl;
    cout<<"==================="<<endl;
    for(auto r: ht.buckets.front().records){
        cout<<get<0>(r)<<endl;
    }
    cout<<"==================="<<endl;
    for(auto r: ht.buckets.at(1).records){
        cout<<get<0>(r)<<endl;
    }
    cout<<"==================="<<endl;
    for(auto r: ht.buckets.at(2).records){
        cout<<get<0>(r)<<endl;
    }
    cout<<"==================="<<endl;
    for(auto r: ht.buckets.at(3).records){
        cout<<get<0>(r)<<endl;
    }



}
