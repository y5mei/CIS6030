//
// Created by Yao-Wen Mei on 2022/9/20.
//

#ifndef PROJECT_BPLUSTREE_H
#define PROJECT_BPLUSTREE_H

using namespace std;

class BPlusTree {

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






template<class T>
class Node {
public:
    Node *parent;
    vector<Node *> children;
    vector<T> keys;
    int number; // number of current inserted children
    int MAX_SIZE;
    bool isLeaf;
    vector<T> values; // only if isLeaf is true
    Node *next; // only if isLeaf is true
    Node(int size = 8, bool isLeaf = false);

    T search(T k);
};


int BPlusTreeReturn(std::string input);

#include "BPlusTree.tpp"
#endif //PROJECT_BPLUSTREE_H
