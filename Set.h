#include <bits/stdc++.h>

using namespace std;

template<class ValueType>
class Set {
    struct node {
        ValueType _key;
        node* _left;
        node* _right;
        node* _parent;
        size_t _height;
        size_t _size;

        node(ValueType key, node* parent)
        : _key(key)
        , _left(nullptr)
        , _right(nullptr)
        , _parent(parent)
        , _height(1)
        , _size(1) {}

        node(ValueType key, node* left, node* right, node* parent, size_t height, size_t size)
        : _key(key)
        , _left(left)
        , _right(right)
        , _parent(parent)
        , _height(height)
        , _size(size) {}

        ~node() {}
    };

    node* _tree;

    int height(node* t) {
        if (t) {
            return t->_height;
        }
        return 0;
    }
    int size(node* t) {
        if (t) {
            return t->_size;
        }
        return 0;
    }

    void fix(node *t) {
        t->_height = 1 + max(height(t->_left), height(t->_right));
        t->_size = 1 + (size(t->_left) + size(t->_right));
    }

    int zet(node *t) {
        return height(t->_left) - height(t->_right);
    }

    node* RightRotate(node *t) {
        node* t1 = t->_left;
        t->_left = t1->_right;
        t1->_right = t;

        t1->_parent = t->_parent;
        t->_parent = t1;
        if (t->_left) {
            t->_left->_parent = t;
        }
        fix(t);
        fix(t1);
        return t1;
    }

    node* LeftRotate(node *t) {
        node* t1 = t->_right;
        t->_right = t1->_left;
        t1->_left = t;

        t1->_parent = t->_parent;
        t->_parent = t1;
        if (t->_right) {
            t->_right->_parent = t;
        }
        fix(t);
        fix(t1);
        return t1;
    }

    node* fmin(node* t) {
        if (t->_left) {
            return fmin(t->_left);
        }
        return t;
    }

    node* dmin(node* t) {
        if (!t->_left) {
            return t->_right;
        }
        t->_left = dmin(t->_left);
        if (t->_left) {
            t->_left->_parent = t;
        }
        return restruct(t);
    }


    node* restruct(node* t) {
        fix(t);
        if (zet(t) == 2) {
            if (zet(t->_left) < 0) {
                t->_left = LeftRotate(t->_left);
            }
            return RightRotate(t);
        } else if (zet(t) == -2) {
            if (zet(t->_right) > 0) {
                t->_right = RightRotate(t->_right);
            }
            return LeftRotate(t);
        }
        return t;
    }


    node* insert(ValueType x, node* t, node* prt) {
        if (!t) {
            t = new node(x, prt);
        } else if (x < t->_key) {
            t->_left = insert(x, t->_left, t);
        } else if (t->_key < x) {
            t->_right = insert(x, t->_right, t);
        }
        return restruct(t);
    }

    node* erase(const ValueType &x, node* t) {
        if (!t) {
            return t;
        }

        if (t->_key < x) {
            t->_right = erase(x, t->_right);
        } else if (x < t->_key) {
            t->_left = erase(x, t->_left);
        } else {
            node* l = t->_left;
            node* r = t->_right;
            if (l && r) {
                node* p = t->_parent;
                delete t;
                node* d = fmin(r);
                d->_right = dmin(r);
                d->_left = l;
                d->_parent = p;
                d->_left->_parent = d;
                if (d->_right) {
                    d->_right->_parent = d;
                }
                return restruct(d);
            } else {
                node* td = t;
                if (!r) {
                    t = l;
                } else {
                    t = r;
                }
                if (t) t->_parent = td->_parent;
                delete td;
                return t;
            }
        }
        return restruct(t);

    }
    size_t retsize(node* t) const {
        if (t) {
            return t->_size;
        }
        return 0;
    }

    static node* copynode(node* t) {
        if (t) {
//                cout << "WTF\n";
            node* t1 = copynode(t->_left);
            node* t2 = copynode(t->_right);
            node* nt = new node(t->_key, t1, t2, nullptr, t->_height, t->_size);
            if (nt->_left) {
                nt->_left->_parent = nt;
            }
            if (nt->_right) {
                nt->_right->_parent = nt;
            }
            return nt;
        } else {
            return nullptr;
        }
    }

    void del(node* &t) {
        if (t) {
            del(t->_right);
            del(t->_left);
        }
        delete t;
    }

