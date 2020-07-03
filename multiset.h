#ifndef MULTImultiset_H
#define MULTImultiset_H

#include "List.h"
#include "Set.h"

namespace ADT {

    template<typename T>
    struct MultiSetNode {
        List<T> data;
        MultiSetNode *left;
        MultiSetNode *right;
        MultiSetNode *parent;
        int8_t color;

        MultiSetNode();
        explicit MultiSetNode(const T& data);
        explicit MultiSetNode(T&& data);
        explicit MultiSetNode(const MultiSetNode& node) = default;
        explicit MultiSetNode(MultiSetNode&& node) = default;

        MultiSetNode& operator=(MultiSetNode&& node) = default;

        List<T>& get_data();
    };

    template<typename T, class Less = std::less<T>>
    class MultiSet {
    private:
        MultiSetNode<T> *root;
        MultiSetNode<T> *endNode;
        Less less;
        size_t _size;
        void rotate_left(MultiSetNode<T> *);
        void rotate_right(MultiSetNode<T> *);
        MultiSetNode<T>* bst_insert(bool&, const T&);
        MultiSetNode<T>* bst_insert(bool&, T&&);
        MultiSetNode<T>* _copy_tree(MultiSetNode<T>*);
        inline bool _is_equal_key(const T& nodeval, const T& val);
        MultiSetNode<T>* _find_bound(MultiSetNode<T> *_root, const T &val);
        void restore_balance(MultiSetNode<T>*, int8_t type);
        void tree_destroy(MultiSetNode<T> *);
    public:
        //TODO: implement const version of this class (const_iterators etc)
        MultiSet() noexcept;
        MultiSet(const MultiSet& rhs) noexcept;
        MultiSet(MultiSet&& rhs) noexcept;
        ~MultiSet();
        MultiSet& operator=(MultiSet x);

        bool add(const T& data);
        bool add(T&& data);
        void clear() noexcept;
        void clear() const noexcept;
        bool empty();

        size_t size();
        MultiSetNode<T>* end();
        MultiSetNode<T>* begin();

        MultiSetNode<T>* search(const T& key);
        MultiSetNode<T>* lower_bound(const T& val);
        MultiSetNode<T>* upper_bound(const T& val);
        friend void swap(MultiSet& lhs, MultiSet& rhs) {
            using std::swap;

            swap(lhs.root, rhs.root);
            swap(lhs.endNode, rhs.endNode);
            swap(lhs.less, rhs.less);
            swap(lhs._size, rhs._size);
        }

        MultiSetNode<T>* _successor(MultiSetNode<T>*);
    };

#define multiset_parent_of(node) _multiset_parent_of(node)

    template<typename T>
    static inline MultiSetNode<T>* _multiset_parent_of(MultiSetNode<T>* node) {
        return node ? node->parent : nullptr;
    }

#define multiset_grandparent_of(node) _multiset_grandparent_of(node)

    template<typename T>
    static inline MultiSetNode<T>* _multiset_grandparent_of(MultiSetNode<T>* node) {
        if (node) {
            if (node->parent) {
                return node->parent->parent;
            }
        }
        return nullptr;
    }

    template<typename T>
    static inline MultiSetNode<T>* multiset_left_of(MultiSetNode<T> *node) {
        return node ? node->left : nullptr;
    }

    template<typename T>
    static inline MultiSetNode<T>* multiset_right_of(MultiSetNode<T> *node) {
        return node ? node->right : nullptr;
    }

#define multiset_assign_color(node, color) _multiset_multiset_color(node, color)

    template<typename T>
    static inline void _multiset_multiset_color(MultiSetNode<T> *node, char color) {
        if (node) {
            node->color = color;
        }
    }

#define multiset_color_of(node) _multiset_color_of(node)

