//
// Created by Yao-Wen Mei on 2022/10/1.
//

#ifndef PROJECT_FINALPRINTOUT_H
#define PROJECT_FINALPRINTOUT_H

double f2();

void readRawFile(std::string fileName = "../A1_data.txt");

// search a key from a BTree, and return and print the result;
string searchDataBase(string key, std::string databaseFileName = "database_file.txt",
                      std::string btreeFileName = "bTree_file.txt");

void
insertDataBase(string key, std::string databaseFileName = "database_file.txt",
               std::string btreeFileName = "bTree_file.txt");

void
deleteDataBase(string key, std::string databaseFileName = "database_file.txt",
               std::string btreeFileName = "bTree_file.txt");
// build a BTree in RAM from disk file
// search for it using the search method, get it directly from hard disk, delete it
// if after delete, the block is empty, then we need to read all the blocks back, and rewrite the tree and database file
// if it is not empty after delete, just simply write the block back;
#endif //PROJECT_FINALPRINTOUT_H
