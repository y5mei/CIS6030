#include <iostream>
#include "BPlusTree.h"


template<class T>
Node<T>::Node(int size, bool isLeaf) {
    // there should be 8 keys, and 9 children;
    this->MAX_SIZE = size;
    this->isLeaf = isLeaf;
}

template<class T>
T Node<T>::search(T k) {
    if (isLeaf) {
        for (int i = 0; i < this->keys.size(); ++i) {
            if (this->keys[i] == k) {
                return this->values[i];
            }
        }
        throw invalid_argument("search key does not exist in the current tree!");
    } else {
        auto firstKey = this->keys.front();
        auto lastKey = this->keys.back();
        auto currKey = k;
        if (currKey < firstKey) {
            return this->children[0]->search(k);
        } else if (currKey >= lastKey) {
            return this->children.back()->search(k);
        } else {
            for (int i = 0; i < this->keys.size() - 1; ++i) {
                auto ithKey = this->keys[i];
                auto iPlusOneKey = this->keys[i + 1];
                if (currKey >= ithKey && currKey < iPlusOneKey) {
                    return this->children[i + 1]->search(k);
                }
            }
            throw invalid_argument("Didn't find the subTree at where it should be!");
        }
    }
}