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

