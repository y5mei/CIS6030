# Implement Progress Steps

* Read the A1_data.txt file, read by line and construct the record objects
* Sort these records
* Construct Blocks from these sorted records (the blocks should be linked list)
* Build a B+Tree via a nested loop (`for each block for each record`)
* Save the B+Tree as a hard disk file;
* Save the blocks as a file in hard disk;

### During insert [Do I need to maintain the order of block?]

#### lazy implement:
* just insert the new record to the tail of the block linked list [unsorted]
* use B+Tree to iter all the record in order, create another in-order block linked list in memory
* save the new block linked list in disk
* generate a new B+ Tree from the new block linked list and save to disk;

#### eager implement
* All the current blocks are in the RAM as a linked list
* B+Tree can help me find the element less than the current key
* Say the previous record is Block 5 record 6;
* Get the block object of the previous key [How? I can loop from the head to get the block];
* try to insert the new record inside [need to shift the existing record, might need to create new block node, and link it together]
* Need to save the updated blocks to hard disk
* Need to save the new B+Tree file to hard disk

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