    void inorder(node* t) {
        if (!t) return;
        inorder(t->_left);
        cout << t->_key;
        if (t->_parent) {
            cout << "|" << t->_parent->_key;
        }
        cout << "    ";
        inorder(t->_right);
    }

public:
    Set() {
        _tree = nullptr;
    }

    template<typename Iter>
    Set(Iter start, Iter finish) {
        _tree = nullptr;
        while (start != finish) {
            insert(*start);
            start++;
        }
    }

    Set(std::initializer_list<ValueType> list) :
        Set(list.begin(), list.end()) {}

    Set(const Set &s) {
        _tree = copynode(s._tree);
    }

    ~Set() {
        del(_tree);
    }

    Set &operator=(const Set &s) {
        node* s2 = copynode(s._tree);
        del(_tree);
        _tree = s2;
        return *this;
    }

    size_t size() const {
        return retsize(_tree);
    }

    bool empty() const {
        return retsize(_tree) == 0;
    }

    void insert(const ValueType &x) {
        _tree = insert(x, _tree, nullptr);
    }

    void erase(const ValueType &x) {
        _tree = erase(x, _tree);
    }

    class Iterator {

    private:
        using value_type = ValueType;
        using difference_type = std::ptrdiff_t;
        using pointer = const ValueType*;
        using reference = const ValueType&;
        using iterator_category = std::forward_iterator_tag;
        node* k;
        node* s;

    public:
        Iterator() {}

        Iterator(node* z, bool b) {
            k = z;
            while (k && k->_right) {
                k = k->_right;
            }

            if (b == 1) {
                s = nullptr;
            } else {
                while (z && z->_left) {
                    z = z->_left;
                }
                s = z;
            }
        }

        Iterator(ValueType x, node* z) { //FIND
            k = z;
            while (k && k->_right) {
                k = k->_right;
            }

            s = z;
            while (s) {
                if (x < s->_key) {
                    s = s->_left;
                } else if (s->_key < x) {
                    s = s->_right;
                } else {
                    break;
                }
            }
        }

        Iterator(const ValueType &x, node* z, bool b) { //LOWER_BOUND
            k = z;
            while (k && k->_right) {
                k = k->_right;
            }

            s = z;
            while (s) {
                if (x < s->_key) {
                    if (!s->_left) break;
                    s = s->_left;
                } else if (s->_key < x) {
                    if (!s->_right) break;
                    s = s->_right;
                } else {
                    break;
                }
            }
            if (s && s->_key < x) {
                ++(*this);
            }
        }


        Iterator &operator++() {
            if (s->_right) {
                s = s->_right;
                while(s->_left) {
                    s = s->_left;
                }
            } else {
                while(s->_parent && s == s->_parent->_right) {
                    s = s->_parent;
                }
                s = s->_parent;
            }
            return *this;
        }

        Iterator operator++(int) {
            auto it2(*this);
            ++(*this);
            return it2;
        }

        Iterator &operator--() {
            if (!s) {
                s = k;
                return *this;
            }
            ValueType m = s->_key;
            node* m1 = s;
            node* ch = s;
            while (s && (m < s->_key || m1 == s)) {
                if (s->_left && s->_left != ch) {
                    s = s->_left;
                    while (s->_right) {
                        s = s->_right;
                    }
                } else {
                    ch = s;
                    s = s->_parent;
                }
            }
            return *this;
        }

        Iterator operator--(int) {
            auto it2(*this);
            --(*this);
            return it2;
        }

        friend bool operator==(const Iterator &lhs, const Iterator &rhs) {
            if (!lhs.s || !rhs.s) {
                if (!lhs.s && !rhs.s) {
                    return 1;
                }
                return 0;
            }
            if (lhs.s->_key < rhs.s->_key) {
                return 0;
            } else if (rhs.s->_key < lhs.s->_key) {
                return 0;
            } else {
                return 1;
            }
        }

        friend bool operator!=(const Iterator &lhs, const Iterator &rhs) {
            return !(lhs == rhs);
        }

        reference operator*() {
            return s->_key;
        }

        pointer operator->() {
            return &(s->_key);
        }

    };

    typedef Iterator iterator;

    iterator begin() const {
        return iterator(_tree, 0);
    }
    iterator end() const {
        return iterator(_tree, 1);
    }

    iterator find(const ValueType &x) const {
        return iterator(x, _tree);
    }

    iterator lower_bound(const ValueType &x) const {
        return iterator(x, _tree, 1);
    }

    void dis() {
        inorder(_tree);
        cout << endl;
    }
};
