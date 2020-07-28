#pragma once

#include <functional>
#include <utility>
#include <cstddef>
#include <iterator>
#include <cstdint>
#include <iostream>

namespace adt {

    #define set_node typename set<T, Less>::SetNode*

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

        class SetNode {
        private:
            friend class set<T, Less>;
            friend class set_iterator;

            enum {
                RED,
                BLACK
            };

            T data;
            SetNode *left;
            SetNode *right;
            SetNode *parent;
            int8_t color;

            SetNode() : data(), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
            explicit SetNode(const T &val) : data(val), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
            explicit SetNode(T &&val) : data(std::forward<T>(val)), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
            explicit SetNode(const SetNode& node) = default;
            explicit SetNode(SetNode&& node) = default;

            SetNode& operator=(SetNode&& node) = default;
        };

        SetNode *root;
        SetNode *endNode;
        Less less;
        size_t _size;
        void rotate_left(SetNode *);
        void rotate_right(SetNode *);
        SetNode* _set_parent_of(SetNode* node);
        SetNode* _set_grandparent_of(SetNode* node);
        SetNode* set_left_of(SetNode* node);
        SetNode* set_right_of(SetNode* node);
        void _set_assign_color(SetNode* node, char color);
        char _set_color_of(SetNode* node);
        SetNode* bst_insert(bool&, const T&);
        SetNode* bst_insert(bool&, T&&);
        SetNode* _copy_tree(SetNode*);
        SetNode* _remove(SetNode *, SetNode*);
        inline bool _is_equal_key(const T& nodeval, const T& val);
        SetNode* _successor(SetNode*);
        SetNode* _find_bound(SetNode *_root, const T &val);
        void restore_balance(SetNode*, int8_t type);
        void tree_destroy(SetNode *);
    public:
        class set_iterator : public std::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t , T*, T&> {
        public:
            using const_data_reference = const T&;
            using const_data_pointer = const T*;
            
            set_iterator(SetNode* rhs = nullptr) : ptr(rhs) {}
            set_iterator(const set_iterator& rhs) : ptr(rhs.ptr) {}
            
            set_iterator& operator=(const set_iterator& rhs) = default;
            set_iterator& operator=(SetNode* rhs) {
                this->ptr = rhs;
                return *this;
            }

            bool operator==(const set_iterator& rhs) const {
                return this->ptr == rhs.ptr;
            }
            bool operator!=(const set_iterator& rhs) const {
                return !(*this == rhs);
            }

            const_data_reference operator*() {
                return this->ptr->data;
            }
            const_data_pointer operator->() {
                return &(this->ptr->data);
            }

            /* Inorder successor algorithm. */
            set_iterator& operator++() {
                if (this->ptr == nullptr) {
                    return *this;
                }

                SetNode* current;
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
            set_iterator operator++(int) {
                set_iterator tmp(*this);
                ++(*this);

                return tmp;
            }
            
            /* Inorder predecessor algorithm. */
            set_iterator& operator--() {
                if (this->ptr == nullptr) {
                    return *this;
                }

                SetNode* current;
                if (this->ptr->left) {
                    current = this->ptr->left;
                    while (current->right != nullptr) {
                        current = current->right;
                    }
                }
                else {
                    current = this->ptr->parent;
                    while (current->parent != nullptr && this->ptr == current->left) {
                        this->ptr = current;
                        current = current->parent;
                    }
                }

                this->ptr = current;
                return *this;
            }
            set_iterator operator--(int) {
                set_iterator tmp(*this);
                --(*this);

                return tmp;    
            }
            
            void swap(set_iterator& lhs, set_iterator& rhs) {
                std::swap(lhs, rhs);
            }
            
        protected:
            friend class set<T, Less>;
            SetNode* ptr;
        };

        class set_r_iterator : public set_iterator {
        public:            
            set_r_iterator(SetNode* _ptr = nullptr) : set_iterator(_ptr) {}
            set_r_iterator(const set_r_iterator& _ptr) = default;
            
            set_r_iterator& operator=(const set_r_iterator& _ptr) = default;
            set_r_iterator& operator=(SetNode* _ptr) {
                this->ptr = _ptr;
                return *this;
            }

            /* Inorder predecessor algorithm. */
            set_r_iterator& operator++() {
                if (this->ptr == nullptr) {
                    return *this;
                }

                SetNode* current;
                if (this->ptr->left) {
                    current = this->ptr->left;
                    while (current->right != nullptr) {
                        current = current->right;
                    }
                }
                else {
                    current = this->ptr->parent;
                    while (current->parent != nullptr && this->ptr == current->left) {
                        this->ptr = current;
                        current = current->parent;
                    }
                }

                this->ptr = current;
                return *this;
            }

