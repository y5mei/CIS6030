#include "gtest/gtest.h"
#include "gmock/gmock.h"  // Brings in Google Mock.
#include "BPlusTree.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;
using ::testing::ElementsAre;
using ::testing::ElementsAreArray;

class BPlusTreeListNodeTest : public ::testing::Test {
protected:
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
        n1.keys = {2, 3, 5};
        n1.values = {2, 3, 5};
        n2.keys = {7, 11};
        n2.values = {7, 11};
        n3.keys = {13, 17, 19};
        n3.values = {13, 17, 19};
        n4.keys = {23, 29};
        n4.values = {23, 29};
        n5.keys = {31, 37, 41};
        n5.values = {31, 37, 41};
        n6.keys = {43, 47};
        n6.values = {43, 47};
        m1.keys = {7};
        m1.children = {&n1, &n2};
        n1.parent = &m1;
        n2.parent = &m1;
        m2.keys = {23, 31, 43};
        m2.children = {&n3, &n4, &n5, &n6};
        n3.parent = &m2;
        n4.parent = &m2;
        n5.parent = &m2;
        n6.parent = &m2;
        r.keys = {13};
        r.children = {&m1, &m2};
        m1.parent = &r;
        m2.parent = &r;
        n1.next = &n2;
        n2.next = &n3;
        n3.next = &n4;
        n4.next = &n5;
        n5.next = &n6;
    }
};

