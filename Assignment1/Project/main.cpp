#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm> // sort
#include <bitset>
#include "DatabaseFileIO.h"

using namespace std;

// read a file and get vector of sorted records
vector<Record> readRawTxtFile(string fileName = "../A1_data.txt") {
    string inputText;
    // Read from the text file
    ifstream MyReadFile(fileName);
    int cnt = 0;
//    int maxLine2Read = 30;
    vector<Record> records;
    while (getline(MyReadFile, inputText)) {
        // Output the text from the file
        records.push_back(Record(inputText));
//        Record &currentRecord = records.back();
        cnt++;
    }
    return records;
}

int main() {
    std::cout << "Hello, CIS6030!" << std::endl;
    // write to a file is easy:
    // Create a text file
    ofstream MyWriteFile("filename.txt");
    // Write to the file
    MyWriteFile << "1234567890";
    // Close the file
    MyWriteFile.close();

    ifstream in_file("filename.txt", ios::binary);
    in_file.seekg(0, ios::end);
    int file_size = in_file.tellg();
    cout << "Size of the file is" << " " << file_size << " " << "bytes" << endl;

//    sort the input file based on Field1
    vector<Record> records = readRawTxtFile();
    sort(records.begin(), records.end(),         // Lambda expression begins
         [](Record a, Record b) {
             return (a.field1 < b.field1);
         });

    // let's load all the records into the block linked list:
    BlockListNode* b = new BlockListNode();
    BlockListNode* dummy = b;

    for(auto r: records){
        // change this to string buffer and have a separate method cast short to 2 bytes string;
        string recordStr = to_string(r.idxField2End) + r.field1 + r.field2 + r.field3;
        b = b->insertRecordStringToBlock(recordStr);
    }

    cout<<  "The first record is: "<< dummy->getRecordAsString(dummy->getNumOfRecord()) << endl;
    cout<<  "The last record is: "<< b->getRecordAsString(b->getNumOfRecord()) << endl;

    //TODO: construct a B+Tree from the database file?
    cout<<records.size()<<endl;

    // Let's count how many empty sapces are there in the database file.
    int cnt = 0;
    float emptyBytes = 0;
    while (dummy != nullptr){
        cnt ++;
        emptyBytes += dummy->sizeOfEmptyBytes();
        dummy = dummy->next;
    }
    cout<< "There are totally " << cnt << " 1K blocks to store all the txt file"<<endl;
    float percentage = (emptyBytes)/(1024*cnt)*100;
    cout<< "The percentage of empty bytes is: "<<percentage<<"%" << endl;
    return 0;
}