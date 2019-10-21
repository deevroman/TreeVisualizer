#include <iostream>
#include <vector>
#include <utility>
#include <QVariant>
#include <QDebug>

#ifndef TREES_H
#define TREES_H


template<typename T>
class AVLTree{
public:
    struct node{
        T key;
        node* left = nullptr;
        node* right = nullptr;
        int height = 1;
        node(){}
        node(T key):
            key(key){}
    };
    node *root = nullptr;
private:
    size_t getHeight(node* p){
        if(p == nullptr){
            return 0;
        } else {
            return p->height;
        }
    }
    int bfactor(node* p){
        return getHeight(p->right) - getHeight(p->left);
    }
    void fixHeight(node* p){
        p->height = std::max(getHeight(p->left), getHeight(p->right)) + 1;
    }
    void rotateright(node*& p){
        node* q = p->left;
        p->left = q->right;
        q->right = p;
        fixHeight(p);
        fixHeight(q);
        p = q;
    }
    void rotateleft(node*& q){
        node* p = q->right;
        q->right = p->left;
        p->left = q;
        fixHeight(q);
        fixHeight(p);
        q = p;
    }
    void balance(node*& p) {
        fixHeight(p);
        if (bfactor(p) == 2) {
            if (bfactor(p->right) < 0) {
                rotateright(p->right);
            }
            rotateleft(p);
        } else if (bfactor(p) == -2) {
            if (bfactor(p->left) > 0) {
                rotateleft(p->left);
            }
            rotateright(p);
        }
    }
    void INSERT(node*& cur, T& key){
        if(cur == nullptr){
            cur = new node(key);
            return;
        }
        if(key < cur->key){
            INSERT(cur->left, key);
        } else if(cur->key < key){
            INSERT(cur->right, key);
        } else if(key == cur->key){
            return;
        }
        balance(cur);
    }
    node* findMin(node* p){
        if (p->left != nullptr) {
            return findMin(p->left);
        } else {
            return p;
        }
    }
    void removeMin(node*& p){
        if( p->left == nullptr ) {
            p = p->right;
            return;
        }
        removeMin(p->left);
        balance(p);
    }
    void REMOVE(node*& cur, T key){
        if(cur == nullptr){
            return;
        }
        if( key < cur->key ) {
            REMOVE(cur->left, key);
        }
        else if( key > cur->key ) {
            REMOVE(cur->right, key);
        } else {
            node* q = cur->left;
            node* r = cur->right;
            delete cur;
            if( r == nullptr ) {
                cur = q;
                return;
            }
            node* min = findMin(r);
            removeMin(r);
            min->right = r;
            min->left = q;
            balance(min);
            cur = min;
            return;
        }
        balance(cur);
    }
public:
    void insert(T key){
        INSERT(root, key);
    }
    void remove(T key){
        REMOVE(root, key);
    }
};

template<typename T>
class SplayTree {
public:
    struct node {
        T key;
        node *left = nullptr;
        node *right = nullptr;
        node *parent = nullptr;

        node() {}

        node(T key, node* left, node* right) :
                key(key), left(left), right(right){}
    };

    node *root = nullptr;
private:
    void setParent(node *child, node *parent) {
        if (child != nullptr) {
            child->parent = parent;
        }
    }

    void keepParent(node *v) {
        setParent(v->left, v);
        setParent(v->right, v);
    }

    void rotate(node *parent, node *child) {
        node *gparent = parent->parent;
        if (gparent != nullptr) {
            if (gparent->left == parent) {
                gparent->left = child;
            } else {
                gparent->right = child;
            }
        }
        if (parent->left == child) {
            parent->left = child->right;
            child->right = parent;
        } else {
            parent->right = child->left;
            child->left = parent;
        }
        keepParent(child);
        keepParent(parent);
        child->parent = gparent;
    }