// Strings can be directly compared in cpp.
TEST_F(BPlusTreeListNodeTest, String_Can_Be_Compared) {
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

TEST_F(BPlusTreeListNodeTest, Node_Key_Type_Is_Generic) {
    Node<string> stringNode = Node<string>();
    ASSERT_EQ(stringNode.MAX_SIZE, 8);

    Node<int> intNode = Node<int>();
    ASSERT_EQ(intNode.MAX_SIZE, 8);
}

TEST_F(BPlusTreeListNodeTest, Node_Can_Be_Searched) {
    Node<int> *searchedNode1 = n1.searchNode(2);
    Node<int> *searchedNode2 = n1.searchNode(3);
    Node<int> *searchedNode3 = m1.searchNode(2);
    Node<int> *searchedNode4 = r.searchNode(2);
    Node<int> *searchedNode5 = r.searchNode(37);
    ASSERT_EQ(searchedNode1, &n1);
    ASSERT_EQ(searchedNode2, &n1);
    ASSERT_EQ(searchedNode3, &n1);
    ASSERT_EQ(searchedNode4, &n1);
    ASSERT_EQ(searchedNode5, &n5);
}

TEST_F(BPlusTreeListNodeTest, Node_Value_Can_Be_Searched) {
    ASSERT_EQ(r.search(2), 2);
    ASSERT_EQ(r.search(3), 3);
    ASSERT_EQ(r.search(5), 5);
    ASSERT_EQ(r.search(7), 7);
    ASSERT_EQ(r.search(11), 11);
    ASSERT_EQ(r.search(13), 13);
    ASSERT_EQ(r.search(17), 17);
    ASSERT_EQ(r.search(19), 19);
    ASSERT_EQ(r.search(23), 23);
    ASSERT_EQ(r.search(29), 29);
    ASSERT_EQ(r.search(31), 31);
    ASSERT_EQ(r.search(37), 37);
    ASSERT_EQ(r.search(41), 41);
    ASSERT_EQ(r.search(43), 43);
    ASSERT_EQ(r.search(47), 47);
}

TEST(Sort_Permutation, Two_Vector_Can_Be_Sort_Together) {
    vector<int> va = {3, 4, 2, 1};
    vector<string> vb = {"3", "4", "2", "1"};
    sortPairs<int, string>(va, vb);
    ASSERT_EQ(va[0], 1);
    ASSERT_EQ(va[3], 4);
    ASSERT_EQ(vb[0], "1");
    ASSERT_EQ(vb[3], "4");
}

TEST_F(BPlusTreeListNodeTest, Root_Can_Be_Found_From_Any_Node) {
    ASSERT_EQ(r.findRoot(), &r);
    ASSERT_EQ(m1.findRoot(), &r);
    ASSERT_EQ(m2.findRoot(), &r);
    ASSERT_EQ(n6.findRoot(), &r);
}

TEST_F(BPlusTreeListNodeTest, Value_Can_Be_Inserted_To_LeafNode_With_Empty_Slots) {
//    r.insert(30, 30);
//    ASSERT_EQ(r.search(30), 30);
    r.insert(6, 6);
    ASSERT_EQ(r.search(6), 6);
    ASSERT_EQ(n1.parent, &m1);
    ASSERT_EQ(m1.children.size(), 3);
    ASSERT_EQ(m1.keys.size(), 2);

    ASSERT_EQ(m1.keys[0], 5);
    ASSERT_EQ(m1.keys[1], 7);
    ASSERT_EQ(m1.children[0], &n1);
    ASSERT_EQ(m1.children[1], n1.next);
    ASSERT_EQ(m1.children[2], &n2);
}

TEST_F(BPlusTreeListNodeTest, Value_Can_Be_Inserted_To_LeafNode_Without_Empty_Slots) {
    r.insert(40, 40);
    ASSERT_EQ(r.search(40), 40);
    ASSERT_EQ(r.children.size(), 3);
    vector<int> v1{13, 40};
    ASSERT_EQ(r.keys, v1);
    ASSERT_EQ(r.children[1], &m2);

    vector<int> v2{23, 31};
    ASSERT_EQ(m2.keys, v2);
    ASSERT_EQ(r.children[2]->keys.size(), 1);
    ASSERT_EQ(r.children[2]->keys[0], 43);
    vector<int> v5{23, 29};
    ASSERT_EQ(m2.children[1]->values, v5);
    ASSERT_EQ(m2.children[1]->keys, v5);
    vector<int> v6{31, 37};
    ASSERT_EQ(m2.children[2]->values, v6);
    ASSERT_EQ(m2.children[2]->keys, v6);

    auto m3 = r.children[2];
    ASSERT_EQ(m3->children.size(), 2);
    vector<int> v3{40, 41};
    ASSERT_EQ(m3->children[0]->values, v3);
    ASSERT_EQ(m3->children[0]->keys, v3);
    vector<int> v4{43, 47};
    ASSERT_EQ(m3->children[1]->values, v4);
    ASSERT_EQ(m3->children[1]->keys, v4);

    auto nodeM = m3->children[0];
    ASSERT_EQ(n1.next, &n2);
    ASSERT_EQ(n2.next, &n3);
    ASSERT_EQ(n3.next, &n4);
    ASSERT_EQ(n4.next, &n5);
    ASSERT_EQ(n5.next, nodeM);
    ASSERT_EQ(nodeM->next, &n6);

    // root can be accessed as well
    ASSERT_EQ(nodeM->findRoot(), &r);
    ASSERT_EQ(m3->findRoot(), &r);
}


TEST(BPlusTreeTest, Value_Can_Be_Inserted_To_Empty_Head) {
    BPlusTree<int> b = BPlusTree<int>(3);
    ASSERT_EQ(b.root->MAX_SIZE, 3);
    ASSERT_EQ(b.root->isLeaf, true);

    b.insert(2, 2);
    b.insert(3, 3);
    b.insert(11, 11);
    vector<int> v1{2, 3, 11};
    ASSERT_EQ(b.root->keys, v1);
    ASSERT_EQ(b.root->values, v1);

    b.insert(7, 7);
    vector<int> v2{2, 3};
    vector<int> v3{7, 11};
    auto n1 = b.root->children[0];
    auto n2 = b.root->children[1];
    ASSERT_EQ(b.root->children[0]->keys, v2);
    ASSERT_EQ(b.root->children[0]->values, v2);
    ASSERT_EQ(b.root->children[1]->keys, v3);
    ASSERT_EQ(b.root->children[1]->values, v3);
    ASSERT_EQ(b.root->children[0]->next, b.root->children[1]);
    ASSERT_EQ(b.root->keys[0], 7);
    ASSERT_EQ(b.root->children.size(), 2);

    b.insert(5, 5);
    b.insert(13, 13);
    vector<int> v4{2, 3, 5};
    ASSERT_EQ(n1->keys, v4);

    b.insert(17, 17);
    b.insert(23, 23);
    b.insert(29, 29);
    // should be [7, 13, 23] -> [2, 3, 5], [7, 11], [13, 17], [23, 29];
    vector<int> v5{13, 17};
    vector<int> v6{23, 29};
    vector<int> v7{7, 13, 23};
    auto n3 = b.root->children[2];
    auto n4 = b.root->children[3];

    ASSERT_EQ(b.root->keys, v7);
    ASSERT_EQ(n1->keys, v4);
    ASSERT_EQ(n2->keys, v3);
    ASSERT_EQ(n3->keys, v5);
    ASSERT_EQ(n4->keys, v6);

    b.insert(31, 31);
    b.insert(37, 37);
    auto m1 = b.root->children[0];
    auto m2 = b.root->children[1];
    auto n5 = m2->children[1];
    EXPECT_THAT(b.root->keys, ElementsAre(23));
    EXPECT_THAT(m1->keys, ElementsAre(7, 13));
    EXPECT_THAT(m2->keys, ElementsAre(31));
    EXPECT_THAT(n1->keys, ElementsAre(2,3,5));
    EXPECT_THAT(n2->keys, ElementsAre(7,11));
    EXPECT_THAT(n3->keys, ElementsAre(13, 17));
    EXPECT_THAT(n4->keys, ElementsAre(23, 29));
    EXPECT_THAT(n5->keys, ElementsAre(31, 37));
    ASSERT_EQ(m1->next, nullptr);
}

TEST(BPlusTreeTest, Build_A_BTree_From_Null) {

    //Test Example from Youtube Video:
    //https://www.youtube.com/watch?v=DqcZLulVJ0M
    BPlusTree<int> b = BPlusTree<int>(3);
    vector<int> arr{1, 4, 7, 10, 17};
    for(int n: arr){
        b.insert(n);
    }
    EXPECT_THAT(b.root->keys, ElementsAre(7));
    EXPECT_THAT(b.root->children[0]->keys, ElementsAre(1, 4));
    EXPECT_THAT(b.root->children[1]->keys, ElementsAre(7, 10, 17));

    b.insert(21);
    b.insert(31);
    b.insert(25);
    b.insert(19);
    EXPECT_THAT(b.root->keys, ElementsAre(7, 17, 25));

    b.insert(20);
    b.insert(28);
    EXPECT_THAT(b.root->keys, ElementsAre(20));
    EXPECT_THAT(b.root->children[0]->keys, ElementsAre(7, 17));
    EXPECT_THAT(b.root->children[1]->keys, ElementsAre(25));
    auto m1 = b.root->children[0];
    auto m2 = b.root->children[1];
    auto n1 = b.root->children[0]->children[0];
    auto n2 = b.root->children[0]->children[1];
    auto n3 = b.root->children[0]->children[2];
    auto n4 = b.root->children[1]->children[0];
    auto n5 = b.root->children[1]->children[1];
    EXPECT_THAT(n1->keys, ElementsAre(1, 4));
    EXPECT_THAT(n2->keys, ElementsAre(7, 10));
    EXPECT_THAT(n3->keys, ElementsAre(17, 19));
    EXPECT_THAT(n4->keys, ElementsAre(20, 21));
    EXPECT_THAT(n5->keys, ElementsAre(25, 28, 31));
    EXPECT_THAT(m1->children, ElementsAre(n1, n2, n3));
    EXPECT_THAT(m2->children, ElementsAre(n4, n5));
    EXPECT_THAT(n1->parent, m1);
    EXPECT_THAT(n2->parent, m1);
    EXPECT_THAT(n3->parent, m1);
    EXPECT_THAT(n4->parent, m2);
    EXPECT_THAT(n5->parent, m2);
    b.insert(42);
    auto n6 = n5->next;
    EXPECT_THAT(b.root->keys, ElementsAre(20));
    EXPECT_THAT(m1->keys, ElementsAre(7, 17));
    EXPECT_THAT(m2->keys, ElementsAre(25, 31));
    EXPECT_THAT(b.root->children, ElementsAre(m1, m2));
    EXPECT_THAT(m1->parent, b.root);
    EXPECT_THAT(m2->parent, b.root);
    EXPECT_THAT(n5->keys, ElementsAre(25, 28));
    EXPECT_THAT(n6->keys, ElementsAre(31, 42));
    EXPECT_THAT(m1->children, ElementsAre(n1, n2, n3));
    EXPECT_THAT(m2->children, ElementsAre(n4, n5, n6));
}

TEST(BPlusTreeTest, BTree_Values_Can_Be_Searched) {

    //Test Example from Youtube Video:
    //https://www.youtube.com/watch?v=DqcZLulVJ0M
    BPlusTree<int> b = BPlusTree<int>(3);
    vector<int> arr{1, 4, 7, 10, 17, 21, 31, 25, 19, 20, 28, 42};
    for(int n: arr){
        b.insert(n);
    }

    for (int n: arr){
        EXPECT_THAT(b.search(n), n);
    }


}