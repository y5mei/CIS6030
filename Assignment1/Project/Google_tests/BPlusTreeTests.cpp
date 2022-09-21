#include "gtest/gtest.h"
#include "BPlusTree.h"

TEST(BPlusTreeTest, bbb) {
    std::string pig = "mayson";
    ASSERT_EQ(BPlusTreeReturn(pig), 100);
}