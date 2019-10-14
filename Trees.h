#include <iostream>
#include <vector>
#ifndef AVLTREE_H
#define AVLTREE_H


template<typename T>
class AVLTree{
public:
    struct node{
        T key;
        node* left = nullptr;
        node* right = nullptr;
        node(){}
        node(T key):
            key(key){}
    };
    node *root = nullptr;
private:
    void INSERT(node*& cur, T& key){
        if(cur == nullptr){
            cur = new node(key);
            return;
        }
        if(key < cur->key){
            INSERT(cur->left, key);
        } else {
            INSERT(cur->right, key);
        }
    }
    void VIEW(node*& cur, int i = 0) const {
        if(cur == nullptr){
            return;
        }
        if(cur->left != nullptr){
            VIEW(cur->left, i+1);
        }
        std::cout << i << " " << cur->key << "\n";
        if(cur->right){
            VIEW(cur->right, i+1);
        }
    }
    void getStat(std::vector<int> &stat, node* cur, size_t h = 0){
        if(cur == nullptr){
            return;
        }
        if(stat.size() <= h){
            stat.push_back(1);
        } else {
            stat[h]++;
        }
        if(cur->left != nullptr){
            getStat(stat, cur->left, h+1);
        }
        if(cur->right != nullptr){
            getStat(stat, cur->right, h+1);
        }
    }
public:
    void insert(T key){
        INSERT(root, key);
    }
    void view(){
        VIEW(root);
    }
    std::vector<int> getHeightStat(){
        std::vector<int> stat(1, 0);
        getStat(stat, root);
        return stat;
    }
};

#endif // AVLTREE_H