    template<typename T>
    static inline char _multiset_color_of(MultiSetNode<T> *node) {
        return node ? node->color : (int8_t) BLACK;
    }

#define multiset_balance_insertion(side, rotate_1, rotate_2)             \
    do  {                                                           \
            {                                                       \
                MultiSetNode<T> *uncle = side(multiset_grandparent_of(node)); \
                                                                    \
                if (multiset_color_of(uncle) == RED) {                   \
                    /*Uncle RED means color-flip*/                  \
                                                                    \
                    multiset_assign_color(multiset_parent_of(node), BLACK);   \
                    multiset_assign_color(multiset_grandparent_of(node), RED);\
                    multiset_assign_color(uncle, BLACK);                 \
                    node = multiset_grandparent_of(node);                \
                }                                                   \
                else {                                              \
                    /*Uncle BLACK means rotations*/                 \
                    if (node == side(multiset_parent_of(node))) {        \
                        node = multiset_parent_of(node);                 \
                        rotate_2(node);                             \
                    }                                               \
                    multiset_assign_color(multiset_parent_of(node), BLACK);   \
                    multiset_assign_color(multiset_grandparent_of(node), RED);\
                    rotate_1(multiset_grandparent_of(node));             \
                }                                                   \
            }                                                       \
    } while(0)

#define multiset_balance_deletion(side1, side2, rotate_side1, rotate_side2)   \
    do {                                                                 \
            {                                                            \
                MultiSetNode<T>* sibling = side1(multiset_parent_of(node));        \
                if (multiset_color_of(sibling) == RED) {                      \
                    multiset_assign_color(sibling, BLACK);                    \
                    multiset_assign_color(multiset_parent_of(node), RED);          \
                    rotate_side1(multiset_parent_of(node));                   \
                    sibling = side1(multiset_parent_of(node));                \
                }                                                        \
                if (multiset_color_of(side2(sibling)) == BLACK && multiset_color_of(side1(sibling)) == BLACK) { \
                    multiset_assign_color(sibling, RED);                      \
                    node = multiset_parent_of(node);                          \
                }                                                        \
                else {                                                   \
                    if (multiset_color_of(side1(sibling)) == BLACK) {         \
                        multiset_assign_color(side2(sibling), BLACK);         \
                        multiset_assign_color(sibling, RED);                  \
                        rotate_side2(sibling);                           \
                        sibling = side1(multiset_parent_of(node));            \
                    }                                                    \
                    multiset_assign_color(sibling, multiset_color_of(multiset_parent_of(node)));\
                    multiset_assign_color(multiset_parent_of(node), BLACK);        \
                    multiset_assign_color(side1(sibling), BLACK);             \
                    rotate_side1(multiset_parent_of(node));                   \
                    node = this->root;                                   \
                }                                                        \
            }                                                            \
    } while(0)

}

/*Implementation*/
using namespace ADT;

template<typename T, class Less>
MultiSet<T, Less>::MultiSet() noexcept : root(nullptr), endNode(nullptr) , _size(0) {}

template<typename T, class Less>
MultiSetNode<T>* MultiSet<T, Less>::_copy_tree(MultiSetNode<T>* other_root) {
    if (other_root == nullptr) {
        return nullptr;
    }

    MultiSetNode<T>* new_node = new MultiSetNode<T>(other_root->data);

    new_node->left = _copy_tree(other_root->left);
    if (new_node->left) {
        new_node->left->parent = new_node;
    }

    new_node->right = _copy_tree(other_root->right);
    if (new_node->right) {
        new_node->right->parent = new_node;
    }

    return new_node;
}

template<typename T, class Less>
MultiSet<T, Less>::MultiSet(const MultiSet& other) noexcept {
    /*Create an exact copy of this MultiSet, O(n)*/
    this->root = _copy_tree(other.root);
    this->endNode = new MultiSetNode<T>();
    this->endNode->left = this->root;
    this->root->parent = this->endNode;
}

template<typename T, class Less>
MultiSet<T, Less>::MultiSet(MultiSet&& other) noexcept : MultiSet() {
    swap(*this, other);
}

template<typename T, class Less>
MultiSet<T, Less>& MultiSet<T, Less>::operator=(MultiSet other) {
    /*Copy and swap idiom, let the compiler handle the copy of the argument*/
    swap(*this, other);

    return *this;
}

template<typename T, class Less>
void MultiSet<T, Less>::tree_destroy(MultiSetNode<T> *current) {

    if (!current) {
        return;
    }

    tree_destroy(current->left);
    tree_destroy(current->right);

    delete current;
}

template<typename T, class Less>
MultiSet<T, Less>::~MultiSet() {
    if (this->root) {
        tree_destroy(this->root);
        delete this->endNode;
    }
}

template<typename T, class Less>
MultiSetNode<T> *MultiSet<T, Less>::bst_insert(bool &added_new, const T &data) {

    if (empty()) {
        added_new = true;
        this->root = new MultiSetNode<T>(data);
        this->endNode = new MultiSetNode<T>();
        this->endNode->left = this->root;
        this->root->parent = this->endNode;
        return this->root;
    }

    MultiSetNode<T> *current = this->root;
    while (1) {

        if (less(data, current->data[0])) {
            if (current->left != nullptr) {
                current = current->left;
            }
            else {
                added_new = true;
                MultiSetNode<T> *new_node = new MultiSetNode<T>(data);
                current->left = new_node;
                new_node->parent = current;

                return new_node;
            }
        }
        else if (less(current->data[0], data)) {
            if (current->right != nullptr) {
                current = current->right;
            }
            else {
                added_new = true;
                MultiSetNode<T> *new_node = new MultiSetNode<T>(data);
                current->right = new_node;
                new_node->parent = current;

                return new_node;
            }
        }
        else {
            added_new = false;
            current->data.push_back(data);
            return current;
        }
    }
}

