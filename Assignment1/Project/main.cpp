#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm> // sort
#include <bitset>
#include <sstream>
#include "BPlusTree.h"
#include "FinalPrintOut.h"


using namespace std;

void printHelpInfo() {
    string s = "";
    s += "\n============================>>> Help Information  <<<=====================================\n";
    s += " This is the program for CIS6030 Assignment-1 Question-1\n";
    s += " 1. ) For part ONE of this question, Please type `b` to BUILD a database file and a b+tree file on the hard disk.\n";
    s += " You can re-run this build command many times as you want, and the new generated files will overwrite the old ones.\n";
    s += " 2.) For part TWO of this Question:"
         " Please type `s` to search,\n"
         "            `i` to insert, \n"
         "            `d` to delete a record,\n"
         "            `rk` to do Range search with Keys,\n "
         "            `ri`to do Range search with Index numbers (0-idxed nums),\n"
         "            `a`to display all the records sequentially ordered by the first field.\n";
    s += " Additionally, All the field ones are saved as a text file,  field_one_file.txt, for verification purpose.\n"
         " and this file got updated every time after insert/delete, for verification purpose.\n";
    s += "\n============================<<< Command Details  >>>=====================================\n";
    s += "    * for `s` search, after press `s`, and enter, you will be promoted to input a string of length 9 (field1);\n";
    s += "    * for `i` insert, after press `i`, and enter, you will be promoted to input a string of a full record;\n";
    s += "        - the record you input must include field 1, field 2 and field 3. The field 1 must be diff than any existing keys.\n";
    s += "    * for `d` delete, after press `d`, and enter, you will be promoted to input a string of length 9 (field1);\n";
    s += "    * for `rk` range search with Key Strings, after press `r` and enter, you will be promoted to input 9 chars for the start key,\n"
         "      then you will be promoted to input another 9 chars for the end key, all the records within this range will be displayed.\n";
    s += "    * for `ri` range search via Index Num, after press `ri` and enter, you will be promoted to input a zero based int number\n"
         "       for the start idx, then you will be promoted to input another zero based int number for the end idx, all the records\n"
         "       within this range will be displayed.\n";
    s += "    * for `a` dispaly all records in terminal ordered by field one, if you only want to check field ones, you can verify with\n"
         "      the auto saved txt file,  field_one_file.txt.\n";
    cout << s << endl;
}

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
        cout << "---------------- Running command:------------------------" << endl;
//        if (command.length()!=1){
//            cout<<" Error: the input command must be length of ONE char, but \""<<command<<"\" was given!, try again!"<<endl;
//            continue;
//        }
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
                searchDataBase(searchKey);
            } catch (invalid_argument const &e) {
                cout << e.what() << endl;
            }
            continue;
        }
        if (command == "rk") {
            cout << "Please input the staring keys (9 bytes) for the range you want to search:" << endl;
            cout << "(Start Range Search Key)>> ";
            string searchKey1;
            cin >> searchKey1;
            cout << "Please input the ending keys (9 bytes) for the range you want to search:" << endl;
            cout << "(End Range Search Key)>> ";
            string searchKey2;
            cin >> searchKey2;
            try {
                rangeSearch(searchKey1, searchKey2);
            } catch (invalid_argument const &e) {
                cout << e.what() << endl;
            }
            continue;
        }
        if (command == "ri") {
            cout << "Please input the staring idx (int number) for the range you want to start the search:" << endl;
            cout << "(Start Range index [0 indexed int number])>> ";
            int searchKey1;
            cin >> searchKey1;
            cout << "Please input the ending keys (int number) for the range you want to end the search:" << endl;
            cout << "(End Range Search Key [0 indexed int number])>> ";
            int searchKey2;
            cin >> searchKey2;
            try {
                rangeSearchWithIdx(searchKey1, searchKey2);
            } catch (invalid_argument const &e) {
                cout << e.what() << endl;
            }
            continue;
        }
        if (command == "h") {
            printHelpInfo();
            continue;
        }
        if (command == "a") {
            displayAllRecords();
            continue;
        }
        if (command == "i") {
            cout << "Please input the full record (field1 field2 field3) you want to insert:"
                 << endl;
            cout << "(Insert Record)>> ";
            // get the user input file path, and assign to rawDataFilePath variable
            string record;
            getline(cin >> ws, record);
            try {
                insertDataBase(record);
            } catch (invalid_argument const &e) {
                cout << "Error: Insert FAIL, please read the error message below:" << endl;
                cout << e.what() << endl;
            }
            continue;
        }
        if (command == "d") {
            cout << "Please input the 9 chars field1 that you want to delete:"
                 << endl;
            cout << "(Delete Record)>> ";
            // get the user input file path, and assign to rawDataFilePath variable
            string record;
            getline(cin >> ws, record);
            try {
                deleteDataBase(record);
            } catch (invalid_argument const &e) {
                cout << "Error: Delete FAIL, please read the error message below:" << endl;
                cout << e.what() << endl;
            }
            continue;
        }
        cout << "\nError: the command: " << command << " is not a valid command, please type h for more help." << endl;
    }
    cout << "Question-1 Program has been terminated.\n";
}