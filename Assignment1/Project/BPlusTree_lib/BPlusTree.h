//
// Created by Yao-Wen Mei on 2022/9/20.
//

#ifndef PROJECT_BPLUSTREE_H
#define PROJECT_BPLUSTREE_H

using namespace std;

class BPlusTree {

};

class Node{
public:
    Node* parent;
    vector<Node*> children;
    vector<string> keys;
    int number; // number of current inserted children
    int MAX_SIZE;
    bool isLeaf;
    vector<string> values; // only if isLeaf is true
    Node* next; // only if isLeaf is true
    Node(int size = 8, bool isLeaf = false);

    string search(string str);
};

int BPlusTreeReturn(std::string input);

#endif //PROJECT_BPLUSTREE_H
