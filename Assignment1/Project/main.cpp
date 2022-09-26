#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm> // sort
#include <bitset>
#include "DatabaseFileIO.h"
#include "BPlusTree.h"

using namespace std;

int main() {
//    sort the input file based on Field1
    vector<Record> records = readRawTxtFile();
    sort(records.begin(), records.end(), [](Record a, Record b) { return (a.field1 < b.field1); });

    // let's load all the records into the block linked list:
    BlockListNode *b = new BlockListNode();
    BlockListNode *dummyHead = b;

    for (auto r: records) {
        b = b->insertRecordStringToBlock(r.content);
    }

    cout << "The first record is: " << dummyHead->getRecordAsString(1) << endl;
    cout << "Block 1 has totally # of records: " << dummyHead->getNumOfRecord() << endl;
    cout << "The second record in block 2 is: " << dummyHead->next->getRecordAsString(2) << endl;
    cout << "Block 2 has totally # of records: " << dummyHead->next->getNumOfRecord() << endl;
    cout << "The 4th record in block 4 is: " << dummyHead->next->next->next->getRecordAsString(4) << endl;
    cout << "Block 4has totally # of records: " << dummyHead->next->next->next->getNumOfRecord() << endl;
    cout << "The last record is: " << b->getRecordAsString(b->getNumOfRecord()) << endl;

    //TODO: construct a B+Tree from the database file?
    cout << records.size() << endl;
    //1) Build a database file and save to disk; block size 1024;
    BlockListNode::saveToDisk(dummyHead, "database_file.txt");
    //2.1) Build a B+Tree in RAM,
    b = dummyHead;
    BPlusTree<string> bTree = BPlusTree<string>(8);

    int block_cnt = 0;
    while(b != nullptr){
        block_cnt ++;
        for (int i = 0; i < b->getNumOfRecord(); ++i) {
            Record r = Record(b->getRecordAsString(i+1));
            string value = "Block#: "+to_string(block_cnt)+" Record#: "+ to_string(i+1); //1 based num
            string key = r.field1;
            bTree.insert(key, value);
        }
        b = b->next;
    }

    // Record/Block index can be obtained from the bTree
    // Example: aaagbmhha(1, 1), aaieqtkeh(2, 2), zzzjzagrk(3365, last one)
    cout<<"=============== Reading Results From B+Tree ====================="<<endl;
    cout<<"aaagbmhha is: " <<bTree.search("aaagbmhha")<<endl;
    cout<<"aaieqtkeh is: " <<bTree.search("aaieqtkeh")<<endl;
    cout<<"zzzjzagrk is: " <<bTree.search("zzzjzagrk")<<endl;
    cout<<"=============== Reading Results From B+Tree ====================="<<endl;

    //2.2) save the B+Tree to disk;
    //3.1) Be above to build a database blocks object in RAM from the disk file;
    BlockListNode* readBackHead = readFileFromHardDisk("database_file.txt", 1024);
    readBackHead = readBackHead->next->next->next;
    cout<<"Read From Disk for record 4 block 4 is:================"<<endl;
    cout<<readBackHead->getRecordAsString(4)<<endl;


    //3.2) Be above to read only one block form disk:
//    BlockListNode* onlyOneBlock1 = readBlockWithLSeek("database_file.txt", 2, 1024);
//    cout<<"================== read only one block from disk=================="<<endl;
//    cout<< onlyOneBlock1->getNumOfRecord()<<endl;
//    cout<< onlyOneBlock1->getRecordAsString(2)<<endl;
    BlockListNode* onlyOneBlock2 = readBlockWithLSeek("database_file.txt", 5, 1024);
    cout<<"================== read only one block from disk=================="<<endl;
    cout<< onlyOneBlock2->getNumOfRecord()<<endl;
    cout<< onlyOneBlock2->getRecordAsString(5)<<endl;
//    BlockListNode* onlyOneBlock4 = readBlockWithLSeek("database_file.txt", 4, 1024);
//    cout<<"================== read only one block from disk=================="<<endl;
//    cout<< onlyOneBlock4->getNumOfRecord()<<endl;
//    cout<< onlyOneBlock4->getRecordAsString(4)<<endl;
    //4) Be above to build a B+Tree object in RAM from the disk file;

    // Let's count how many empty space are there in the database file.
    int cnt = 0;
    float emptyBytes = 0;
    while (dummyHead != nullptr) {
        cnt++;
        emptyBytes += dummyHead->getEmptyBytes();
        dummyHead = dummyHead->next;
    }
    cout << "There are totally " << cnt << " 1K blocks to store all the txt file" << endl;
    float percentage = (emptyBytes) / (1024 * cnt) * 100;
    cout << "The percentage of empty bytes is: " << percentage << "%" << endl;
    return 0;
}