//
// Created by Yao-Wen Mei on 2022/9/20.
//

#include <string>
#include <vector>
#include <iostream>
#include <deque>
#include "BPlusTree.h"


using namespace std;

int BPlusTreeReturn(string input){
    return 100;
}

int getNodeNumFormVec(Node<string> *node, vector<Node<string>*> *vec){
    for (int i = 0; i < vec->size(); ++i) {
        if (vec->at(i)==node){
            return i;
        }
    }
}

// this is a debug method, input a root node, and return the path how to find the key from the root
Node<string> *searchNodeWithDebugPrint(string k, Node<string> *node, vector<Node<string>*> *vec) {
    if (node->isLeaf) {
        return node;
    } else {
        string firstKey = node->keys.front();
        string lastKey = node->keys.back();
        string currKey = k;
        std::cout << "================ search "<<k<<" at Node Num: "<< getNodeNumFormVec(node, vec) <<" ===========" << endl;
        std::cout << firstKey << endl;
        std::cout << lastKey << endl;
        std::cout << "---------------- all the keys are ---------------" << endl;
        for(string k: node->keys){
            cout<<k<<endl;
        }
        if (currKey < firstKey) {
            cout <<" ******* Going to Search Node: "<<getNodeNumFormVec(node->children[0], vec)<<endl;
            return searchNodeWithDebugPrint(k, node->children[0], vec);
        } else if (currKey >= lastKey) {
            cout <<" ******* Going to Search Node: "<<getNodeNumFormVec(node->children.back(), vec)<<endl;
            return searchNodeWithDebugPrint(k, node->children.back(), vec);
        } else {
            for (int i = 0; i < node->keys.size() - 1; ++i) {
                auto ithKey = node->keys[i];
                auto iPlusOneKey = node->keys[i + 1];
                if (currKey >= ithKey && currKey < iPlusOneKey) {
                    cout <<" ******* Going to Search Node: "<<getNodeNumFormVec(node->children[i + 1], vec)<<endl;
                    cout<<"This is the child at index "<<(i+1)<<endl;
                    return searchNodeWithDebugPrint(k, node->children[i + 1], vec);
                }
            }
            throw invalid_argument("Didn't find the subTree at where it should be!");
        }
    }
}

void printNodeForDebug(int nodeNum, vector<Node<string>*> *vec){
    auto* node = vec->at(nodeNum);
    cout<<"------- The keys for node num: "<<nodeNum<<" are:"<<endl;
    for (string k: node->keys) {
        cout<<k<<endl;
    }
}

void printAllKeysOfANode(Node<int>* node){
    for (int i = 0; i < node->keys.size(); ++i) {
        cout<<node->keys[i]<<",";
    }
    cout<<"    | "<<endl;
}

void levelOrderPrintBTreeForDebug(Node<int>* root){
    deque<Node<int> *> q;
    q.push_back(root);
    short cnt = 1;

    while (!q.empty()) {
        int size = q.size();
        cout<<"====== Printing level ====== "<<cnt++<<endl;
        for (int i = 0; i < size; ++i) {
            Node<int> *currNode = q.front();
            cout<<"~~~~~~"<<currNode<<endl;
            printAllKeysOfANode(currNode);
            q.pop_front();

            for (auto *child: currNode->children) {
                q.push_back(child);
            }
        }
    }
}

// Search for the leaf node where the key, k, should be placed;
short HardDiskNode::searchNodeAtNonLeafNode(string k) {
    auto firstKey = this->keys.front();
    auto lastKey = this->keys.back();
    auto currKey = k;

    if (currKey < firstKey) {
        return this->children.front();
    } else if (currKey >= lastKey) {
        return this->children.back();
    } else {
        for (int i = 0; i < this->keys.size() - 1; ++i) {
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
string HardDiskNode::searchValueOnLeafNode(std::string k) {
    for (int i = 0; i < this->keys.size(); ++i) {
        if (this->keys[i] == k) {
            return this->values[i];
        }
    }
    return "-1";
}

HardDiskNode::HardDiskNode(int size, bool isLeaf) {
    // there should be 8 keys, and 9 children;
    this->MAX_SIZE = size;
    this->isLeaf = isLeaf;
}

void HardDiskNode::deseralizeHardDiskNodeFromStr(string str) {
    HardDiskNode* node = this;
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

    // Each time a tree niode is read in RAM, all the keys at it are displayied
    cout << ">> Reading a TreeNode to RAM with keys: ";
    for (string k: node->keys) {
        cout << k << " ";
    }
    cout << "." << endl;

    short numOfChildren = CharShort(str[idx++], str[idx++]).num;
    node->children.clear();
    for (int i = 0; i < numOfChildren; ++i) {
        short child = CharShort(str[idx++], str[idx++]).num;
        node->children.push_back(child);
    }

    short numOfvalues = CharShort(str[idx++], str[idx++]).num;
    node->values.clear();
    for (int i = 0; i < numOfvalues; ++i) {
        string valStr = {str[idx++], str[idx++], str[idx++], str[idx++]};
        node->values.push_back(valStr);
    }
}