            /* Inorder successor algorithm. */
            set_r_iterator& operator--() {
                if (this->ptr == nullptr) {
                    return *this;
                }
                SetNode* current;
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

            set_r_iterator operator++(int) {
                set_r_iterator tmp(*this);
                ++(*this);

                return tmp;
            }

            set_r_iterator operator--(int) {
                set_r_iterator tmp(*this);
                --(*this);

                return tmp;    
            }
        };        

        set() noexcept;
        set(const set& rhs) noexcept;
        set(set&& rhs) noexcept;
        ~set();

        set& operator=(set x);

        /* Set modifiers. */
        bool add(const T& data);
        bool add(T&& data);
        bool remove(const T& key);
        set_iterator remove(set_iterator itr);
        void clear() noexcept;
        void clear() const noexcept;

        /* Set math operations. */
        /*TODO : join, intersection, difference */
        set& join(const set& other);
        set& intersection(const set& other);
        set& difference(const set& other);

        bool empty();
        size_t size();

        /* Set lookup operations. */
        set_iterator begin();
        set_iterator end();
        set_r_iterator rbegin();
        set_r_iterator rend();
        set_iterator search(const T& key);
        set_iterator lower_bound(const T& val);
        set_iterator upper_bound(const T& val);

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
    #define set_grandparent_of(node) _set_grandparent_of(node)
    #define set_assign_color(node, color) _set_assign_color(node, color)
    #define set_color_of(node) _set_color_of(node)

