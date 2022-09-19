#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm> // sort
#include <bitset>
#include "DatabaseFileIO.h"

using namespace std;

struct record {
    string field1;
    string field2;
    string field3;
    short idxField2End;
};

// returns the end idx of the second field (the space),
// the start idx of the third field will be idx + 1
// Example: a b c def, this method will return 5
int endOfField2(string str) {
    int cnt = 3;
    for (int i = 0; i < str.size(); i++) {
        if (isspace(str[i])) {
            cnt -= 1;
            if (cnt == 0) return i;
        }
    }
    return -1;
}


// read a file and get vector of sorted records
vector<record> readRawTxtFile(string fileName = "../A1_data.txt") {
    string inputText;
    // Read from the text file
    ifstream MyReadFile(fileName);
    int cnt = 0;
//    int maxLine2Read = 30;
    vector<record> records;
    while (getline(MyReadFile, inputText)) {
        // Output the text from the file
        records.push_back(record());
        record &currentRecord = records.back();
        currentRecord.field1 = inputText.substr(0, 9);
        currentRecord.idxField2End = endOfField2(inputText);
        currentRecord.field2 = inputText.substr(10, currentRecord.idxField2End - 10);
        currentRecord.field3 = inputText.substr(currentRecord.idxField2End + 1);
        cnt++;
    }
    return records;
}

void printVectorOfRecord(vector<record> v) {
    // Print out what we have in vectors:
    for (auto r: v) {
        cout << r.field1 << "___";
        cout << r.field2 << "___";
        cout << r.field3 << endl;
    }
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
    vector<record> records = readRawTxtFile();
    sort(records.begin(), records.end(),         // Lambda expression begins
         [](record a, record b) {
             return (a.field1 < b.field1);
         } // end of lambda expression
    );
//    printVectorOfRecord(records);
    cout << records.back().field1 << endl;

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