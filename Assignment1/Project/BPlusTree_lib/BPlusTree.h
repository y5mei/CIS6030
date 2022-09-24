//
// Created by Yao-Wen Mei on 2022/9/20.
//

#ifndef PROJECT_BPLUSTREE_H
#define PROJECT_BPLUSTREE_H

#include <cmath>

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
    Node* searchNode(T k);
    void insert(T k, T v);

    friend ostream& operator<<(ostream& os, Node const & node) {
        string s = "";
        if(node.isLeaf){
            s+="values for leaf nodes: \n";
            for (auto i: node.values){
                s += to_string(i)+" ";
            }
        }else{
            s+="keys for interior nodes: \n";
            for (auto i: node.keys){
                s+= to_string(i)+" ";
            }
        }

        return os << s;
    }

private:
    void insertLeafNodeIntoInteriorNode(T key, Node* newChildNode);
};

template <class T1, class T2>
void sortPairs(vector<T1> &vk, vector<T2> &vv){
    vector<pair <T1, T2>> vect;
    for (int i = 0; i<vk.size(); i++)
        vect.push_back(make_pair(vk[i],  vv[i]));
    // sort
    sort(vect.begin(), vect.end());

    // put back into the same place
    for(int i = 0; i<vect.size(); i++){
        vk[i] = vect[i].first;
        vv[i] = vect[i].second;
    }
}

template<class T>
void Node<T>::insert(T k, T v) {
    // if the leaf nodeN is not previously fulled, insert it in order;
    Node* nodeN = this->searchNode(k);
    if (nodeN->MAX_SIZE > nodeN->keys.size()){
        int currSize = nodeN->keys.size();
        nodeN->keys.push_back(k);
        nodeN->values.push_back(v);
        sortPairs<int, int>(nodeN->keys, nodeN->values);
    }else{
        // if nodeN is previously full, split into two new leaf nodes, N and M
        // TODO: Need to release the new node when not using
//        Node<T> newNode = Node<T>(nodeN->MAX_SIZE, nodeN->isLeaf);
//        Node* nodeM = &newNode;
        Node* nodeM = new Node<T>(nodeN->MAX_SIZE, nodeN->isLeaf);
        nodeM->next = nodeN->next;
        nodeN->next = nodeM;

        nodeN->keys.push_back(k);
        nodeN->values.push_back(v);
        sortPairs<int, int>(nodeN->keys, nodeN->values);

        int resizeSize = ceil((MAX_SIZE + 1)/2);
        vector<T> split_keys_lo(nodeN->keys.begin(), nodeN->keys.begin()+resizeSize);
        vector<T> split_keys_hi(nodeN->keys.begin()+resizeSize, nodeN->keys.end());
        vector<T> split_vals_lo(nodeN->values.begin(), nodeN->values.begin()+resizeSize);
        vector<T> split_vals_hi(nodeN->values.begin()+resizeSize, nodeN->values.end());

        nodeN->keys = split_keys_lo;
        nodeM->keys = split_keys_hi;
        nodeN->values = split_vals_lo;
        nodeM->values = split_vals_hi;

        // insert the smallest key in M and the pointer to M into N's parent
        Node* parentOfN = nodeN->parent;
        // insert a Key Pointer pair into its parents
        parentOfN->insertLeafNodeIntoInteriorNode(nodeM->keys.front(), nodeM);
        // if nodeN is the first node in its parent, we need to update the parent's key
//        if (parentOfN->children.front()==nodeN){
//            parentOfN->keys[0] = nodeM->keys[0];
//        }
    }
}


template<class T>
void Node<T>::insertLeafNodeIntoInteriorNode(T newKey, Node *newChildNode) {
    if (this->MAX_SIZE > this->keys.size()){
        Node* ptr = this->children.front();
        // increase the size of keys and children by one, fill with anything
        this->children.push_back(newChildNode);
        this->keys.push_back(newKey);
        // update children
        int idx = 0;
        while(idx < this->children.size()){
            this->children[idx] = ptr;
            idx +=1;
            ptr = ptr->next;
        }
        // update keys
        for(int i = 0; i<this->keys.size(); ++i){
            Node* c = this->children[i+1];
            this->keys[i] = c->keys[0];
        }
        // update parent
        newChildNode->parent = this;
    }else{

    }
}

int BPlusTreeReturn(std::string input);

#include "BPlusTree.tpp"
#endif //PROJECT_BPLUSTREE_H