    void splay(node* v) {
        if (v->parent == nullptr) {
            return;
        }
        node *parent = v->parent;
        node *gparent = parent->parent;
        if (gparent == nullptr) {
            rotate(parent, v);
        } else {
            bool zigzig = (gparent->left == parent) == (parent->left == v);
            if (zigzig) {
                rotate(gparent, parent);
                rotate(parent, v);
            } else {
                rotate(parent, v);
                rotate(gparent, v);
            }
            splay(v);
        }
    }

    node *find(node *v, T key) {
        if (v == nullptr) {
            return nullptr;
        }
        if(key == v->key){
            splay(v);
            return v;
        }
        if (key < v->key && v->left != nullptr) {
            return find(v->left, key);
        }
        if (key > v->key && v->right != nullptr) {
            return find(v->right, key);
        }
        splay(v);
        return v;
    }
    std::pair<node *, node *> split(T key) {
        if (root == nullptr) {
            return {nullptr, nullptr};
        }
        root = find(root, key);
        if (root->key == key){
            setParent(root->left, nullptr);
            setParent(root->right, nullptr);
            return {root->left, root->right};
        }
        if (root->key < key) {
            node *right = root->right;
            root->right = nullptr;
            setParent(right, nullptr);
            return {root, right};
        } else {
            node *left = root->left;
            root->left = nullptr;
            setParent(left, nullptr);
            return {left, root};
        }
    }

    void INSERT(node *&v, T key) {
        auto p = split(key);
        v = new node(key, p.first, p.second);
        keepParent(v);
        root = v;
    }

    node *merge(node *left, node *right) {
        if (right == nullptr) {
            return left;
        }
        if (left == nullptr) {
            return right;
        }
        right = find(right, left->key);
        right->left = left;
        left->parent = right;
        return right;
    }
    void REMOVE(node*& v, T key){
        v = find(v, key);
        if(v == nullptr || v-> key != key){
            return;
        }
        setParent(v->left, nullptr);
        setParent(v->right, nullptr);
        delete v;
        root = v = merge(v->left, v->right);
    }
public:
    void insert(T key){
        INSERT(root, key);
    }
    void remove(T key){
        REMOVE(root, key);
    }
};

template <class T>
class RedBlackTree{
public:
    struct node{
        T getkey(){
            return key;
        }
        T key;
        node *left;
        node *right;
        node *p;
        bool color;
    };
public:
    RedBlackTree(){
        nil = new node();
        root = nil;
        nil->left = 0;
        nil->p = 0;
        nil->right = 0;
        nil->color = false;
    }

private:

    void leftRotate(node *x ){		// x->right != nil
        if(x->right->left == nullptr && x->right->right == nullptr){
            qDebug() << "должен быть краш";
            return;
        }
        node* y = x->right;
        if( x->p == nil )
            root = y;
        else{
            if( x == x->p->left )
                x->p->left = y;
            else
                x->p->right = y;
        }
        y->p = x->p;
        x->right = y->left;
        y->left->p = x;
        y->left = x;
        x->p = y;
    }

    void rightRotate(node *x ){
        if(x->left->left == nullptr && x->left->right == nullptr){
            qDebug() << "должен быть краш";
            return;
        }
        node * y = x->left;
        if( x->p == nil )
            root = y;
        else{
            if( x->p->left == x )
                x->p->left = y;
            else
                x->p->right = y;
        }
        y->p = x->p;
        x->left = y->right;
        y->right->p = x;
        y->right = x;
        x->p = y;
    }

    void rbDelete(node *z ){
        node * x = nil;
        node * y = nil;
        if( z->left == nil || z->right == nil )
            y = z;
        else
            y = treeSuccessor( z );
        if( y->left != nil )
            x = y->left;
        else
            x = y->right;
        x->p = y->p;
        if( y->p == nil )
            root = x;
        else{
            if( y == y->p->left )
                y->p->left = x;
            else
                y->p->right = x;
        }
        if( y != z )
            z->key = y->key;
        if( y->color == false )
            rbDeleteFixup( x );
        delete y;
    }

