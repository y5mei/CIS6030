#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm> // sort
#include <bitset>
#include <sstream>

#include "DatabaseFileIO.h"
#include "BPlusTree.h"
#include "RawDataParse.h"
#include "FinalPrintOut.h"


using namespace std;

void printHelpInfo() {
    string s = "";
    s += "\n============================ Help Information  =====================================\n";
    s += " This is the program for CIS6030 Assignment-1 Question-1\n";
    s += " 1. ) For part ONE of this question, Please type `b` to BUILD a database file and a b+tree file on the hard disk.\n";
    s += " You can re-run this build command many times as you want, and the new generated files will overwrite the old ones.\n";
    s += " 2.) For part TWO of this Question, Please type `s` to search, `i` to insert, and `d` to delete a record.\n";
    s += "    * for `s` search, after press `s`, and enter, you will be promoted to input a string of length 9 (field1);\n";
    s += "    * for `i` insert, after press `i`, and enter, you will be promoted to input a string of a full record;\n";
    s += "        - the record you input must include field 1, field 2 and field 3. The field 1 must be diff than any existing keys.\n";
    s += "    * for `d` delete, after press `d`, and enter, you will be promoted to input a string of length 9 (field1);\n";
    cout << s << endl;
}

//void searchBTree(){
//    cout<<" Please type the key (9 chars) that you want to search in the database file:\n"<<endl;
//    cout<<">> ";
//    string key;
//    cin >> key;
//    search(key);
//}

//void buildBTree(){
//    cout<<"lallala"<<endl;
//    readRawDataAndGenerateDataBaseFile("../../A1_data.txt");
//}

int main() {
    cout << "Welcome to CIS6030 Assignment-1 Question-1 (B+Tree)" << endl;\
    bool isQuit = false;
    string command;
    while (!isQuit) {
        cout << "=====================================================================================" << endl;
        cout << "Please input your command, type h for help, type q for quit" << endl;
        cout << "(Input Command)>> ";
        cin.clear(); // some stupid code to clear the cin buffer to avoid left over \n;
        cin.sync();
        getline(cin >> ws, command);

        if (command.length()!=1){
            cout<<" Error: the input command must be length of ONE char, but \""<<command<<"\" was given!, try again!"<<endl;
            continue;
        }
        if (command == "q") {
            isQuit = true;
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
                if (rawDataFilePath == "") {
                    readRawFile();
                } else {
                    readRawFile(rawDataFilePath);
                }
            } catch (invalid_argument e) {
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
                searchDataBase(searchKey);
            } catch (invalid_argument e) {
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
            } catch (invalid_argument e) {
                cout<<"Error: Insert FAIL, please read the error message below:"<<endl;
                cout << e.what() << endl;
            }
            continue;
        }
        cout<<"\nError: the command: "<<command <<" is not a valid command, please type h for more help."<<endl;
    }
    cout << "Question-1 Program has been terminated.";


}