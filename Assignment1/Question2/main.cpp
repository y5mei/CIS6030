#include <iostream>
#include <string>
#include <tuple>
#include <sstream>
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

void generateSomeRandomRecordsAndInsert(){
    vector<Record> records = readRawTxtFile();
    for (int i = 0; i < 100; ++i) {
        string newContent = records.at(i).content;
        newContent[0] = '1';
        Record newRecord = Record(newContent);
        cout<<newRecord.field1<<endl;
        insertDataBase(newRecord.content);
    }
    HashTableFromDisk htDisk = HashTableFromDisk();
    for (int i = 0; i < 100; ++i) {
        string newContent = records.at(i).content;
        newContent[0] = '1';
        Record r = Record(newContent);
        string location = htDisk.searchForKeyWithoutPrint(r.field1, getHashmapKey,"linked_hashtable.txt");
        if(location=="-1"){
            cout<<r.field1<<" can not be found" <<endl;
            return;
        }
    }
    cout<<"All the records are found...!"<<endl;
}

void testSearchAfterInsert(){
    vector<Record> records = readRawTxtFile();
    HashTableFromDisk htDisk = HashTableFromDisk();
    for(auto r: records){
        string location = htDisk.searchForKeyWithoutPrint(r.field1, getHashmapKey,"linked_hashtable.txt");
        if(location=="-1"){
            cout<<r.field1<<" can not be found" <<endl;
            return;
        }
    }
    cout<<"All the records are found...!"<<endl;
}

void printHelpInfo() {
    string s = "";
    s += "\n============================ Help Information  =====================================\n";
    s += " This is the program for CIS6030 Assignment-1 Question-2\n";
    s += " 1. ) For part ONE of this question, Please type `b` to BUILD a database file and a hashmap file on the hard disk.\n";
    s += " You can re-run this build command many times as you want, and the new generated files will overwrite the old ones.\n";
    s += " 2.) For part TWO of this Question, Please type `s` to search, `i` to insert.\n";
    s += "    * for `s` search, after press `s`, and enter, you will be promoted to input a string of length 9 (field1);\n";
    s += "    * for `i` insert, after press `i`, and enter, you will be promoted to input a string of a full record;\n";
    s += "        - the record you input must include field 1, field 2 and field 3. The field 1 must be diff than any existing keys.\n";
    cout << s << endl;
}

int main() {
// Uncomment here to test with all the records and insert 100 new records
//    generateSomeRandomRecordsAndInsert();
//    testSearchAfterInsert();
//    return 0;

    cout<<"This is a test for bytes"<<endl;
    cout << "Welcome to CIS6030 Assignment-1 Question-2 (Linear Hashmap)" << endl;\
    bool isQuit = false;
    string command;
    while (!isQuit) {
        cout << "=====================================================================================" << endl;
        cout << "Please input your command, type h for help, type q for quit" << endl;
        cout << "(Input Command)>> ";
        cin.clear(); // some stupid code to clear the cin buffer to avoid left over \n;
        cin.sync();
        getline(cin >>ws, command);
        cout<<"---------------- Running command:------------------------"<<endl;
        if (command.length()!=1){
            cout<<" Error: the input command must be length of ONE char, but \""<<command<<"\" was given!, try again!"<<endl;
            continue;
        }
        if (command == "q") {
            isQuit = true;
            continue;
        }
        if (command == "b") {
            cout << "Please input the path to the raw A1_data.txt file (press return will use the default value):"
                 << endl;
            cout << "(Input FilePath)>> ";
            // get the user input file path, and assign to rawDataFilePath variable
            cin.ignore(0, '\n');
            string rawDataFilePath = "";
            getline(cin, rawDataFilePath);
            if (rawDataFilePath.empty()) {
                rawDataFilePath = "";
            } else {
                istringstream istr(rawDataFilePath);
                istr >> rawDataFilePath;
            }
            try {
                if (rawDataFilePath.empty()) {
                    readRawFile();
                } else {
                    readRawFile(rawDataFilePath);
                }
            } catch (invalid_argument const &e) {
                cout << e.what() << endl;
            }
            continue;
        }
        if (command == "s") {
            cout << "Please input the key (field1) you want to search:" << endl;
            cout << "(Search Key)>> ";
            string searchKey;
            cin >> searchKey;
            try {
                HashTableFromDisk htDisk = HashTableFromDisk();
//                cout << htDisk << endl;
                string location = htDisk.searchForKey(searchKey, getHashmapKey,"linked_hashtable.txt");
                if(location != "-1"){
                    StingShort ss = StingShort(location);
                    cout<<"Database file block number: "<<ss.block<<" and record number: "<<ss.record<<endl;
                    readDataFromDatabaseFileViaLocationString(location);
                }

            } catch (invalid_argument const &e) {
                cout << e.what() << endl;
            }
            continue;
        }
        if (command == "h") {
            printHelpInfo();
            continue;
        }

        if (command == "i"){
            cout << "Please input the full record (field1 field2 field3) you want to insert:"
                 << endl;
            cout << "(Insert Record)>> ";
            // get the user input file path, and assign to rawDataFilePath variable
            string record;
            getline(cin>>ws, record);
            try {
                insertDataBase(record);
                cout << ">> Insert successful." << endl;
            } catch (invalid_argument const &e) {
                cout<<"Error: Insert FAIL, please read the error message below:"<<endl;
                cout << e.what() << endl;
            }
            continue;
        }
        cout<<"\nError: the command: "<<command <<" is not a valid command, please type h for more help."<<endl;
    }
    cout << "Question-2 Program has been terminated.\n";
}