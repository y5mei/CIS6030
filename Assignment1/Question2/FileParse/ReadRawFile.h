//
// Created by Yao-Wen Mei on 2022/10/3.
//

#ifndef QUESTION2_READRAWFILE_H
#define QUESTION2_READRAWFILE_H

#include <string>
#include "../Utility/Record.h"

using namespace std;
class ReadRawFile {

};
void readRawFile(const string& fileName = "../A1_data.txt");
vector<Record> readRawTxtFile(const string& fileName = "../A1_data.txt");
void readDataFromDatabaseFileViaLocationString(string location);
#endif //QUESTION2_READRAWFILE_H
