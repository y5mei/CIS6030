#include "gtest/gtest.h"
#include "DatabaseFileIO.h"


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
    EXPECT_EQ(b.sizeOfEmptyBytes(), 1);
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

// generate 3 block nodes via the insert method
TEST(Database_lib, Generate_Linked_List_via_Insert) {
    BlockListNode* b1 = new BlockListNode(8);
    BlockListNode* dummy = b1;
    // cnt, 0, 3, \0, \0, 0, 1
    b1 = b1->insertRecordStringToBlock("01");
    ASSERT_EQ(b1->getRecordAsString(1),"01");
    ASSERT_EQ(b1->sizeOfEmptyBytes(), 3);

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