template<typename T, class Less>
MultiSetNode<T> *MultiSet<T, Less>::bst_insert(bool &added_new, T &&data) {

    if (empty()) {
        added_new = true;
        this->root = new MultiSetNode<T>(std::forward<T>(data));
        this->endNode = new MultiSetNode<T>();
        this->endNode->left = this->root;
        this->root->parent = this->endNode;
        return this->root;
    }

    MultiSetNode<T> *current = this->root;
    while (1) {

        if (less(data, current->data[0])) {
            if (current->left != nullptr) {
                current = current->left;
            }
            else {
                added_new = true;
                MultiSetNode<T> *new_node = new MultiSetNode<T>(std::forward<T>(data));
                current->left = new_node;
                new_node->parent = current;

                return new_node;
            }
        }
        else if (less(current->data[0], data)) {
            if (current->right != nullptr) {
                current = current->right;
            }
            else {
                added_new = true;
                MultiSetNode<T> *new_node = new MultiSetNode<T>(std::forward<T>(data));
                current->right = new_node;
                new_node->parent = current;

                return new_node;
            }
        }
        else {
            added_new = false;
            current->data.push_back(data);
            return current;
        }
    }
}

template<typename T, class Less>
MultiSetNode<T>* MultiSet<T, Less>::_successor(MultiSetNode<T> *node) {
    if (node == nullptr) {
        return nullptr;
    }
    MultiSetNode<T>* current;
    if (node->right) {
        current = node->right;
        while (current->left != nullptr) {
            current = current->left;
        }
    }
    else {
        current = node->parent;
        while (current != nullptr && node == current->right) {
            node = current;
            current = current->parent;
        }
    }

    return current;
}


template<typename T, class Less>
MultiSetNode<T>* MultiSet<T, Less>::begin() {
    MultiSetNode<T> *current = this->root;
    if (current == nullptr) {
        return nullptr;
    }
    while (current->left != nullptr) {
        current = current->left;
    }

    return current;
}

template<typename T, class Less>
MultiSetNode<T>* MultiSet<T, Less>::end() {
    if (this->root == nullptr) {
        return nullptr;
    }
    return this->endNode;
}

template<typename T, class Less>
bool MultiSet<T, Less>::add(const T &data) {

    bool added_new;
    MultiSetNode<T> *current = bst_insert(added_new, data);

    if (!added_new) {
        return false;
    }

    restore_balance(current,INSERTION);

    this->root->color = BLACK;
    this->_size++;
    this->endNode->left = this->root;
    this->root->parent = this->endNode;

    return true;
}

template<typename T, class Less>
bool MultiSet<T, Less>::add(T &&data) {

    bool added_new;
    MultiSetNode<T> *current = bst_insert(added_new, std::forward<T>(data));

    if (!added_new) {
        return false;
    }

    restore_balance(current,INSERTION);

    this->root->color = BLACK;
    this->_size++;
    this->root->parent = this->endNode;
    this->endNode->left = this->root;

    return true;
}

template<typename T, class Less>
void MultiSet<T, Less>::clear() noexcept {
    if (this->root) {
        tree_destroy(this->root);
        delete this->endNode;
        this->root = nullptr;
        this->endNode = nullptr;
        this->_size = 0;
    }
}

template<typename T, class Less>
void MultiSet<T, Less>::clear() const noexcept {
    if (this->root) {
        tree_destroy(this->root);
        delete this->endNode;
        this->root = nullptr;
        this->endNode = nullptr;
        this->_size = 0;
    }
}

template<typename T, class Less>
bool MultiSet<T, Less>::empty() {
    return this->root == nullptr;
}


