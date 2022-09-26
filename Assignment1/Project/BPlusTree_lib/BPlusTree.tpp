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

template<class T1, class T2>
void sortPairs(vector<T1> &vk, vector<T2> &vv) {
    vector<pair<T1, T2>> vect;
    for (int i = 0; i < vk.size(); i++)
        vect.push_back(make_pair(vk[i], vv[i]));
    // sort
    sort(vect.begin(), vect.end());

    // put back into the same place
    for (int i = 0; i < vect.size(); i++) {
        vk[i] = vect[i].first;
        vv[i] = vect[i].second;
    }
}

template<class T>
void Node<T>::insert(T k, T v) {
    // if the leaf nodeN is not previously fulled, insert it in order;
    Node *nodeN = this->searchNode(k);
    if (nodeN->MAX_SIZE > nodeN->keys.size()) {
        nodeN->keys.push_back(k);
        nodeN->values.push_back(v);
        sortPairs<T, T>(nodeN->keys, nodeN->values);

        // adjust the parent node if necessary
        // TODO: [Ask TA, I feel this code below will never be triggered, the example from class, insert 22 is incorrect]
        Node *parentNode = nodeN->parent;
        if (parentNode != nullptr) { // if leaf node has no parent(is root), do not need to adjust parent
            for (int i = 1; i < parentNode->children.size(); ++i) {
                if (parentNode->children[i] == nodeN) {
                    parentNode->keys[i - 1] = nodeN->keys[0];
                    break;
                }
            }
        }
    } else {
        // if nodeN is previously full, split into two new leaf nodes, N and M
        // TODO: Need to release the new node when not using
//        Node<T> newNode = Node<T>(nodeN->MAX_SIZE, nodeN->isLeaf);
//        Node* nodeM = &newNode;
        Node *nodeM = new Node<T>(nodeN->MAX_SIZE, nodeN->isLeaf);
        nodeM->next = nodeN->next;
        nodeN->next = nodeM;

        nodeN->keys.push_back(k);
        nodeN->values.push_back(v);
        sortPairs<T, T>(nodeN->keys, nodeN->values);

        int resizeSize = ceil((MAX_SIZE + 1.0) / 2.0);
        vector<T> split_keys_lo(nodeN->keys.begin(), nodeN->keys.begin() + resizeSize);
        vector<T> split_keys_hi(nodeN->keys.begin() + resizeSize, nodeN->keys.end());
        vector<T> split_vals_lo(nodeN->values.begin(), nodeN->values.begin() + resizeSize);
        vector<T> split_vals_hi(nodeN->values.begin() + resizeSize, nodeN->values.end());

        nodeN->keys = split_keys_lo;
        nodeM->keys = split_keys_hi;
        nodeN->values = split_vals_lo;
        nodeM->values = split_vals_hi;

        // insert the smallest key in M and the pointer to M into N's parent
        Node *parentOfN = nodeN->parent;

        if (parentOfN == nullptr) {
            // the root can have as few as 2 children if it is not a leaf
            parentOfN = new Node<T>(MAX_SIZE, false);
            parentOfN->keys.push_back(nodeM->keys.front());
            parentOfN->children.push_back(nodeN);
            parentOfN->children.push_back(nodeM);
            nodeN->parent = parentOfN;
            nodeM->parent = parentOfN;
            //TODO: need to return this new parentNode as root;
            return;
        }
        // insert a Key Pointer pair into its parents
        parentOfN->insertLeafNodeIntoInteriorNode(nodeM->keys.front(), nodeM);
    }
}


template<class T>
void Node<T>::insertLeafNodeIntoInteriorNode(T newKey, Node *newChildNode) {
    if (this->MAX_SIZE > this->keys.size()) {
        // increase the size of keys and children by one, fill with the key and pointer
        this->children.push_back(newChildNode);
        this->keys.push_back(newKey);

        // We cannot use the ptr->next method to loop through children nodes, as the interior nodes does have the next pointers
        // update children
        sort(children.begin(), children.end(),
             [](Node *a, Node *b) {
                 return (a->keys.front() < b->keys.front());
             });

        // update keys
        // Note that keys are not always equal to the first element in the children's key
        // if an interior node has children's children, the key is the first key of the children's children
        // if an interior node's children is leaf node, then the key is the leaf node's first key
        for (int i = 0; i < this->keys.size(); ++i) {
            Node *c = this->children[i + 1];
            if (c->isLeaf) {
                this->keys[i] = c->keys[0];
            } else {
                Node *grandC = c->children[0];
                this->keys[i] = grandC->keys[0];
            }
        }

        // update parent
        newChildNode->parent = this;
    } else {
        // sort the pointers, leave the first ceil((m+2)/2) in N and leave the rest in M
        Node *parentN = this;
        Node *parentM = new Node<T>(this->MAX_SIZE, this->isLeaf);

        children.push_back(newChildNode);
        keys.push_back(newKey);

        // sort all the children
        sort(children.begin(), children.end(),
             [](Node *a, Node *b) {
                 return (a->keys.front() < b->keys.front());
             });

        // sort all the keys
        for (int i = 0; i < this->keys.size(); ++i) {
            Node *c = this->children[i + 1];
            if (c->isLeaf) {
                this->keys[i] = c->keys[0];
            } else {
                Node *grandC = c->children[0];
                this->keys[i] = grandC->keys[0];
            }
        }

        // separate all the keys and ptrs for N and M
        Node *currChild = this->children.front();
        vector<Node *> parentNChild(children.begin(), children.begin() + ceil((MAX_SIZE + 2.0) / 2.0));
        vector<T> parentNKeys(keys.begin(), keys.begin() + ceil(MAX_SIZE / 2.0));
        vector<Node *> parentMChild(children.end() - floor((MAX_SIZE + 2.0) / 2.0), children.end());
        vector<T> parentMKeys(keys.end() - floor(MAX_SIZE / 2.0), keys.end());
        T keyForGrantParent = keys.at(ceil(MAX_SIZE / 2.0));

        // Setup parent node for children in both N and M
        for (Node *ptr: parentNChild) {
            ptr->parent = parentN;
        }
        for (Node *ptr: parentMChild) {
            ptr->parent = parentM;
        }

        // set the keys and ptrs to nodeM and nodeN
        parentN->keys = parentNKeys;
        parentN->children = parentNChild;
        parentM->keys = parentMKeys;
        parentM->children = parentMChild;

        // try to insert the new node, nodeM, into the grandParentNode
        Node *grantParent = parentN->parent;
        // copy code from line 117;
        if (grantParent == nullptr) {
            // the root can have as few as 2 children if it is not a leaf
            grantParent = new Node<T>(MAX_SIZE, false);
            grantParent->children.push_back(parentN);
            grantParent->children.push_back(parentM);
            Node *gc = parentM->children[0];
            grantParent->keys.push_back(gc->keys[0]);

            parentN->parent = grantParent;
            parentM->parent = grantParent;
            //TODO: need to return this new parentNode as root;
            return;
        }
        grantParent->insertLeafNodeIntoInteriorNode(keyForGrantParent, parentM);
    }
}

template<class T>
Node<T> *Node<T>::findRoot() {
    Node *curr = this;
    while (curr->parent != nullptr) {
        curr = curr->parent;
    }
    return curr;
}

