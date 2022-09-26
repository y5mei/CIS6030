#include "gtest/gtest.h"
#include "DatabaseFileIO.h"
#include <vector>
#include <filesystem>


TEST(Database_lib, BlockListNode_Default_Num_0) {
    BlockListNode b;
    EXPECT_EQ(b.getNumOfRecord(), 0);
}

// Block size is 1024 by default;
TEST(Database_lib, Block_Default_Size_is_1K) {
    BlockListNode b;
    EXPECT_EQ(sizeof(b.block), 1024);
    EXPECT_EQ(b.ACTUAL_SIZE, 1024);
}

// Block size can be customized for unit test purpose
TEST(Database_lib, Block_Size_Can_Be_Set_To_30Bytes) {
    BlockListNode b = BlockListNode(30);
    EXPECT_EQ(b.ACTUAL_SIZE, 30);
}

// Can write blocks until it is full.
TEST(Database_lib, Write_A_Block_Until_Full) {
    // set size to be
    // 1 byte for cnt_record
    // 2 byte for record jump, 3 bytes for record x 2
    // put 1 byte extra empty space in middle
    // This block is:
    //  2, 09, 06, \0, a, b, c, 0, 1, 2
    BlockListNode b = BlockListNode(12);
    b.insertRecordStringToBlock("012");
    b.insertRecordStringToBlock("abc");
    EXPECT_EQ(b.getNumOfRecord(), 2);
    EXPECT_EQ(b.getJumpOfRecord(1), 9);
    EXPECT_EQ(b.getJumpOfRecord(2), 6);
    EXPECT_EQ(b.block[11], '2');
    EXPECT_EQ(b.getEmptyBytes(), 1);
    // this tests _that_ the expected exception is thrown
    EXPECT_THROW({
                     try {
                         b.insertRecordStringToBlock("mayson is a pig");
                     }
                     catch (const std::invalid_argument &e) {
                         // and this tests that it has the correct message
                         EXPECT_STREQ("not enough empty space to hold new str, need to create a linked list", e.what());
                         throw;
                     }
                 }, std::invalid_argument);

}

// Can get records back from Blocks as String.
TEST(Database_lib, Get_Records_From_Block_By_Idx) {
    BlockListNode b = BlockListNode(12);
    b.insertRecordStringToBlock("012");
    b.insertRecordStringToBlock("abc");
    ASSERT_EQ(b.getRecordAsString(1),"012");
    ASSERT_EQ(b.getRecordAsString(2),"abc");
}

TEST(Database_lib, Blocks_Are_Like_LinkedList) {
    BlockListNode* b1 = new BlockListNode(30);
    b1->insertRecordStringToBlock("Sequence 1");
    b1->generateNextNode("Sequence 2");
    b1->next->generateNextNode("Sequence 3");

    ASSERT_EQ(b1->getRecordAsString(1),"Sequence 1");
    ASSERT_EQ(b1->next->getRecordAsString(1),"Sequence 2");
    ASSERT_EQ(b1->next->next->getRecordAsString(1),"Sequence 3");

    // cnt # of nodes
    int cnt = 0;
    while (b1 != nullptr){
        cnt+=1;
        ASSERT_EQ(b1->getRecordAsString(1),"Sequence "+std::to_string(cnt));
        b1 = b1->next;
    }
    ASSERT_EQ(cnt, 3);
}

TEST(Database_lib, Generate_Linked_From_A_Block_Of_ContentStr_From_Disk) {
    BlockListNode b = BlockListNode(12);
    b.insertRecordStringToBlock("012");
    b.insertRecordStringToBlock("abc");
    ASSERT_EQ(b.getRecordAsString(1),"012");
    ASSERT_EQ(b.getRecordAsString(2),"abc");
    ASSERT_EQ(b.getEmptyBytes(),1);
    ASSERT_EQ(sizeof(b.block), 1024);
}

// save 3 blocks with 12 bytes block size to hard disk
// and read the whole file back as a linked list of block
// assert the records can be obtained;
// Need to do the same thing when doing insert and delete
TEST(Database_lib, Save_A_Block_To_Disk_And_Read_It_Back) {
    BlockListNode b = BlockListNode(12);
    BlockListNode* curr = &b;
    curr = curr->insertRecordStringToBlock("012");
    curr = curr->insertRecordStringToBlock("abc");
    curr = curr->insertRecordStringToBlock("345");
    curr = curr->insertRecordStringToBlock("def");
    curr = curr->insertRecordStringToBlock("789");
    ASSERT_EQ(b.getRecordAsString(1),"012");
    ASSERT_EQ(b.getRecordAsString(2),"abc");
    ASSERT_EQ(b.next->getRecordAsString(1),"345");
    ASSERT_EQ(b.next->getRecordAsString(2),"def");

    BlockListNode::saveToDisk(&b, "testdatafile.txt");
    BlockListNode* read = readFileFromHardDisk("testdatafile.txt", 12);
    ASSERT_EQ(read->getEmptyBytes(), 1);
    ASSERT_EQ(read->getRecordAsString(1), "012");
    ASSERT_EQ(read->getRecordAsString(2), "abc");
    ASSERT_EQ(read->next->getRecordAsString(1), "345");
    ASSERT_EQ(read->next->getRecordAsString(2), "def");
    ASSERT_EQ(read->next->next->getRecordAsString(1), "789");
}