    void rbDeleteFixup(node * x){
        while( x != root && x->color == false ){
            node * w = 0;
            if( x->p->left == x ){
                w = x->p->right;
                if( w->color == true ){
                    w->color = false;
                    x->p->color = true;
                    leftRotate( x->p );
                    w = x->p->right;
                }
                if( w->left->color == false && w->right->color == false ){
                    w->color = true;
                    x = x->p;
                }
                else{
                    if( w->right->color == false ){
                        w->left->color = false;
                        w->color = true;
                        rightRotate( w );
                        w = x->p->right;
                    }
                    w->color = x->p->color;
                    x->p->color = false;
                    w->right->color = false;
                    leftRotate(x->p);
                    x = root;
                }
            }
            else{
                w = x->p->left;
                if( w->color == true ){
                    w->color = false;
                    x->p->color = true;
                    rightRotate( x->p );
                    w = x->p->left;
                }
                if( w->right->color == false && w->left->color == false ){
                    w->color = true;
                    x = x->p;
                }
                else{
                    if( w->left->color == false ){
                        w->right->color = false;
                        w->color = true;
                        leftRotate( w );
                        w = x->p->left;
                    }
                    w->color = x->p->color;
                    x->p->color = false;
                    w->left->color = false;
                    rightRotate(x->p);
                    x = root;
                }
            }
        }
        x->color = false;
    }

    void rbInsertFixup(node *z ){
        while( z->p->color  == true ){
            if( z->p == z->p->p->left ){
                node * y = z->p->p->right;
                if( y->color == true ){
                    z->p->color = false;
                    y->color = false;
                    z->p->p->color = true;
                    z = z->p->p;
                }
                else{
                    if( z == z->p->right ){
                        z = z->p;
                        leftRotate( z );
                    }//
                    z->p->color = false;
                    z->p->p->color = true;
                    z->p->right->color = false;
                    rightRotate( z->p->p );
                }
            }
            else{
                node * y = z->p->p->left;
                if( y->color == true ){
                    z->p->color = false;
                    y->color = false;
                    z->p->p->color = true;
                    z = z->p->p;
                }
                else{
                    if( z == z->p->left ){
                        z = z->p;
                        rightRotate( z );
                    }
                    z->p->color = false;
                    z->p->p->color = true;
                    z->p->left->color = false;
                    leftRotate( z->p->p );
                }
            }
        }
        root->color = false;
    }

    node * treeSuccessor(node *x ){
        if( x->right != nil ){
            while( x->left != nil )
                x = x->left;
            return x;
        }
        node * y = x->p;
        while( y != nil && x == y->right ){
            x = y;
            y = y->p;
        }
        return y;
    }

    node *nil;
public:
    node *root;
    void insert( T key ){
        node* t = new node();
        t->key = key;
        node* x = root;
        node* y = nil;
        while( x != nil ){
            y = x;
            if( key < x->key )
                x = x->left;
            else
                x = x->right;
        }
        t->p = y;
        if( y == nil )
            root = t;
        else{
            if ( t->key < y->key )
                y->left = t;
            else
                y->right = t;
        }
        t->left = nil;
        t->right = nil;
        t->color = true;
        rbInsertFixup( t );
    }
    node * search(T key ){
        node * x = root;
        while( x != nil && key != x->key ){
            if( key < x->key )
                x = x->left;
            else
                x = x->right;
        }
        return x;
    }

    void remove(T key ){
        node * x = search(key );
        if( x != nil )
            rbDelete( x );
    }

};


template<typename T>
std::vector<int> getHeightStat(T cur){
    std::vector<int> stat(1, 0);
    getStat(stat, cur);
    return stat;
}

template<typename T>
void getStat(std::vector<int> &stat, T cur, size_t h = 0){
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



template <typename T>
int getSize(RedBlackTree<int>::node* now){
    if(now->left == nullptr && now->right == nullptr){
        return 0;
    }
    return getSize<RedBlackTree<int>::node*>(now->left) + getSize<RedBlackTree<int>::node*>(now->right) + 1;
}

template<typename T>
int getSize(T now){
    if(now == nullptr){
        return 0;
    }
    return getSize(now->left) + getSize(now->right) + 1;
}



#endif // TREES_H