template<typename T, class Less>
void MultiSet<T, Less>::restore_balance(MultiSetNode<T>* node, int8_t type) {

    if (type == DELETION) {
        while (node != this->root && multiset_color_of(node) == BLACK) {
            if (node == multiset_left_of(multiset_parent_of(node))) {
                multiset_balance_deletion(multiset_right_of, multiset_left_of, rotate_left, rotate_right);
            }
            else {
                multiset_balance_deletion(multiset_left_of, multiset_right_of, rotate_right, rotate_left);
            }
        }

        if (node != nullptr && multiset_color_of(node) != BLACK) {
            multiset_assign_color(node, BLACK);
        }
    }
    else if (type == INSERTION) {
        node->color = RED;
        this->root->parent = nullptr;

        while (node != endNode && node != this->root) {
            if (node->parent->color != RED) {
                break;
            }

            if (multiset_parent_of(node) == multiset_left_of(multiset_grandparent_of(node))) {
                multiset_balance_insertion(multiset_right_of, rotate_right, rotate_left);
            }
            else {
                multiset_balance_insertion(multiset_left_of, rotate_left, rotate_right);
            }
        }
    }
    else {
        throw std::invalid_argument("This should not have happened, pls debug me :)");
    }
}


template<typename T, class Less>
inline bool MultiSet<T, Less>::_is_equal_key(const T& nodeval, const T& val) {
    if (less(val, nodeval)) {
        return false;
    }
    else if (less(nodeval, val)) {
        return false;
    }
    return true;
}

template<typename T, class Less>
MultiSetNode<T>* MultiSet<T, Less>::_find_bound(MultiSetNode<T> *_root, const T &val) {
    if (_root == nullptr) {
        return nullptr;
    }

    if (_is_equal_key(_root->data[0], val)) {
        return _root;
    }

    if (less(_root->data[0], val)) {
        return _find_bound(_root->right, val);
    }

    MultiSetNode<T>* retnode = _find_bound(_root->left, val);
    return retnode == nullptr ? _root : retnode;
}

template<typename T, class Less>
MultiSetNode<T>* MultiSet<T, Less>::upper_bound(const T& val) {
    MultiSetNode<T>* retnode = _find_bound(this->root, val);
    if (retnode == nullptr) {
        return this->end();
    }
    else {
        if (_is_equal_key(retnode->data[0], val)) {
            return _successor(retnode);
        }
        else {
            return retnode;
        }
    }
}

template<typename T, class Less>
MultiSetNode<T>* MultiSet<T, Less>::lower_bound(const T& val) {
    MultiSetNode<T> *retnode = _find_bound(this->root, val);
    if (retnode == nullptr) {
        return this->end();
    }
    else {
        return retnode;
    }
}

template<typename T, class Less>
MultiSetNode<T>* MultiSet<T, Less>::search(const T &val) {

    if (this->root == nullptr) {
        return this->end();
    }
    MultiSetNode<T> *current = this->root;
    while (current) {
        if (less(val, current->data[0])) {
            current = current->left;
        }
        else if (less(current->data[0], val)) {
            current = current->right;
        }
        else {
            return current;
        }
    }

    return this->end();
}

template<typename T, class Less>
void MultiSet<T, Less>::rotate_right(MultiSetNode<T> *node) {

    if (!node) {
        return;
    }

    MultiSetNode<T> *left_child = node->left;
    MultiSetNode<T> *left_right_child = left_child->right;

    node->left = left_right_child;
    if (left_right_child) {
        left_right_child->parent = node;
    }

    MultiSetNode<T> *node_p = node->parent;
    left_child->parent = node_p;

    if (node_p == nullptr) {
        this->root = left_child;
    }
    else if (node_p->right == node) {
        node_p->right = left_child;
    }
    else {
        node_p->left = left_child;
    }

    left_child->right = node;
    node->parent = left_child;
}

template<typename T, class Less>
void MultiSet<T, Less>::rotate_left(MultiSetNode<T> *node) {

    if (!node) {
        return;
    }

    MultiSetNode<T> *right_child = node->right;
    MultiSetNode<T> *right_left_child = right_child->left;

    node->right = right_left_child;
    if (right_left_child) {
        right_left_child->parent = node;
    }

    MultiSetNode<T>  *node_p = node->parent;
    right_child->parent = node_p;

    if (node_p == nullptr) {
        this->root = right_child;
    }
    else if (node_p->left == node) {
        node_p->left = right_child;
    }
    else {
        node_p->right = right_child;
    }

    right_child->left = node;
    node->parent = right_child;
}

template<typename T, class Less>
size_t MultiSet<T, Less>::size() {
    return this->_size;
}

template<typename T>
MultiSetNode<T>::MultiSetNode() : data(), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}

template<typename T>
MultiSetNode<T>::MultiSetNode(const T &val) : data(), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
    data.push_back(val);
}

template<typename T>
MultiSetNode<T>::MultiSetNode(T &&val) : data(), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
    data.push_back(val);
}

template<typename T>
List<T> &MultiSetNode<T>::get_data() {
    return this->data;
}

#endif
