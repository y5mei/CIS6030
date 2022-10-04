//
// Created by Yao-Wen Mei on 2022/9/20.
//

#ifndef PROJECT_BPLUSTREE_H
#define PROJECT_BPLUSTREE_H

#include <cmath>
#include <bitset>

using namespace std;

// convert a char to two short
struct CharShort {
    char high;
    char low;
    unsigned short num;

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
    unsigned short block;
    unsigned short record;
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

    HardDiskNode(short maxSize = 8, bool isLeaf = false) : MAX_SIZE(maxSize), isLeaf(isLeaf) {}

// Search for the leaf node where the key, k, should be placed;
    short searchNodeAtNonLeafNode(const string& k) {
        auto firstKey = this->keys.front();
        auto lastKey = this->keys.back();
        const auto& currKey = k;

        if (currKey < firstKey) {
            return this->children.front();
        } else if (currKey >= lastKey) {
            return this->children.back();
        } else {
            int limit = keys.size() - 1;
            for (int i = 0; i < limit; ++i) {
                auto ithKey = this->keys[i];
                auto iPlusOneKey = this->keys[i + 1];
                if (currKey >= ithKey && currKey < iPlusOneKey) {
                    return this->children.at(i+1);
                }
            }
            throw invalid_argument("Didn't find the subTree at where it should be!");
        }
    }

    // Search a value from a Key, K, return -1 if key does not exist;
    string searchValueOnLeafNode(const std::string& k) {
        int limit = keys.size();
        for (int i = 0; i < limit; ++i) {
            if (this->keys[i] == k) {
                return this->values[i];
            }
        }
        return "-1";
    }
    void deseralizeHardDiskNodeFromStr(string str, bool isPrintOut=true) {
        HardDiskNode *node = this;
        short leaf = str[0];
        node->isLeaf = leaf;

        short m = CharShort(str[1], str[2]).num;
        node->MAX_SIZE = m;

        short next = CharShort(str[3], str[4]).num;
        node->next = next;

        short parent = CharShort(str[5], str[6]).num;
        node->parent = parent;

        short numOfKeys = CharShort(str[7], str[8]).num;
        int idx = 9;
        node->keys.clear();
        for (int i = 0; i < numOfKeys; ++i) {
            string key = "";
            for (int j = 0; j < 9; ++j) {
                key = key + str[idx];
                idx++;
            }
            node->keys.push_back(key);
        }

        if(isPrintOut){
            // Each time a tree niode is read in RAM, all the keys at it are displayied
            cout << ">> Reading a TreeNode to RAM with keys: ";
            for (string k: node->keys) {
                cout << k << " ";
            }
            cout << "." << endl;
        }

        short numOfChildren = CharShort(str[idx], str[idx+1]).num;
        idx = idx +2;
        node->children.clear();
        for (int i = 0; i < numOfChildren; ++i) {
            short child = CharShort(str[idx], str[idx+1]).num;
            idx = idx +2;
            node->children.push_back(child);
        }

        short numOfvalues = CharShort(str[idx], str[idx+1]).num;
        idx = idx +2;
        node->values.clear();
        for (int i = 0; i < numOfvalues; ++i) {
            string valStr = {str[idx], str[idx+1], str[idx+2], str[idx+3]};
            idx = idx + 4;
            node->values.push_back(valStr);
        }
    }
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
    void del(T k);
    short getMinKeyNUM() {
        return floor((MAX_SIZE+1)/2);
    }

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

    void del(T key);

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

template<class T>
void BPlusTree<T>::del(T key) {
    // delete a key from B+Tree given that this key exist in the Tree
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
            e += "The key value: ";
            e += k;
            e += " is already in the BTree and data block; \nIf you want to UPDATE the value, you need to delete it first, then insert it again.";
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
        int limit = successor->keys.size() - 1;
        for (int i = 0; i < limit; ++i) {
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
