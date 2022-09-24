#include <iostream>
#include "BPlusTree.h"
#include <numeric>
#include <algorithm> // sort


template<class T>
Node<T>::Node(int size, bool isLeaf) {
    // there should be 8 keys, and 9 children;
    this->MAX_SIZE = size;
    this->isLeaf = isLeaf;
}

// Search a value from a Key, K
template<class T>
T Node<T>::search(T k) {

    Node<T> *leafNode = searchNode(k);
    for (int i = 0; i < leafNode->keys.size(); ++i) {
        if (leafNode->keys[i] == k) {
            return leafNode->values[i];
        }
    }
    throw invalid_argument("search key does not exist in the current tree!");
}

// Search for the leaf node where the key, k, should be placed;
template<class T>
Node<T> *Node<T>::searchNode(T k) {
    if (isLeaf) {
        return this;
    } else {
        auto firstKey = this->keys.front();
        auto lastKey = this->keys.back();
        auto currKey = k;
        if (currKey < firstKey) {
            return this->children[0]->searchNode(k);
        } else if (currKey >= lastKey) {
            return this->children.back()->searchNode(k);
        } else {
            for (int i = 0; i < this->keys.size() - 1; ++i) {
                auto ithKey = this->keys[i];
                auto iPlusOneKey = this->keys[i + 1];
                if (currKey >= ithKey && currKey < iPlusOneKey) {
                    return this->children[i + 1]->searchNode(k);
                }
            }
            throw invalid_argument("Didn't find the subTree at where it should be!");
        }
    }
}

