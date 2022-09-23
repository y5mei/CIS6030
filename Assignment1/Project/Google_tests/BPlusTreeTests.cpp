#include "gtest/gtest.h"
#include "BPlusTree.h"
#include <string>
#include <vector>

using namespace std;

class BPlusTreeTest : public ::testing::Test {
protected:
    int aaa  =100;
    Node<int> n1 = Node<int>(3, true);
    Node<int> n2 = Node<int>(3, true);
    Node<int> n3 = Node<int>(3, true);
    Node<int> n4 = Node<int>(3, true);
    Node<int> n5 = Node<int>(3, true);
    Node<int> n6 = Node<int>(3, true);
    Node<int> m1 = Node<int>(3, false);
    Node<int> m2 = Node<int>(3, false);
    Node<int> r = Node<int>(3, false);

    void SetUp() override {
        n1.keys = {2,3,5};
        n1.values =  {2,3,5};
        n2.keys = {7, 11};
        n2.values = {7, 11};
        n3.keys = {13, 17, 19};
        n3.values = {13, 17, 19};
        n4.keys = {23, 29};
        n4.values = {23, 29};
        n5.keys = {31, 37, 41};
        n5.values =  {31, 37, 41};
        n6.keys = {43, 47};
        n6.values =  {43, 47};
        m1.keys = {7};
        m1.children = {&n1, &n2};
        m2.keys = {23, 31, 43};
        m2.children = {&n3, &n4, &n5, &n6};
        r.keys = {13};
        r.children = {&m1, &m2};
    }
};

// Strings can be directly compared in cpp.
TEST_F(BPlusTreeTest, String_Can_Be_Compared) {
    string a = "aaa";
    string b = "aab";
    ASSERT_GT(b, a);

    string a1 = "22";
    string b1 = "123";
    ASSERT_GT(a1, b1);
    // This is a very interesting edge case,
    // the string instance must be cast to string first, then it can use the operator overloads.
    ASSERT_EQ((string) "22" > (string) "123", 1);
    ASSERT_EQ("22" < "123", 1);
}

TEST_F(BPlusTreeTest, Node_Key_Type_Is_Generic) {
    Node<string> stringNode = Node<string>();
    ASSERT_EQ(stringNode.MAX_SIZE , 8);

    Node<int> intNode = Node<int>();
    ASSERT_EQ(intNode.MAX_SIZE , 8);
}

TEST_F(BPlusTreeTest, Node_Can_Be_Searched) {
    Node<int>* searchedNode1 = n1.searchNode(2);
    Node<int>* searchedNode2 = n1.searchNode(3);
    Node<int>* searchedNode3 = m1.searchNode(2);
    Node<int>* searchedNode4 = r.searchNode(2);
    Node<int>* searchedNode5 = r.searchNode(37);
    ASSERT_EQ(searchedNode1, &n1);
    ASSERT_EQ(searchedNode2, &n1);
    ASSERT_EQ(searchedNode3, &n1);
    ASSERT_EQ(searchedNode4, &n1);
    ASSERT_EQ(searchedNode5, &n5);
}

TEST_F(BPlusTreeTest, Node_Value_Can_Be_Searched) {
    ASSERT_EQ(r.search(2), 2);
    ASSERT_EQ(r.search(3), 3);
    ASSERT_EQ(r.search(5), 5);
    ASSERT_EQ(r.search(7), 7);
    ASSERT_EQ(r.search(11),11);
    ASSERT_EQ(r.search(13),13);
    ASSERT_EQ(r.search(17),17);
    ASSERT_EQ(r.search(19),19);
    ASSERT_EQ(r.search(23),23);
    ASSERT_EQ(r.search(29),29);
    ASSERT_EQ(r.search(31),31);
    ASSERT_EQ(r.search(37),37);
    ASSERT_EQ(r.search(41),41);
    ASSERT_EQ(r.search(43),43);
    ASSERT_EQ(r.search(47),47);
}