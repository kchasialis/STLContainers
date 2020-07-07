#pragma once

#include <functional>
#include <utility>
#include <cstddef>
#include <iterator>
#include <cstdint>
#include <iostream>

namespace adt {

    template<typename T>
    struct SetNode {
        enum {
            RED,
            BLACK
        };

        T data;
        SetNode *left;
        SetNode *right;
        SetNode *parent;
        int8_t color;

        SetNode();
        explicit SetNode(const T& data);
        explicit SetNode(T&& data);
        explicit SetNode(const SetNode& node) = default;
        explicit SetNode(SetNode&& node) = default;

        SetNode& operator=(SetNode&& node) = default;
    };

    template<typename T>
    class set_iterator : public std::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t , T*, T&> {
    public:
        using const_data_reference = const T&;
        using const_data_pointer = const T*;
        
        set_iterator(SetNode<T>* rhs = nullptr) : ptr(rhs) {}
        set_iterator(const set_iterator<T>& rhs) = default;
        set_iterator<T>& operator=(const set_iterator<T>& rhs) = default;
        set_iterator<T>& operator=(SetNode<T>* rhs) {
            this->ptr = rhs;
            return *this;
        }

        bool operator==(const set_iterator<T>& rhs) const {
            return this->ptr == rhs.ptr;
        }
        bool operator!=(const set_iterator<T>& rhs) const {
            return !(*this == rhs);
        }

        const_data_reference operator*() {
            return this->ptr->data;
        }
        const_data_pointer operator->() {
            return &(this->ptr->data);
        }

        /* Inorder successor algorithm. */
        set_iterator<T>& operator++() {
            if (this->ptr == nullptr) {
                return *this;
            }
            SetNode<T>* current;
            if (this->ptr->right) {
                current = this->ptr->right;
                while (current->left != nullptr) {
                    current = current->left;
                }
            }
            else {
                current = this->ptr->parent;
                while (current != nullptr && this->ptr == current->right) {
                    this->ptr = current;
                    current = current->parent;
                }
            }

            this->ptr = current;
            return *this;
        }
        set_iterator<T> operator++(int) {
            set_iterator<T> tmp(*this);
            ++(*this);

            return tmp;
        }
        
        /* Inorder predecessor algorithm. */
        set_iterator<T>& operator--() {
            if (this->ptr == nullptr) {
                return *this;
            }

            SetNode<T>* current;
            if (this->ptr->left) {
                current = this->ptr->left;
                while (current->right != nullptr) {
                    current = current->right;
                }
            }
            else {
                current = this->ptr->parent;
                while (current != nullptr && this->ptr == current->left) {
                    this->ptr = current;
                    current = current->parent;
                }
            }

            this->ptr = current;
            return *this;
        }
        set_iterator<T> operator--(int) {
            set_iterator<T> tmp(*this);
            --(*this);

            return tmp;    
        }
        
        void swap(set_iterator<T>& lhs, set_iterator<T>& rhs) {
            std::swap(lhs, rhs);
        }

        SetNode<T>* get_ptr() {
            return this->ptr;
        }

        const SetNode<T>* get_ptr() const {
            return this->ptr;
        }

    private:
        SetNode<T>* ptr;
    };

    template<typename T, class Less = std::less<T>>
    class set {
    private:
        enum {
            RED,
            BLACK
        };
        enum SetBalanceType {
            DELETION,
            INSERTION
        };

        SetNode<T> *root;
        SetNode<T> *endNode;
        Less less;
        size_t _size;
        void rotate_left(SetNode<T> *);
        void rotate_right(SetNode<T> *);
        SetNode<T>* bst_insert(bool&, const T&);
        SetNode<T>* bst_insert(bool&, T&&);
        SetNode<T>* _copy_tree(SetNode<T>*);
        SetNode<T>* _remove(SetNode<T> *, SetNode<T>*);
        inline bool _is_equal_key(const T& nodeval, const T& val);
        SetNode<T>* _successor(SetNode<T>*);
        SetNode<T>* _find_bound(SetNode<T> *_root, const T &val);
        void restore_balance(SetNode<T>*, int8_t type);
        void tree_destroy(SetNode<T> *);
    public:
        set() noexcept;
        set(const set& rhs) noexcept;
        set(set&& rhs) noexcept;
        ~set();

