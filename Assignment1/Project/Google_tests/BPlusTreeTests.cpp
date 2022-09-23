#include "gtest/gtest.h"
#include "BPlusTree.h"
#include <string>
#include <vector>

using namespace std;

// Strings can be directly compared in cpp.
TEST(BPlusTreeTest, String_Can_Be_Compared) {
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

TEST(BPlusTreeTest, Node_Key_Type_Is_Generic) {
    Node<string> n = Node<string>();
    ASSERT_EQ(n.MAX_SIZE , 8);
    Pair<int, int, int> p = Pair<int, int ,int>();
    ASSERT_EQ(p.p2, 5);
}

TEST(BPlusTreeTest, Node_Constructor_Works_With_Different_M) {
    Node<int> n1 = Node<int>(3, true);
    n1.keys = {2,3,5};
    n1.values =  {2,3,5};

    Node<int> n2 = Node<int>(3, true);
    n2.keys = {7, 11};
    n2.values = {7, 11};

    Node<int> n3 = Node<int>(3, true);
    n3.keys = {13, 17, 19};
    n3.values = {13, 17, 19};

    Node<int> n4 = Node<int>(3, true);
    n4.keys = {23, 29};
    n4.values = {23, 29};

    Node<int> n5 = Node<int>(3, true);
    n5.keys = {31, 37, 41};
    n5.values =  {31, 37, 41};

    Node<int> n6 = Node<int>(3, true);
    n6.keys = {43, 47};
    n6.values =  {43, 47};

    Node<int> m1 = Node<int>(3, false);
    m1.keys = {7};
    m1.children = {&n1, &n2};

    Node<int> m2 = Node<int>(3, false);
    m2.keys = {23, 31, 43};
    m2.children = {&n3, &n4, &n5, &n6};

    Node<int> r = Node<int>(3, false);
    r.keys = {13};
    r.children = {&m1, &m2};

    ASSERT_EQ(r.search(2), 2);
    ASSERT_EQ(r.search(3), 3);
    ASSERT_EQ(r.search(5), 5);
    ASSERT_EQ(r.search(7), 7);
    ASSERT_EQ(m1.search(11),11);
    ASSERT_EQ(m2.search(13),13);
    ASSERT_EQ(m2.search(17),17);
    ASSERT_EQ(m2.search(19),19);
    ASSERT_EQ(m2.search(23),23);
    ASSERT_EQ(m2.search(29),29);
    ASSERT_EQ(m2.search(31),31);
    ASSERT_EQ(m2.search(37),37);
    ASSERT_EQ(m2.search(41),41);
    ASSERT_EQ(m2.search(43),43);
    ASSERT_EQ(m2.search(47),47);
}