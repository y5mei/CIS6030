# B+Tree
Yaowen Mei (1177855) 2023 Fall CIS6060

1. An implementation of a B+Tree is under the BPlusTree_lib folder, this is a 9 way tree
   1. All leaf nodes are on the same level;
   2. For leaf nodes, there are at most 8 keys and at least 4 keys ([4, 8] keys on leaf nodes);
   3. For interior nodes, there are at most 9 pointers and at least 5 pointers. ([5, 9] pointers on interior nodes);
   4. The number of keys are always one less than the number of pointers.

## To Run this Program:
1. unzip the zip file in a directory of CoCS;
2. make a build directory: `mkdir build`
3. cd to this build directory `cd build`
4. cmake the source files `cmake ..`
5. make the source files `make`
6. To start running this program: `./Database_run`
7. After the program is running, you can type `h` to see command help messages. Briefly,you have to:
   * type `b` first to build the `databas_file` and `bplustree_file` on disk;
   * <font  color=red> YOU MUST RUN `b` FIRST BEFORE YOU CAN INSERT, DELETE AND SEARCH</font>
   * type `i`, `d`, `s`, `rk`, `ri`, `a` for insert, delete, search and range_search_with_keys, range_search_with_idx_nums, and display all records in order in terminal;
   * you can always reset the database file and btree file by running `b` again.
     (this will reset all the insert, delete you made, and reset all the disk files to the very begining.)
8. After insert, delete, or build, a txt file, `field_one_file.txt` will be generated, this file contains all the field1s in order for verification purpose.
9. For range search with keys, if you input keys that are not exist in the btree, this program will output a subset of the existing records that belongs to the range you input.
10. For range search with idx nums, you have to make sure the idx are valid, I didn't perform any validation on the num.

## Unit Test:
Google Test framework is used for unit test purpose [Link](https://www.jetbrains.com/help/clion/creating-google-test-run-debug-configuration-for-test.html#code-gen-menu)
Unit test can be found and run by run the main method in `./Google_tests/TestMain.cpp` file.