TEST(Database_lib, Save_A_Block_To_Disk_And_Read_Only_One_Block_Back_With_Offset) {
    BlockListNode b = BlockListNode(12);
    BlockListNode* curr = &b;
    curr = curr->insertRecordStringToBlock("012");
    curr = curr->insertRecordStringToBlock("abc");
    curr = curr->insertRecordStringToBlock("345");
    curr = curr->insertRecordStringToBlock("def");
    curr = curr->insertRecordStringToBlock("789");

    BlockListNode::saveToDisk(&b, "testdatafile.txt");
    BlockListNode* read1 = readBlockWithLSeek("testdatafile.txt", 1, 12);
    ASSERT_EQ(read1->getRecordAsString(1), "012");
    ASSERT_EQ(read1->getRecordAsString(2), "abc");
    BlockListNode* read2 = readBlockWithLSeek("testdatafile.txt", 2, 12);
    ASSERT_EQ(read2->getRecordAsString(1), "345");
    ASSERT_EQ(read2->getRecordAsString(2), "def");
    BlockListNode* read3 = readBlockWithLSeek("testdatafile.txt", 3, 12);
    ASSERT_EQ(read3->getRecordAsString(1), "789");
}

// generate 3 block nodes via the insert method
TEST(Database_lib, Generate_Linked_List_via_Insert) {
    BlockListNode* b1 = new BlockListNode(8);
    BlockListNode* dummy = b1;
    // cnt, 0, 3, \0, \0, 0, 1
    b1 = b1->insertRecordStringToBlock("01");
    ASSERT_EQ(b1->getRecordAsString(1),"01");
    ASSERT_EQ(b1->getEmptyBytes(), 3);

    b1 = b1->insertRecordStringToBlock("02");
    ASSERT_EQ(b1->getRecordAsString(1),"02");
    b1 = b1->insertRecordStringToBlock("?");
    ASSERT_EQ(b1->getRecordAsString(2),"?");

    b1 = b1->insertRecordStringToBlock("03");
    ASSERT_EQ(b1->getRecordAsString(1),"03");

    // loop through the linked list
    // cnt # of nodes
    int cnt = 0;
    while (dummy != nullptr){
        cnt+=1;
        ASSERT_EQ(dummy->getRecordAsString(1),"0"+std::to_string(cnt));
        if (cnt == 2){
            ASSERT_EQ(dummy->getRecordAsString(2),"?");
        }
        dummy = dummy->next;
    }
    ASSERT_EQ(cnt, 3);
}

// This test cannot pass on Win machine due to LSeek()
TEST(Database_lib_new, Read_From_Raw_File_And_Save_To_Disk){
    std::vector<Record> records = readRawTxtFile("../../A1_data.txt");
    // let's load all the records into the block linked list:
    BlockListNode *b = new BlockListNode();
    BlockListNode *dummyHead = b;
    int cnt = 0;
    for (auto r: records) {
        b = b->insertRecordStringToBlock(r.content);
        cnt ++;
    }
    BlockListNode::saveToDisk(dummyHead, "database_file.txt");
    // read everything back
    BlockListNode* readBackHead = readFileFromHardDisk("database_file.txt", 1024);
    ASSERT_EQ(readBackHead->getNumOfRecord(), 14);
    ASSERT_EQ(readBackHead->next->getNumOfRecord(), 14);
    ASSERT_EQ(readBackHead->next->next->getNumOfRecord(), 14);
    ASSERT_EQ(readBackHead->next->next->next->getNumOfRecord(), 13);

    BlockListNode* singleBlock1 = readBlockWithLSeek("database_file.txt", 1, 1024);
    BlockListNode* singleBlock2 = readBlockWithLSeek("database_file.txt", 2, 1024);
    BlockListNode* singleBlock3 = readBlockWithLSeek("database_file.txt", 3, 1024);
    BlockListNode* singleBlock4 = readBlockWithLSeek("database_file.txt", 4, 1024);
    BlockListNode* singleBlockLast = readBlockWithLSeek("database_file.txt", 3365, 1024);

    std::string lastRecord = "zzzjzagrk program at the university of guelph who have financial ";
    ASSERT_EQ(singleBlock1->getNumOfRecord(),14);
    ASSERT_EQ(singleBlock2->getNumOfRecord(),14);
    ASSERT_EQ(singleBlock3->getNumOfRecord(),14);
    ASSERT_EQ(singleBlock4->getNumOfRecord(),13);
    ASSERT_EQ(singleBlockLast->getRecordAsString(6),lastRecord);
}