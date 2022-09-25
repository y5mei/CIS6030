//
// Created by Yao-Wen Mei on 2022/9/20.
//

#ifndef PROJECT_BPLUSTREE_H
#define PROJECT_BPLUSTREE_H

#include <cmath>

using namespace std;


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


template<class T>
class Node {
public:
    Node *parent = nullptr;
    vector<Node *> children;
    vector<T> keys;
    int number; // number of current inserted children
    int MAX_SIZE;
    bool isLeaf;
    vector<T> values; // only if isLeaf is true
    Node *next = nullptr; // only if isLeaf is true
    Node(int size = 8, bool isLeaf = false);
    Node* findRoot();

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
    Node<T>* root;
    void insert(T key, T val);
    void insert(T keyValue);
    BPlusTree(int size);

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

#include "BPlusTree.tpp"

#endif //PROJECT_BPLUSTREE_H
