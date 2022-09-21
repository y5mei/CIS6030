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
}


TEST(BPlusTreeTest, Node_Constructor_Works_With_Different_M) {
    Node n1 = Node(3, true);
    vector<string> n1keys = {"2","3","5"};
    n1.keys = n1keys;
    n1.values = n1keys;

    Node n2 = Node(3, true);
    vector<string> n2keys = {"7","11"};
    n2.keys = n2keys;
    n2.values = n2keys;

    Node n3 = Node(3, true);
    n3.keys = {"13","17","19"};
    n3.values = {"13","17","19"};

    Node n4 = Node(3, true);
    n4.keys = {"23","29"};
    n4.values = {"23","29"};

    Node n5 = Node(3, true);
    n5.keys = {"31","37","41"};
    n5.values = {"31","37","41"};

    Node n6 = Node(3, true);
    n6.keys = {"43","47"};
    n6.values = {"43","47"};

    Node m1 = Node(3, false);
    m1.keys = {"7"};
    m1.children = {&n1, &n2};

    Node m2 = Node(3, false);
    m2.keys = {"23","31","43"};
    m2.children = {&n3, &n4, &n5, &n6};

    Node r = Node(3, false);
    r.keys = {"13"};
    r.children = {&m1, &m2};

//    ASSERT_EQ(m1.children[0]->values[0],"2");
//    ASSERT_EQ(m1.keys[0], "7");
//    ASSERT_EQ(r.children[0]->keys[0], "7");
//    ASSERT_EQ(n1.search("2"), "2");
//    ASSERT_EQ(m1.search("2"), "2");
//    ASSERT_EQ(r.search("2"), "2");
    ASSERT_EQ(m1.search("11"),"11");
    ASSERT_EQ(m2.search("13"),"13");
    ASSERT_EQ(m2.search("31"),"31");
}