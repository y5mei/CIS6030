//
// Created by Yao-Wen Mei on 2022/9/20.
//

#include <string>
#include <vector>
#include <iostream>
#include "BPlusTree.h"

using namespace std;

int BPlusTreeReturn(string input){
    return 100;
}

Node::Node(int size, bool isLeaf) {
    // there should be 8 keys, and 9 children;
    this->MAX_SIZE = size;
    this->isLeaf = isLeaf;
}

string Node::search(std::string str) {
    cout<<this->keys.size()<<endl;
    if(isLeaf){
        for (int i = 0; i < this->keys.size(); ++i) {
            if (this->keys[i] == str){
                return this->values[i];
            }
        }
        return "Null"; // cannot find this str
    }else{
        // need to remove stoi for real database keys
        if (stoi(str) < stoi(this->keys[0])){
            return this->children[0]->search(str);
        }else if(stoi(str) > stoi(this->keys.back())){
            return this->children.back()->search(str);
        }else{
            for (int i = 1; i < this->keys.size()-1; ++i) {
                if (stoi(str) >= stoi(this->keys[i]) && stoi(str) < stoi(this->keys[i+1])){
                    return this->children[i]->search(str);
                }
            }
        }
    }

}
