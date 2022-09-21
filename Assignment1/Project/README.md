# Implement Progress Steps

* Read the A1_data.txt file, read by line and construct the record objects
* Sort these records
* Construct Blocks from these sorted records (the blocks should be linked list)
* Save the blocks to a file in hard disk

## B+Tree

1. An implementation of a B+Tree is under the BPlusTree_lib folder, this is a 9 way tree
   1. All leaf nodes are on the same level;
   2. For leaf nodes, there are at most 8 keys and at least 4 keys ([4, 8] keys on leaf nodes);
   3. For interior nodes, there are at most 9 pointers and at least 5 pointers. ([5, 9] pointers on interior nodes);
   4. The number of keys are always one less than the number of pointers.

## Unit Test:

Google Test framework is used for unit test purpose [Link](https://www.jetbrains.com/help/clion/creating-google-test-run-debug-configuration-for-test.html#code-gen-menu)

## Linux Command:
1. To delete a folder: `rm -r folder/`