    #define set_balance_insertion(side, rotate_1, rotate_2)         \
    do  {                                                           \
            {                                                       \
                SetNode *uncle = side(set_grandparent_of(node)); \
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
                SetNode* sibling = side1(set_parent_of(node));        \
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
    set_node set<T, Less>::_copy_tree(SetNode* other_root) {
        if (other_root == nullptr) {
            return nullptr;
        }

        SetNode* new_node = new SetNode(other_root->data);

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
        this->endNode = new SetNode();
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
    void set<T, Less>::tree_destroy(SetNode *current) {

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
    set_node set<T, Less>::bst_insert(bool &added_new, const T &data) {

        if (empty()) {
            added_new = true;
            this->root = new SetNode(data);
            this->endNode = new SetNode();
            this->endNode->left = this->root;
            this->root->parent = this->endNode;
            return this->root;
        }

        SetNode *current = this->root;
        while (1) {

            if (less(data, current->data)) {
                if (current->left != nullptr) {
                    current = current->left;
                }
                else {
                    added_new = true;
                    SetNode *new_node = new SetNode(data);
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
                    SetNode *new_node = new SetNode(data);
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
    set_node set<T, Less>::bst_insert(bool &added_new, T &&data) {

        if (empty()) {
            added_new = true;
            this->root = new SetNode(std::forward<T>(data));
            this->endNode = new SetNode();
            this->endNode->left = this->root;
            this->root->parent = this->endNode;
            return this->root;
        }

        SetNode *current = this->root;
        while (1) {

            if (less(data, current->data)) {
                if (current->left != nullptr) {
                    current = current->left;
                }
                else {
                    added_new = true;
                    SetNode *new_node = new SetNode(std::forward<T>(data));
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
                    SetNode *new_node = new SetNode(std::forward<T>(data));
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
        SetNode *current = bst_insert(added_new, data);

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
        SetNode *current = bst_insert(added_new, std::forward<T>(data));

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
    void set<T, Less>::restore_balance(SetNode* node, int8_t type) {

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
    set_node set<T, Less>::_remove(SetNode* current, SetNode* successor) {

        /*If this node is not a leaf and has both children*/
        if (current->left != nullptr && current->right != nullptr) {
            /*Get the minimum value of the right subtree*/
            current->data = successor->data;
            current = successor;
        }

        SetNode* r_node = current->left != nullptr ? current->left : current->right;

        /*If node has one children*/
        if (r_node != nullptr) {
            r_node->parent = current->parent;
            SetNode* parent_node = current->parent;
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

            SetNode* parent_node = current->parent;
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
    typename set<T, Less>::set_iterator set<T, Less>::begin() {
        SetNode *current = this->root;
        if (current == nullptr) {
            return set_iterator();
        }
        while (current->left != nullptr) {
            current = current->left;
        }

        return set_iterator(current);
    }

    template<typename T, class Less>
    typename set<T, Less>::set_iterator set<T, Less>::end() {
        return set_iterator(this->endNode);
    }

    template<typename T, class Less>
    typename set<T, Less>::set_r_iterator set<T, Less>::rbegin() {
        SetNode *current = this->root;
        while (current && current->right) {
            current = current->right;
        }

        return set_r_iterator(current);
    }

    template<typename T, class Less>
    typename set<T, Less>::set_r_iterator set<T, Less>::rend() {
        return set_r_iterator(this->endNode);
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
    set_node set<T, Less>::_successor(SetNode *node) {
        if (node == nullptr) {
            return nullptr;
        }
        SetNode* current;
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
    set_node set<T, Less>::_find_bound(SetNode *_root, const T &val) {
        if (_root == nullptr) {
            return nullptr;
        }

        if (_is_equal_key(_root->data, val)) {
            return _root;
        }

        if (less(_root->data, val)) {
            return _find_bound(_root->right, val);
        }

        SetNode* retnode = _find_bound(_root->left, val);
        return retnode == nullptr ? _root : retnode;
    }

    template<typename T, class Less>
    typename set<T, Less>::set_iterator set<T, Less>::upper_bound(const T& val) {
        SetNode* retnode = _find_bound(this->root, val);
        if (retnode == nullptr) {
            return this->end();
        }
        else {
            if (_is_equal_key(retnode->data, val)) {
                return set_iterator(_successor(retnode));
            }
            else {
                return set_iterator(retnode);
            }
        }
    }

    template<typename T, class Less>
    typename set<T, Less>::set_iterator set<T, Less>::lower_bound(const T& val) {
        SetNode *retnode = _find_bound(this->root, val);
        if (retnode == nullptr) {
            return this->end();
        }
        else {
            return set_iterator(retnode);
        }
    }

    template<typename T, class Less>
    bool set<T, Less>::remove(const T &key) {

        SetNode* current = nullptr;
        auto result = search(key);

        if (result == end()) {
            return false;
        }

        SetNode* save = this->endNode;
        this->root->parent = nullptr;

        SetNode* successor =  (++set_iterator(result)).ptr;
        current = _remove(result.ptr, successor);

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
    typename set<T, Less>::set_iterator set<T, Less>::remove(set_iterator itr) {

        if (itr == end()) {
            return itr;
        }

        SetNode* save = this->endNode;
        this->root->parent = nullptr;

        SetNode* successor =  (++set_iterator(itr)).ptr;
        SetNode* current = _remove(itr.ptr, successor);

        if (current != nullptr) {
            set_iterator retrnValue(current == successor ? itr.ptr : successor);

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
    typename set<T, Less>::set_iterator set<T, Less>::search(const T &val) {

        if (this->root == nullptr) {
            return this->end();
        }
        SetNode *current = this->root;
        while (current) {
            if (less(val, current->data)) {
                current = current->left;
            }
            else if (less(current->data, val)) {
                current = current->right;
            }
            else {
                return set_iterator(current);
            }
        }

        return this->end();
    }

    template<typename T, class Less>
    void set<T, Less>::rotate_right(SetNode *node) {

        if (!node) {
            return;
        }

        SetNode *left_child = node->left;
        SetNode *left_right_child = left_child->right;

        node->left = left_right_child;
        if (left_right_child) {
            left_right_child->parent = node;
        }

        SetNode *node_p = node->parent;
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
    void set<T, Less>::rotate_left(SetNode *node) {

        if (!node) {
            return;
        }

        SetNode *right_child = node->right;
        SetNode *right_left_child = right_child->left;

        node->right = right_left_child;
        if (right_left_child) {
            right_left_child->parent = node;
        }

        SetNode  *node_p = node->parent;
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
    set_node set<T, Less>::_set_parent_of(set_node node) {
        return node ? node->parent : nullptr;
    }

    template<typename T, class Less>
    set_node set<T, Less>::_set_grandparent_of(set_node node) {
        if (node) {
            if (node->parent) {
                return node->parent->parent;
            }
        }
        return nullptr;
    }

    template<typename T, class Less> 
    set_node set<T, Less>::set_left_of(set_node node) {
        return node ? node->left : nullptr;
    }

    template<typename T, class Less>
    set_node set<T, Less>::set_right_of(set_node node) {
        return node ? node->right : nullptr;
    }

    template<typename T, class Less>
    void set<T, Less>::_set_assign_color(set_node node, char color) {
        if (node) {
            node->color = color;
        }
    }

    template<typename T, class Less>
    char set<T, Less>::_set_color_of(set_node node) {
        return node ? node->color : (int8_t) SetNode::BLACK;
    }

    template<typename T, class Less>
    size_t set<T, Less>::size() {
        return this->_size;
    }
}