        set& operator=(set x);

        bool add(const T& data);
        bool add(T&& data);
        bool remove(const T& key);
        set_iterator<T> remove(set_iterator<T> itr);
        void clear() noexcept;
        void clear() const noexcept;

        bool empty();
        size_t size();

        set_iterator<T> begin();
        set_iterator<T> end();
        set_iterator<T> search(const T& key);
        set_iterator<T> lower_bound(const T& val);
        set_iterator<T> upper_bound(const T& val);

        friend void swap(set& lhs, set& rhs) {
            using std::swap;

            swap(lhs.root, rhs.root);
            swap(lhs.endNode, rhs.endNode);
            swap(lhs.less, rhs.less);
            swap(lhs._size, rhs._size);
        }
    };

    /*Implementation*/
    #define set_parent_of(node) _set_parent_of(node)

    template<typename T>
    static inline SetNode<T>* _set_parent_of(SetNode<T>* node) {
        return node ? node->parent : nullptr;
    }

    #define set_grandparent_of(node) _set_grandparent_of(node)

    template<typename T>
    static inline SetNode<T>* _set_grandparent_of(SetNode<T>* node) {
        if (node) {
            if (node->parent) {
                return node->parent->parent;
            }
        }
        return nullptr;
    }

    template<typename T>
    static inline SetNode<T>* set_left_of(SetNode<T> *node) {
        return node ? node->left : nullptr;
    }

    template<typename T>
    static inline SetNode<T>* set_right_of(SetNode<T> *node) {
        return node ? node->right : nullptr;
    }

    #define set_assign_color(node, color) _set_set_color(node, color)

    template<typename T>
    static inline void _set_set_color(SetNode<T> *node, char color) {
        if (node) {
            node->color = color;
        }
    }

    #define set_color_of(node) _set_color_of(node)

    template<typename T>
    static inline char _set_color_of(SetNode<T> *node) {
        return node ? node->color : (int8_t) SetNode<T>::BLACK;
    }

    #define set_balance_insertion(side, rotate_1, rotate_2)         \
    do  {                                                           \
            {                                                       \
                SetNode<T> *uncle = side(set_grandparent_of(node)); \
                                                                    \
                if (set_color_of(uncle) == RED) {                   \
                    /*Uncle RED means color-flip*/                  \
                                                                    \
                    set_assign_color(set_parent_of(node), BLACK);   \
                    set_assign_color(set_grandparent_of(node), RED);\
                    set_assign_color(uncle, BLACK);                 \
                    node = set_grandparent_of(node);                \
                }                                                   \
                else {                                              \
                    /*Uncle BLACK means rotations*/                 \
                    if (node == side(set_parent_of(node))) {        \
                        node = set_parent_of(node);                 \
                        rotate_2(node);                             \
                    }                                               \
                    set_assign_color(set_parent_of(node), BLACK);   \
                    set_assign_color(set_grandparent_of(node), RED);\
                    rotate_1(set_grandparent_of(node));             \
                }                                                   \
            }                                                       \
    } while(0)

    #define set_balance_deletion(side1, side2, rotate_side1, rotate_side2)   \
    do {                                                                 \
            {                                                            \
                SetNode<T>* sibling = side1(set_parent_of(node));        \
                if (set_color_of(sibling) == RED) {                      \
                    set_assign_color(sibling, BLACK);                    \
                    set_assign_color(set_parent_of(node), RED);          \
                    rotate_side1(set_parent_of(node));                   \
                    sibling = side1(set_parent_of(node));                \
                }                                                        \
                if (set_color_of(side2(sibling)) == BLACK && set_color_of(side1(sibling)) == BLACK) { \
                    set_assign_color(sibling, RED);                      \
                    node = set_parent_of(node);                          \
                }                                                        \
                else {                                                   \
                    if (set_color_of(side1(sibling)) == BLACK) {         \
                        set_assign_color(side2(sibling), BLACK);         \
                        set_assign_color(sibling, RED);                  \
                        rotate_side2(sibling);                           \
                        sibling = side1(set_parent_of(node));            \
                    }                                                    \
                    set_assign_color(sibling, set_color_of(set_parent_of(node)));\
                    set_assign_color(set_parent_of(node), BLACK);        \
                    set_assign_color(side1(sibling), BLACK);             \
                    rotate_side1(set_parent_of(node));                   \
                    node = this->root;                                   \
                }                                                        \
            }                                                            \
    } while(0)

