#include <iostream>
#include <vector>
#include <utility>
#include <QVariant>
#include <QDebug>

#ifndef TREES_H
#define TREES_H


template<typename T>
class AVLTree {
public:
    struct node {
        T key;
        node *left = nullptr;
        node *right = nullptr;
        int height = 1;

        node() {}

        node(T key) :
                key(key) {}
    };

    node *root = nullptr;
private:
    size_t getHeight(node *p) {
        if (p == nullptr) {
            return 0;
        } else {
            return p->height;
        }
    }

    int bfactor(node *p) {
        return getHeight(p->right) - getHeight(p->left);
    }

    void fixHeight(node *p) {
        p->height = std::max(getHeight(p->left), getHeight(p->right)) + 1;
    }

    void rotateright(node *&p) {
        node *q = p->left;
        p->left = q->right;
        q->right = p;
        fixHeight(p);
        fixHeight(q);
        p = q;
    }

    void rotateleft(node *&q) {
        node *p = q->right;
        q->right = p->left;
        p->left = q;
        fixHeight(q);
        fixHeight(p);
        q = p;
    }

    void balance(node *&p) {
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

    void INSERT(node *&cur, T &key) {
        if (cur == nullptr) {
            cur = new node(key);
            return;
        }
        if (key < cur->key) {
            INSERT(cur->left, key);
        } else if (cur->key < key) {
            INSERT(cur->right, key);
        } else if (key == cur->key) {
            return;
        }
        balance(cur);
    }

    node *findMin(node *p) {
        if (p->left != nullptr) {
            return findMin(p->left);
        } else {
            return p;
        }
    }

    void removeMin(node *&p) {
        if (p->left == nullptr) {
            p = p->right;
            return;
        }
        removeMin(p->left);
        balance(p);
    }

    void REMOVE(node *&cur, T key) {
        if (cur == nullptr) {
            return;
        }
        if (key < cur->key) {
            REMOVE(cur->left, key);
        } else if (key > cur->key) {
            REMOVE(cur->right, key);
        } else {
            node *q = cur->left;
            node *r = cur->right;
            delete cur;
            if (r == nullptr) {
                cur = q;
                return;
            }
            node *min = findMin(r);
            removeMin(r);
            min->right = r;
            min->left = q;
            balance(min);
            cur = min;
            return;
        }
        balance(cur);
    }

    T GETMAX(node *now) {
        if (now == nullptr) {
            return 0;
        }
        if (now->right == nullptr) {
            return now->key;
        }
        return GETMAX(now->right);
    }

    void CLEAR(node *now) {
        if (now == nullptr) {
            return;
        }
        CLEAR(now->left);
        CLEAR(now->right);
        delete now;
    }

public:
    void clear() {
        CLEAR(root);
        root = nullptr;
    }

    void insert(T key) {
        INSERT(root, key);
    }

    T getMax() {
        return GETMAX(root);
    }

    void remove(T key) {
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

        node(T key, node *left, node *right) :
                key(key), left(left), right(right) {}
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

    void splay(node *v) {
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
        if (key == v->key) {
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
        if (root->key == key) {
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

    void REMOVE(node *&v, T key) {
        v = find(v, key);
        if (v == nullptr || v->key != key) {
            return;
        }
        setParent(v->left, nullptr);
        setParent(v->right, nullptr);
        delete v;
        root = v = merge(v->left, v->right);
    }

    void CLEAR(node *now) {
        if (now == nullptr) {
            return;
        }
        CLEAR(now->left);
        CLEAR(now->right);
        delete now;
    }

public:
    void clear() {
        CLEAR(root);
        root = nullptr;
    }

    void insert(T key) {
        INSERT(root, key);
    }

    void remove(T key) {
        REMOVE(root, key);
    }
};

#define NIL &sentinel

template<class T>
class RedBlackTree {
    enum color {
        BLACK, RED
    };
public:

    struct node {
        node *left;
        node *right;
        node *parent;
        color color;
        T key;
    };
    node sentinel = {NIL, NIL, 0, BLACK, 0};

    node *root = NIL;

    void rotateLeft(node *x) {

        node *y = x->right;

        x->right = y->left;
        if (y->left != NIL) y->left->parent = x;

        if (y != NIL) y->parent = x->parent;
        if (x->parent) {
            if (x == x->parent->left)
                x->parent->left = y;
            else
                x->parent->right = y;
        } else {
            root = y;
        }

        y->left = x;
        if (x != NIL) x->parent = y;
    }

    void rotateRight(node *x) {

        node *y = x->left;

        x->left = y->right;
        if (y->right != NIL) y->right->parent = x;

        if (y != NIL) y->parent = x->parent;
        if (x->parent) {
            if (x == x->parent->right)
                x->parent->right = y;
            else
                x->parent->left = y;
        } else {
            root = y;
        }

        y->right = x;
        if (x != NIL) x->parent = y;
    }

    void insertFixup(node *x) {

        while (x != root && x->parent->color == RED) {

            if (x->parent == x->parent->parent->left) {
                node *y = x->parent->parent->right;
                if (y->color == RED) {

                    x->parent->color = BLACK;
                    y->color = BLACK;
                    x->parent->parent->color = RED;
                    x = x->parent->parent;
                } else {

                    if (x == x->parent->right) {
                        x = x->parent;
                        rotateLeft(x);
                    }

                    x->parent->color = BLACK;
                    x->parent->parent->color = RED;
                    rotateRight(x->parent->parent);
                }
            } else {

                node *y = x->parent->parent->left;
                if (y->color == RED) {

                    x->parent->color = BLACK;
                    y->color = BLACK;
                    x->parent->parent->color = RED;
                    x = x->parent->parent;
                } else {

                    if (x == x->parent->left) {
                        x = x->parent;
                        rotateRight(x);
                    }
                    x->parent->color = BLACK;
                    x->parent->parent->color = RED;
                    rotateLeft(x->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    node *insert(T data) {
        node *current, *parent, *x;

        current = root;
        parent = 0;
        while (current != NIL) {
            if (data == current->key) return (current);
            parent = current;
            current = (data < current->key) ?
                      current->left : current->right;
        }

        x = new node();

        x->key = data;
        x->parent = parent;
        x->left = NIL;
        x->right = NIL;
        x->color = RED;

        if (parent) {
            if (data < parent->key)
                parent->left = x;
            else
                parent->right = x;
        } else {
            root = x;
        }

        insertFixup(x);
        return (x);
    }

    void deleteFixup(node *x) {

        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                node *w = x->parent->right;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rotateLeft(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == BLACK && w->right->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->right->color == BLACK) {
                        w->left->color = BLACK;
                        w->color = RED;
                        rotateRight(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    rotateLeft(x->parent);
                    x = root;
                }
            } else {
                node *w = x->parent->left;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rotateRight(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == BLACK && w->left->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->left->color == BLACK) {
                        w->right->color = BLACK;
                        w->color = RED;
                        rotateLeft(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rotateRight(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    void deleteNode(node *z) {
        node *x, *y;

        if (!z || z == NIL) return;


        if (z->left == NIL || z->right == NIL) {
            y = z;
        } else {
            y = z->right;
            while (y->left != NIL) y = y->left;
        }

        if (y->left != NIL)
            x = y->left;
        else
            x = y->right;

        x->parent = y->parent;
        if (y->parent)
            if (y == y->parent->left)
                y->parent->left = x;
            else
                y->parent->right = x;
        else
            root = x;

        if (y != z) z->key = y->key;


        if (y->color == BLACK)
            deleteFixup(x);

        free(y);
    }

    node *findNode(T data) {

        node *current = root;
        while (current != NIL)
            if (data == current->key)
                return (current);
            else
                current = (data < current->key) ?
                          current->left : current->right;
        return nullptr;
    }

    void remove(T key) {
        auto now = findNode(key);
        if (now != nullptr) {
            deleteNode(now);
        }
    }

    void CLEAR(node *now) {
        if (now == nullptr) {
            return;
        }
        if (now->left == now && now->right == now) {
            return;
        }
        CLEAR(now->left);
        CLEAR(now->right);
        delete now;
    }

    void clear() {
        CLEAR(root);
        root = NIL;
    }
};

#undef NIL


template<typename T>
int getSize(RedBlackTree<int>::node *now) {
    if (now == now->left && now == now->right) {
        return 0;
    }
    return getSize<RedBlackTree<int>::node *>(now->left) + getSize<RedBlackTree<int>::node *>(now->right) + 1;
}

template<typename T>
int getSize(T now) {
    if (now == nullptr) {
        return 0;
    }
    return getSize(now->left) + getSize(now->right) + 1;
}


#endif // TREES_H
