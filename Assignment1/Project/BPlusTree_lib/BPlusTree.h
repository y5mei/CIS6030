//
// Created by Yao-Wen Mei on 2022/9/20.
//

#ifndef PROJECT_BPLUSTREE_H
#define PROJECT_BPLUSTREE_H

#include <cmath>

using namespace std;

// convert a char to two short
struct CharShort {
    char high;
    char low;
    short num;

    CharShort(short j) {
        num = j;
        low = j;
        j = j >> 8;
        high = j;
    };

    CharShort(char h, char l) {
        high = h;
        low = l;
        num = h << 8;
        num = num | (unsigned char) l; // make sure the high 8 bits of l is 0 nor 1
    }
};

// covert 4 chars to 4 shorts
struct StingShort {
    short block;
    short record;
    string str;

    // the input str need to be 4 char in lengh
    StingShort(string s) {
        str = s;
        // block
        CharShort cs1 = CharShort(s[0], s[1]);
        block = cs1.num;
        // record
        CharShort cs2 = CharShort(s[2], s[3]);
        record = cs2.num;
    };

    StingShort(short blockNum, short recordNum) {
        block = blockNum;
        record = recordNum;
        CharShort b = CharShort(blockNum);
        CharShort r = CharShort(recordNum);
        str = {b.high, b.low, r.high, r.low};
    }
};


template<class T1, class T2, class T>
class Pair {
public:
    T1 key;
    T2 value;
    vector<T> field;
    int p1;
    int p2;

    Pair(int myp1 = 4, int myp2 = 5);
};

template<class T1, class T2, class T>
Pair<T1, T2, T>::Pair(int myp1, int myp2) {
    this->p1 = myp1;
    this->p2 = myp2;
}

class HardDiskNode {
public:
    short MAX_SIZE;
    bool isLeaf;
    short next = 0; // only if isLeaf is true
    short parent = 0;
    vector<string> keys; // m keys
    vector<short> children; // m+1 children
    vector<string> values; // or m values but only if isLeaf is true
    HardDiskNode(int size = 8, bool isLeaf = false);

    // return which children need to search based on the keys, and k value
    short searchNodeAtNonLeafNode(string k);

    string searchValueOnLeafNode(string k);

    void deseralizeHardDiskNodeFromStr(string str);
};

template<class T>
class Node {
public:
    short MAX_SIZE;
    bool isLeaf;
    Node *next = nullptr; // only if isLeaf is true
    Node *parent = nullptr;
    vector<T> keys; // m keys
    vector<Node *> children; // m+1 children
    vector<T> values; // or m values but only if isLeaf is true



    Node(int size = 8, bool isLeaf = false);

    Node *findRoot();

    T search(T k);

    Node *searchNode(T k);

    void insert(T k, T v);

    friend ostream &operator<<(ostream &os, Node const &node) {
        string s = "";
        if (node.isLeaf) {
            s += "values for leaf nodes: \n";
            for (auto i: node.values) {
                s += to_string(i) + " ";
            }
        } else {
            s += "keys for interior nodes: \n";
            for (auto i: node.keys) {
                s += to_string(i) + " ";
            }
        }

        return os << s;
    }

private:
    void insertLeafNodeIntoInteriorNode(T key, Node *newChildNode);
};


template<class T>
class BPlusTree {
public:
    Node<T> *root;

    void insert(T key, T val);

    void insert(T keyValue);

    BPlusTree(int size);

    T search(T key);

    Node<T> *searchNode(T key);

    T searchInOrderSuccessor(T key);
};

template<class T>
BPlusTree<T>::BPlusTree(int size) {
    root = new Node<T>(size, true);
}

template<class T>
void BPlusTree<T>::insert(T key, T val) {
    root->insert(key, val);
    root = root->findRoot();
}

template<class T>
void BPlusTree<T>::insert(T key) {
    root->insert(key, key);
    root = root->findRoot();
}

int BPlusTreeReturn(std::string input);

template<class T>
T BPlusTree<T>::search(T key) {
    root = root->findRoot();
    return root->search(key);
}

// return the node where to insert this key, so that we can find this
// key's inorder successor
template<class T>
Node<T> *BPlusTree<T>::searchNode(T key) {
    root = root->findRoot();
    return root->searchNode(key);
}

// return the value of the inorder successor of the key, k that we want to insert
// for example, if k = ab, we want to find the value of aa;
template<class T>
T BPlusTree<T>::searchInOrderSuccessor(T k) {
    auto *successor = this->searchNode(k);
    auto lastKey = successor->keys.back();
    auto currKey = k;

    // make sure the key, k, is not in the successor node:
    for (T key: successor->keys) {
        if (key == k) {
            string e = "";
            e += "The searching key: ";
            e += k;
            e += " is already in the BTree and data block; \n If you want to UPDATE the value, you need to delete it first, then insert it again.";
            throw invalid_argument(e);
        }
    }

    // there is a corner case, if the insert record is smaller than the current smallest one, aaagbmhha,
    // need to insert at the very beginning of the data block, then, I will use a block 0 and record 0 to indicate
    // this special return need to be handled by the code that insert a record to data block;
    if (currKey < successor->keys.front()) {
//        CharShort cs = CharShort(0);
//        StingShort ss = StingShort(cs.num, cs.num);
//        return successor->values[0]; // return block 0 record 0
        return k;
    }
    if (currKey > lastKey) {
        return successor->values.back();
    } else {
        for (int i = 0; i < successor->keys.size() - 1; ++i) {
            auto ithKey = successor->keys[i];
            auto iPlusOneKey = successor->keys[i + 1];
            if (currKey > ithKey && currKey < iPlusOneKey) {
                return successor->values[i];
            }
        }
        string e = "";
        e += "Cannot find a place to put the key: ";
        e += k;
        e += " in the current B+Tree.";
        throw invalid_argument(e);
    }
}

// this is a debug method, input a root node, and return the path how to find the key from the root
Node<string> *searchNodeWithDebugPrint(string k, Node<string> *node, vector<Node<string> *> *vec);

void printNodeForDebug(int nodeNum, vector<Node<string> *> *vec);

void levelOrderPrintBTreeForDebug(Node<int> *root);

#include "BPlusTree.tpp"

#endif //PROJECT_BPLUSTREE_H