    template<typename T, class Less>
    set<T, Less>::set() noexcept : root(nullptr), endNode(nullptr) , _size(0) {}

    template<typename T, class Less>
    SetNode<T>* set<T, Less>::_copy_tree(SetNode<T>* other_root) {
        if (other_root == nullptr) {
            return nullptr;
        }

        SetNode<T>* new_node = new SetNode<T>(other_root->data);

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
    set<T, Less>::set(const set& other) noexcept {
        /*Create an exact copy of this set, O(n)*/
        this->root = _copy_tree(other.root);
        this->endNode = new SetNode<T>();
        this->endNode->left = this->root;
        this->root->parent = this->endNode;
    }

    template<typename T, class Less>
    set<T, Less>::set(set&& other) noexcept : set() {
        swap(*this, other);
    }

    template<typename T, class Less>
    set<T, Less>& set<T, Less>::operator=(set other) {
        /*Copy and swap idiom, let the compiler handle the copy of the argument*/
        swap(*this, other);

        return *this;
    }

    template<typename T, class Less>
    void set<T, Less>::tree_destroy(SetNode<T> *current) {

        if (!current) {
            return;
        }

        tree_destroy(current->left);
        tree_destroy(current->right);

        delete current;
    }

    template<typename T, class Less>
    set<T, Less>::~set() {
        if (this->root) {
            tree_destroy(this->root);
            delete this->endNode;
        }
    }

    template<typename T, class Less>
    SetNode<T> *set<T, Less>::bst_insert(bool &added_new, const T &data) {

        if (empty()) {
            added_new = true;
            this->root = new SetNode<T>(data);
            this->endNode = new SetNode<T>();
            this->endNode->left = this->root;
            this->root->parent = this->endNode;
            return this->root;
        }

        SetNode<T> *current = this->root;
        while (1) {

            if (less(data, current->data)) {
                if (current->left != nullptr) {
                    current = current->left;
                }
                else {
                    added_new = true;
                    SetNode<T> *new_node = new SetNode<T>(data);
                    current->left = new_node;
                    new_node->parent = current;

                    return new_node;
                }
            }
            else if (less(current->data, data)) {
                if (current->right != nullptr) {
                    current = current->right;
                }
                else {
                    added_new = true;
                    SetNode<T> *new_node = new SetNode<T>(data);
                    current->right = new_node;
                    new_node->parent = current;

                    return new_node;
                }
            }
            else {
                added_new = false;
                return current;
            }
        }
    }

    template<typename T, class Less>
    SetNode<T> *set<T, Less>::bst_insert(bool &added_new, T &&data) {

        if (empty()) {
            added_new = true;
            this->root = new SetNode<T>(std::forward<T>(data));
            this->endNode = new SetNode<T>();
            this->endNode->left = this->root;
            this->root->parent = this->endNode;
            return this->root;
        }

        SetNode<T> *current = this->root;
        while (1) {

            if (less(data, current->data)) {
                if (current->left != nullptr) {
                    current = current->left;
                }
                else {
                    added_new = true;
                    SetNode<T> *new_node = new SetNode<T>(std::forward<T>(data));
                    current->left = new_node;
                    new_node->parent = current;

                    return new_node;
                }
            }
            else if (less(current->data, data)) {
                if (current->right != nullptr) {
                    current = current->right;
                }
                else {
                    added_new = true;
                    SetNode<T> *new_node = new SetNode<T>(std::forward<T>(data));
                    current->right = new_node;
                    new_node->parent = current;

                    return new_node;
                }
            }
            else {
                added_new = false;
                return current;
            }
        }
    }

    template<typename T, class Less>
    bool set<T, Less>::add(const T &data) {

        bool added_new;
        SetNode<T> *current = bst_insert(added_new, data);

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
    bool set<T, Less>::add(T &&data) {

        bool added_new;
        SetNode<T> *current = bst_insert(added_new, std::forward<T>(data));

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
    void set<T, Less>::clear() noexcept {
        if (this->root) {
            tree_destroy(this->root);
            delete this->endNode;
            this->root = nullptr;
            this->endNode = nullptr;
            this->_size = 0;
        }
    }

    template<typename T, class Less>
    void set<T, Less>::clear() const noexcept {
        if (this->root) {
            tree_destroy(this->root);
            delete this->endNode;
            this->root = nullptr;
            this->endNode = nullptr;
            this->_size = 0;
        }
    }

    template<typename T, class Less>
    bool set<T, Less>::empty() {
        return this->root == nullptr;
    }


    template<typename T, class Less>
    void set<T, Less>::restore_balance(SetNode<T>* node, int8_t type) {

        if (type == DELETION) {
            while (node != this->root && set_color_of(node) == BLACK) {
                if (node == set_left_of(set_parent_of(node))) {
                    set_balance_deletion(set_right_of, set_left_of, rotate_left, rotate_right);
                }
                else {
                    set_balance_deletion(set_left_of, set_right_of, rotate_right, rotate_left);
                }
            }

            if (node != nullptr && set_color_of(node) != BLACK) {
                set_assign_color(node, BLACK);
            }
        }
        else if (type == INSERTION) {
            node->color = RED;
            this->root->parent = nullptr;

            while (node != endNode && node != this->root) {
                if (node->parent->color != RED) {
                    break;
                }

                if (set_parent_of(node) == set_left_of(set_grandparent_of(node))) {
                    set_balance_insertion(set_right_of, rotate_right, rotate_left);
                }
                else {
                    set_balance_insertion(set_left_of, rotate_left, rotate_right);
                }
            }
        }
        else {
            throw std::invalid_argument("This should not have happened, pls debug me :)");
        }
    }

    template<typename T, class Less>
    SetNode<T>* set<T, Less>::_remove(SetNode<T>* current, SetNode<T>* successor) {

        /*If this node is not a leaf and has both children*/
        if (current->left != nullptr && current->right != nullptr) {
            /*Get the minimum value of the right subtree*/
            current->data = successor->data;
            current = successor;
        }

        SetNode<T>* r_node = current->left != nullptr ? current->left : current->right;

        /*If node has one children*/
        if (r_node != nullptr) {
            r_node->parent = current->parent;
            SetNode<T>* parent_node = current->parent;
            if (parent_node == nullptr) {
                this->root = r_node;
            }
            else if (current == parent_node->left) {
                parent_node->left = r_node;
            }
            else {
                parent_node->right = r_node;
            }

            current->right = nullptr;
            current->left = nullptr;
            current->parent = nullptr;

            if (set_color_of(current) == BLACK) {
                /*Balance only if its a black node*/
                restore_balance(r_node, DELETION);
            }
        }
        else if (current->parent == nullptr) {
            this->_size = 0;
            delete this->root;
            delete this->endNode;
            this->root = nullptr;
            this->endNode = nullptr;
            return this->root;
        }
        else {
            /*Its a leaf*/
            if (set_color_of(current) == BLACK) {
                /*Balance only if its a black node*/
                restore_balance(current, DELETION);
            }

            SetNode<T>* parent_node = current->parent;
            if (parent_node != nullptr) {
                if (current == parent_node->left) {
                    parent_node->left = nullptr;
                }
                else if (current == parent_node->right) {
                    parent_node->right = nullptr;
                }
                current->parent = nullptr;
            }
        }

        return current;
    }

    template<typename T, class Less>
    set_iterator<T> set<T, Less>::begin() {
        SetNode<T> *current = this->root;
        if (current == nullptr) {
            return set_iterator<T>();
        }
        while (current->left != nullptr) {
            current = current->left;
        }

        return set_iterator<T>(current);
    }

    template<typename T, class Less>
    set_iterator<T> set<T, Less>::end() {
        return set_iterator<T>(this->endNode);
    }


    template<typename T, class Less>
    inline bool set<T, Less>::_is_equal_key(const T& nodeval, const T& val) {
        if (less(val, nodeval)) {
            return false;
        }
        else if (less(nodeval, val)) {
            return false;
        }
        return true;
    }

    template<typename T, class Less>
    SetNode<T>* set<T, Less>::_successor(SetNode<T> *node) {
        if (node == nullptr) {
            return nullptr;
        }
        SetNode<T>* current;
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
    SetNode<T>* set<T, Less>::_find_bound(SetNode<T> *_root, const T &val) {
        if (_root == nullptr) {
            return nullptr;
        }

        if (_is_equal_key(_root->data, val)) {
            return _root;
        }

        if (less(_root->data, val)) {
            return _find_bound(_root->right, val);
        }

        SetNode<T>* retnode = _find_bound(_root->left, val);
        return retnode == nullptr ? _root : retnode;
    }

    template<typename T, class Less>
    set_iterator<T> set<T, Less>::upper_bound(const T& val) {
        SetNode<T>* retnode = _find_bound(this->root, val);
        if (retnode == nullptr) {
            return this->end();
        }
        else {
            if (_is_equal_key(retnode->data, val)) {
                return set_iterator<T>(_successor(retnode));
            }
            else {
                return set_iterator<T>(retnode);
            }
        }
    }

    template<typename T, class Less>
    set_iterator<T> set<T, Less>::lower_bound(const T& val) {
        SetNode<T> *retnode = _find_bound(this->root, val);
        if (retnode == nullptr) {
            return this->end();
        }
        else {
            return set_iterator<T>(retnode);
        }
    }

    template<typename T, class Less>
    bool set<T, Less>::remove(const T &key) {

        SetNode<T>* current = nullptr;
        auto result = search(key);

        if (result == end()) {
            return false;
        }

        SetNode<T>* save = this->endNode;
        this->root->parent = nullptr;

        SetNode<T>* successor =  (++set_iterator<T>(result)).get_ptr();
        current = _remove(result.get_ptr(), successor);

        if (current != nullptr) {
            delete current;
            current = nullptr;

            /*Update the secret node*/
            this->endNode = save;
            this->root->parent = this->endNode;
            this->endNode->left = this->root;
            this->_size--;
        }

        return true;
    }

    template<typename T, class Less>
    set_iterator<T> set<T, Less>::remove(set_iterator<T> itr) {

        if (itr == end()) {
            return itr;
        }

        SetNode<T>* save = this->endNode;
        this->root->parent = nullptr;

        SetNode<T>* successor =  (++set_iterator<T>(itr)).get_ptr();
        SetNode<T>* current = _remove(itr.get_ptr(), successor);

        if (current != nullptr) {
            set_iterator<T> retrnValue(current == successor ? itr.get_ptr() : successor);

            delete current;
            current = nullptr;

            /*Update the secret node*/
            this->endNode = save;
            this->root->parent = this->endNode;
            this->endNode->left = this->root;
            this->_size--;

            if (successor != nullptr) {
                return retrnValue;
            }
            else {
                return this->end();
            }
        }

        return this->end();
    }

    template<typename T, class Less>
    set_iterator<T> set<T, Less>::search(const T &val) {

        if (this->root == nullptr) {
            return this->end();
        }
        SetNode<T> *current = this->root;
        while (current) {
            if (less(val, current->data)) {
                current = current->left;
            }
            else if (less(current->data, val)) {
                current = current->right;
            }
            else {
                return set_iterator<T>(current);
            }
        }

        return this->end();
    }

    template<typename T, class Less>
    void set<T, Less>::rotate_right(SetNode<T> *node) {

        if (!node) {
            return;
        }

        SetNode<T> *left_child = node->left;
        SetNode<T> *left_right_child = left_child->right;

        node->left = left_right_child;
        if (left_right_child) {
            left_right_child->parent = node;
        }

        SetNode<T> *node_p = node->parent;
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
    void set<T, Less>::rotate_left(SetNode<T> *node) {

        if (!node) {
            return;
        }

        SetNode<T> *right_child = node->right;
        SetNode<T> *right_left_child = right_child->left;

        node->right = right_left_child;
        if (right_left_child) {
            right_left_child->parent = node;
        }

        SetNode<T>  *node_p = node->parent;
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
    size_t set<T, Less>::size() {
        return this->_size;
    }

    template<typename T>
    SetNode<T>::SetNode() : data(), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}

    template<typename T>
    SetNode<T>::SetNode(const T &val) : data(val), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}

    template<typename T>
    SetNode<T>::SetNode(T &&val) : data(std::forward<T>(val)), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}

}