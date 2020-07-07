#ifndef MAP_H
#define MAP_H

#include <functional>
#include <utility>
#include <cstddef>
#include <iterator>
#include <cstdint>
#include <iostream>

namespace adt {

    template<typename Key, typename T>
    struct MapNode {
        enum {
            RED,
            BLACK
        };

        typedef std::pair<const Key, T> value_type;

        value_type data;
        MapNode *left;
        MapNode *right;
        MapNode *parent;
        int8_t color;

        MapNode() : data(), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
        MapNode(const Key& key, const T& value) : data(std::make_pair(key, value)), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
        MapNode(const Key& key, T&& value) : data(std::make_pair(key, std::forward<T>(value))), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
        MapNode(Key&& key, T&& value) : data(std::make_pair(std::forward<const Key>(key), std::forward<T>(value))), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
        MapNode(Key&& key, const T& value) : data(std::make_pair(std::forward<const Key>(key), value)), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
        explicit MapNode(const value_type& value) : data(value), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
        explicit MapNode(const MapNode& node) = default;
        explicit MapNode(MapNode&& node) = default;

        MapNode& operator=(MapNode&& node) = default;
    };

    template<typename Key, typename T>
    class map_iterator : public std::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t, T*, T&> {
    public:

        typedef std::pair<const Key, T> value_type;

        map_iterator(MapNode<Key, T>* rhs = nullptr) : ptr(rhs) {}
        map_iterator(const map_iterator<Key, T>& rhs) : ptr(rhs.ptr) {}
        map_iterator<Key, T>& operator=(const map_iterator<Key, T>& rhs) {
            this->ptr = rhs.ptr;
            return *this;
        }
        map_iterator<Key, T>& operator=(MapNode<Key, T>* rhs) {
            this->ptr = rhs;
            return *this;
        }
        bool operator==(const map_iterator<Key, T>& rhs) const {
            return this->ptr == rhs.ptr;
        }
        bool operator!=(const map_iterator<Key, T>& rhs) const {
            return !(*this == rhs);
        }
        value_type& operator*() {
            return this->ptr->data;
        }
        const value_type& operator*() const {
            return this->ptr->data;
        }
        value_type* operator->() {
            return &(this->ptr->data);
        }
        /*Inorder successor algorithm*/
        map_iterator<Key, T> operator++() {
            if (this->ptr == nullptr) {
                return map_iterator<Key, T>();
            }
            MapNode<Key, T>* current;
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
        map_iterator<Key, T> operator++(int) {
            if (this->ptr == nullptr) {
                return map_iterator<Key, T>();
            }

            map_iterator<Key, T> tmp(*this);
            MapNode<Key, T>* current;
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
            return tmp;
        }
        /*Inorder predecessor algorithm*/
        map_iterator<Key, T> operator--() {
            if (this->ptr == nullptr) {
                return map_iterator<Key, T>();
            }

            MapNode<Key, T>* current;
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
        map_iterator<Key, T> operator--(int) {
            if (this->ptr == nullptr) {
                return map_iterator<Key, T>();
            }

            map_iterator<Key, T> tmp(*this);

            MapNode<Key, T>* current;
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
            return tmp;
        }
        void swap(map_iterator<Key, T>& lhs, map_iterator<Key, T>& rhs) {
            std::swap(lhs, rhs);
        }

        MapNode<Key, T>* get_ptr() {
            return this->ptr;
        }

        const MapNode<Key, T>* get_ptr() const {
            return this->ptr;
        }

    private:
        MapNode<Key, T>* ptr;
    };

    template<typename Key, typename T, class Less = std::less<Key>>
    class map {
    private:
        enum {
            RED,
            BLACK
        };
        enum SetBalanceType {
            DELETION,
            INSERTION
        };

        MapNode<Key, T> *root;
        MapNode<Key, T> *endNode;
        Less less;
        size_t _size;
        void rotate_left(MapNode<Key, T> *);
        void rotate_right(MapNode<Key, T> *);
        MapNode<Key, T>* bst_insert(bool& added_new, const Key& key, const T& value);
        MapNode<Key, T>* bst_insert(bool& added_new, const Key& key, T&& value);
        MapNode<Key, T>* bst_insert(bool& added_new, Key&& key, T&& value);
        MapNode<Key, T>* bst_insert(bool& added_new, Key&& key, const T& value);
        MapNode<Key, T>* _copy_tree(MapNode<Key, T>*);
        MapNode<Key, T>* _remove(MapNode<Key, T> *, MapNode<Key, T>*);
        MapNode<Key, T>* _find_bound(MapNode<Key, T>*, const Key&);
        inline bool _is_equal_key(const Key&, const Key&);
        void restore_balance(MapNode<Key, T>*, int8_t type);
        void tree_destroy(MapNode<Key, T> *);
    public:
        map() noexcept;
        map(const map& rhs) noexcept;
        map(map&& rhs) noexcept;
        ~map();

        map& operator=(map x);
        bool add(const Key& key, const T& value);
        bool add(const Key& key, T&& value);
        bool add(Key&& key, T&& value);
        bool add(Key&& key, const T& value);
        bool remove(const Key& key);
        map_iterator<Key, T> remove(map_iterator<Key, T> itr);
        void clear() noexcept;
        void clear() const noexcept;

        bool empty();
        size_t size();

        map_iterator<Key, T> lower_bound(const Key& key);
        map_iterator<Key, T> upper_bound(const Key& key);
        map_iterator<Key, T> begin();
        map_iterator<Key, T> end();
        map_iterator<Key, T> search(const Key& key);
        T& operator[](const Key& key);
        const T& at(const Key& key) const;
        T& at(const Key& key);

        friend void swap(Map& lhs, Map& rhs) {
            using std::swap;

            swap(lhs.root, rhs.root);
            swap(lhs.endNode, rhs.endNode);
            swap(lhs.less, rhs.less);
            swap(lhs._size, rhs._size);
        }
    };

    #define map_parent_of(node) _map_parent_of(node)

        template<typename Key, typename T>
        static inline MapNode<Key, T>* _map_parent_of(MapNode<Key, T>* node) {
            return node ? node->parent : nullptr;
        }

    #define map_grandparent_of(node) _map_grandparent_of(node)

        template<typename Key, typename T>
        static inline MapNode<Key, T>* _map_grandparent_of(MapNode<Key, T>* node) {
            if (node) {
                if (node->parent) {
                    return node->parent->parent;
                }
            }
            return nullptr;
        }

        template<typename Key, typename T>
        static inline MapNode<Key, T>* map_left_of(MapNode<Key, T>* node) {
            return node ? node->left : nullptr;
        }

        template<typename Key, typename T>
        static inline MapNode<Key, T>* map_right_of(MapNode<Key, T> *node) {
            return node ? node->right : nullptr;
        }

    #define map_assign_color(node, color) _map_assign_color(node, color)

        template<typename Key, typename T>
        static inline void _map_assign_color(MapNode<Key, T> *node, char color) {
            if (node) {
                node->color = color;
            }
        }

    #define map_color_of(node) _map_color_of(node)

        template<typename Key, typename T>
        static inline char _map_color_of(MapNode<Key, T> *node) {
            return node ? node->color : (int8_t) MapNode<Key, T>::BLACK;
        }

    #define map_balance_insertion(side, rotate_1, rotate_2)                 \
        do  {                                                               \
                {                                                           \
                    MapNode<Key, T> *uncle = side(map_grandparent_of(node));\
                                                                            \
                    if (map_color_of(uncle) == RED) {                       \
                        /*Uncle RED means color-flip*/                      \
                        map_assign_color(map_parent_of(node), BLACK);       \
                        map_assign_color(map_grandparent_of(node), RED);    \
                        map_assign_color(uncle, BLACK);                     \
                        node = _map_grandparent_of(node);                   \
                    }                                                       \
                    else {                                                  \
                        /*Uncle BLACK means rotations*/                     \
                        if (node == side(map_parent_of(node))) {            \
                            node = map_parent_of(node);                     \
                            rotate_2(node);                                 \
                        }                                                   \
                        map_assign_color(map_parent_of(node), BLACK);       \
                        map_assign_color(map_grandparent_of(node), RED);    \
                        rotate_1(_map_grandparent_of(node));                \
                    }                                                       \
                }                                                           \
        } while(0)

    #define map_balance_deletion(side1, side2, rotate_side1, rotate_side2)  \
        do {                                                                \
                {                                                           \
                    MapNode<Key, T>* sibling = side1(map_parent_of(node));  \
                    if (map_color_of(sibling) == RED) {                     \
                        map_assign_color(sibling, BLACK);                   \
                        map_assign_color(map_parent_of(node), RED);         \
                        rotate_side1(map_parent_of(node));                  \
                        sibling = side1(map_parent_of(node));               \
                    }                                                       \
                    if (map_color_of(side2(sibling)) == BLACK && map_color_of(side1(sibling)) == BLACK) { \
                        map_assign_color(sibling, RED);                     \
                        node = map_parent_of(node);                         \
                    }                                                       \
                    else {                                                  \
                        if (map_color_of(side1(sibling)) == BLACK) {        \
                            map_assign_color(side2(sibling), BLACK);        \
                            map_assign_color(sibling, RED);                 \
                            rotate_side2(sibling);                          \
                            sibling = side1(map_parent_of(node));           \
                        }                                                   \
                        map_assign_color(sibling, map_color_of(map_parent_of(node)));\
                        map_assign_color(map_parent_of(node), BLACK);       \
                        map_assign_color(side1(sibling), BLACK);            \
                        rotate_side1(map_parent_of(node));                  \
                        node = this->root;                                  \
                    }                                                       \
                }                                                           \
        } while(0)

    template<typename Key, typename T, class Less>
    map<Key, T, Less>::map() noexcept : root(nullptr), endNode(nullptr) , _size(0) {}

    template<typename Key, typename T, class Less>
    MapNode<Key, T>* map<Key, T, Less>::_copy_tree(MapNode<Key, T>* other_root) {
        if (other_root == nullptr) {
            return nullptr;
        }

        MapNode<Key, T>* new_node = new MapNode<Key, T>(other_root->get_value());

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

    template<typename Key, typename T, class Less>
    map<Key, T, Less>::map(const map& other) noexcept {
        /*Create an exact copy of this map, O(n)*/
        this->root = _copy_tree(other.root);
        this->endNode = new MapNode<Key, T>();
        this->endNode->left = this->root;
        this->root->parent = this->endNode;
    }

    template<typename Key, typename T, class Less>
    map<Key, T, Less>::map(map&& other) noexcept : map() {
        swap(*this, other);
    }

    template<typename Key, typename T, class Less>
    map<Key, T, Less>& map<Key, T, Less>::operator=(map other) {
        /*Copy and swap idiom, let the compiler handle the copy of the argument*/
        swap(*this, other);

        return *this;
    }

    template<typename Key, typename T, class Less>
    void map<Key, T, Less>::tree_destroy(MapNode<Key, T> *current) {

        if (!current) {
            return;
        }

        tree_destroy(current->left);
        tree_destroy(current->right);

        delete current;
    }

    template<typename Key, typename T, class Less>
    map<Key, T, Less>::~map() {
        if (this->root) {
            tree_destroy(this->root);
            delete this->endNode;
        }
    }

    template<typename Key, typename T, class Less>
    MapNode<Key, T> *map<Key, T, Less>::bst_insert(bool &added_new, const Key& key, const T& value) {

        if (empty()) {
            added_new = true;
            this->root = new MapNode<Key, T>(key, value);
            this->endNode = new MapNode<Key, T>();
            this->endNode->left = this->root;
            this->root->parent = this->endNode;
            return this->root;
        }

        MapNode<Key, T> *current = this->root;
        while (1) {

            if (less(key, current->data.first)) {
                if (current->left != nullptr) {
                    current = current->left;
                }
                else {
                    added_new = true;
                    MapNode<Key, T> *new_node = new MapNode<Key, T>(key, value);
                    current->left = new_node;
                    new_node->parent = current;

                    return new_node;
                }
            }
            else if (less(current->data.first, key)) {
                if (current->right != nullptr) {
                    current = current->right;
                }
                else {
                    added_new = true;
                    MapNode<Key, T> *new_node = new MapNode<Key, T>(key, value);
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

    template<typename Key, typename T, class Less>
    MapNode<Key, T> *map<Key, T, Less>::bst_insert(bool &added_new, const Key &key, T &&value) {

        if (empty()) {
            added_new = true;
            this->root = new MapNode<Key, T>(key, std::forward<T>(value));
            this->endNode = new MapNode<Key, T>();
            this->endNode->left = this->root;
            this->root->parent = this->endNode;
            return this->root;
        }

        MapNode<Key, T> *current = this->root;
        while (1) {

            if (less(key, current->data.first)) {
                if (current->left != nullptr) {
                    current = current->left;
                }
                else {
                    added_new = true;
                    MapNode<Key, T> *new_node = new MapNode<Key, T>(key, std::forward<T>(value));
                    current->left = new_node;
                    new_node->parent = current;

                    return new_node;
                }
            }
            else if (less(current->data.first, key)) {
                if (current->right != nullptr) {
                    current = current->right;
                }
                else {
                    added_new = true;
                    MapNode<Key, T> *new_node = new MapNode<Key, T>(key, std::forward<T>(value));
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

    template<typename Key, typename T, class Less>
    MapNode<Key, T> *map<Key, T, Less>::bst_insert(bool &added_new, Key &&key, const T &value) {
        if (empty()) {
            added_new = true;
            this->root = new MapNode<Key, T>(std::forward<Key>(key), value);
            this->endNode = new MapNode<Key, T>();
            this->endNode->left = this->root;
            this->root->parent = this->endNode;
            return this->root;
        }

        MapNode<Key, T> *current = this->root;
        while (1) {

            if (less(key, current->data.first)) {
                if (current->left != nullptr) {
                    current = current->left;
                }
                else {
                    added_new = true;
                    MapNode<Key, T> *new_node = new MapNode<Key, T>(std::forward<Key>(key), value);
                    current->left = new_node;
                    new_node->parent = current;

                    return new_node;
                }
            }
            else if (less(current->data.first, key)) {
                if (current->right != nullptr) {
                    current = current->right;
                }
                else {
                    added_new = true;
                    MapNode<Key, T> *new_node = new MapNode<Key, T>(std::forward<Key>(key), value);
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


    template<typename Key, typename T, class Less>
    MapNode<Key, T> *map<Key, T, Less>::bst_insert(bool &added_new, Key&& key, T&& value) {

        if (empty()) {
            added_new = true;
            this->root = new MapNode<Key, T>(std::forward<Key>(key), std::forward<T>(value));
            this->endNode = new MapNode<Key, T>();
            this->endNode->left = this->root;
            this->root->parent = this->endNode;
            return this->root;
        }

        MapNode<Key, T> *current = this->root;
        while (1) {

            if (less(key, current->data.first)) {
                if (current->left != nullptr) {
                    current = current->left;
                }
                else {
                    added_new = true;
                    MapNode<Key, T> *new_node = new MapNode<Key, T>(std::forward<Key>(key), std::forward<T>(value));
                    current->left = new_node;
                    new_node->parent = current;

                    return new_node;
                }
            }
            else if (less(current->data.first, key)) {
                if (current->right != nullptr) {
                    current = current->right;
                }
                else {
                    added_new = true;
                    MapNode<Key, T> *new_node = new MapNode<Key, T>(std::forward<Key>(key), std::forward<T>(value));
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

    template<typename Key, typename T, class Less>
    bool map<Key, T, Less>::add(const Key& key, const T& value) {

        bool added_new;
        MapNode<Key, T> *current = bst_insert(added_new, key, value);

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

    template<typename Key, typename T, class Less>
    bool map<Key, T, Less>::add(const Key& key, T&& value) {

        bool added_new;
        MapNode<Key, T> *current = bst_insert(added_new, key, value);

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

    template<typename Key, typename T, class Less>
    bool map<Key, T, Less>::add(Key&& key, const T& value) {

        bool added_new;
        MapNode<Key, T> *current = bst_insert(added_new, key, value);

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

    template<typename Key, typename T, class Less>
    bool map<Key, T, Less>::add(Key&& key, T&& value) {

        bool added_new;
        MapNode<Key, T> *current = bst_insert(added_new, std::forward<Key>(key), std::forward<T>(value));

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

    template<typename Key, typename T, class Less>
    void map<Key, T, Less>::clear() noexcept {
        if (this->root) {
            tree_destroy(this->root);
            delete this->endNode;
            this->root = nullptr;
            this->endNode = nullptr;
            this->_size = 0;
        }
    }

    template<typename Key, typename T, class Less>
    void map<Key, T, Less>::clear() const noexcept {
        if (this->root) {
            tree_destroy(this->root);
            delete this->endNode;
            this->root = nullptr;
            this->endNode = nullptr;
            this->_size = 0;
        }
    }

    template<typename Key, typename T, class Less>
    bool map<Key, T, Less>::empty() {
        return this->root == nullptr;
    }

    template<typename Key, typename T, class Less>
    void map<Key, T, Less>::restore_balance(MapNode<Key, T>* node, int8_t type) {

        if (type == DELETION) {
            while (node != this->root && map_color_of(node) == BLACK) {
                if (node == map_left_of(map_parent_of(node))) {
                    map_balance_deletion(map_right_of, map_left_of, rotate_left, rotate_right);
                }
                else {
                    map_balance_deletion(map_left_of, map_right_of, rotate_right, rotate_left);
                }
            }

            if (node != nullptr && map_color_of(node) != BLACK) {
                map_assign_color(node, BLACK);
            }
        }
        else if (type == INSERTION) {
            node->color = RED;
            this->root->parent = nullptr;

            while (node != endNode && node != this->root) {
                if (node->parent->color != RED) {
                    break;
                }

                if (map_parent_of(node) == map_left_of(_map_grandparent_of(node))) {
                    map_balance_insertion(map_right_of, rotate_right, rotate_left);
                }
                else {
                    map_balance_insertion(map_left_of, rotate_left, rotate_right);
                }
            }
        }
        else {
            throw std::invalid_argument("This should not have happened, pls debug me :)");
        }
    }

    template<typename Key, typename T, class Less>
    MapNode<Key, T>* map<Key, T, Less>::_remove(MapNode<Key, T>* current, MapNode<Key, T>* successor) {

        /*If this node is not a leaf and has both children*/
        if (current->left != nullptr && current->right != nullptr) {
            /*Get the minimum value of the right subtree*/
            using std::swap;
            swap((Key&) current->data.first, (Key&) successor->data.first);
            swap(current->data.second, successor->data.second);
            current = successor;
        }

        MapNode<Key, T>* r_node = current->left != nullptr ? current->left : current->right;

        /*If node has one children*/
        if (r_node != nullptr) {
            r_node->parent = current->parent;
            MapNode<Key, T>* parent_node = current->parent;
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

            if (map_color_of(current) == BLACK) {
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
            if (map_color_of(current) == BLACK) {
                /*Balance only if its a black node*/
                restore_balance(current, DELETION);
            }

            MapNode<Key, T>* parent_node = current->parent;
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

    template<typename Key, typename T, class Less>
    map_iterator<Key, T> map<Key, T, Less>::begin() {
        MapNode<Key, T> *current = this->root;
        if (current == nullptr) {
            return map_iterator<Key, T>();
        }
        while (current->left != nullptr) {
            current = current->left;
        }

        return map_iterator<Key, T>(current);
    }

    template<typename Key, typename T, class Less>
    map_iterator<Key, T> map<Key, T, Less>::end() {
        return map_iterator<Key, T>(this->endNode);
    }

    template<typename Key, typename T, class Less>
    inline bool map<Key, T, Less>::_is_equal_key(const Key& lhs, const Key& rhs) {
        if (less(lhs, rhs)) {
            return false;
        }
        else if (less(rhs, lhs)) {
            return false;
        }
        return true;
    }

    template<typename Key, typename T, class Less>
    MapNode<Key, T>* map<Key, T, Less>::_find_bound(MapNode<Key, T> *_root, const Key &key) {
        if (_root == nullptr) {
            return nullptr;
        }

        if (_is_equal_key(_root->data.first, key)) {
            return _root;
        }

        if (less(_root->data.first, key)) {
            return _find_bound(_root->right, key);
        }

        MapNode<Key, T>* retnode = _find_bound(_root->left, key);
        return retnode == nullptr ? _root : retnode;
    }

    template<typename Key, typename T, class Less>
    map_iterator<Key, T> map<Key, T, Less>::upper_bound(const Key& key) {
        MapNode<Key, T>* retnode = _find_bound(this->root, key);
        if (retnode == nullptr) {
            return this->end();
        }
        else {
            return ++map_iterator<Key, T>(retnode);
        }
    }

    template<typename Key, typename T, class Less>
    map_iterator<Key, T> map<Key, T, Less>::lower_bound(const Key& key) {
        MapNode<Key, T> *retnode = _find_bound(this->root, key);
        if (retnode == nullptr) {
            return this->end();
        }
        else {
            return map_iterator<Key, T>(retnode);
        }
    }


    template<typename Key, typename T, class Less>
    bool map<Key, T, Less>::remove(const Key &key) {

        MapNode<Key, T>* current = nullptr;
        auto result = search(key);

        if (result == end()) {
            return false;
        }

        MapNode<Key, T>* save = this->endNode;
        this->root->parent = nullptr;

        MapNode<Key, T>* successor =  (++map_iterator<Key, T>(result)).get_ptr();
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

    template<typename Key, typename T, class Less>
    map_iterator<Key, T> map<Key, T, Less>::remove(map_iterator<Key, T> itr) {

        if (itr == end()) {
            return itr;
        }

        MapNode<Key, T>* save = this->endNode;
        this->root->parent = nullptr;

        MapNode<Key, T>* successor =  (++map_iterator<Key, T>(itr)).get_ptr();
        MapNode<Key, T>* current = _remove(itr.get_ptr(), successor);

        if (current != nullptr) {
            map_iterator<Key, T> retrnValue(current == successor ? itr.get_ptr() : successor);

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

    template<typename Key, typename T, class Less>
    map_iterator<Key, T> map<Key, T, Less>::search(const Key &key) {

        if (this->root == nullptr) {
            return this->end();
        }
        MapNode<Key, T> *current = this->root;
        while (current) {
            if (less(key, current->data.first)) {
                current = current->left;
            }
            else if (less(current->data.first, key)) {
                current = current->right;
            }
            else {
                return map_iterator<Key, T>(current);
            }
        }

        return this->end();
    }

    template<typename Key, typename T, class Less>
    T& map<Key, T, Less>::operator[](const Key& key) {

        bool added_new;
        MapNode<Key, T> *current = bst_insert(added_new, key, T());

        if (!added_new) {
            return current->data.second;
        }

        restore_balance(current,INSERTION);

        this->root->color = BLACK;
        this->_size++;
        this->root->parent = this->endNode;
        this->endNode->left = this->root;

        return current->data.second;
    }

    template<typename Key, typename T, class Less>
    const T& map<Key, T, Less>::at(const Key& key) const {

        auto it = search(key);
        if (it == end()) {
            throw std::out_of_range("Element is not present on map");
        }

        return it->second;
    }

    template<typename Key, typename T, class Less>
    T& map<Key, T, Less>::at(const Key& key) {

        auto it = search(key);
        if (it == end()) {
            throw std::out_of_range("Element is not present on map");
        }

        return it->second;
    }

    template<typename Key, typename T, class Less>
    void map<Key, T, Less>::rotate_right(MapNode<Key, T> *node) {

        if (!node) {
            return;
        }

        MapNode<Key, T> *left_child = node->left;
        MapNode<Key, T> *left_right_child = left_child->right;

        node->left = left_right_child;
        if (left_right_child) {
            left_right_child->parent = node;
        }

        MapNode<Key, T> *node_p = node->parent;
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

    template<typename Key, typename T, class Less>
    void map<Key, T, Less>::rotate_left(MapNode<Key, T> *node) {

        if (!node) {
            return;
        }

        MapNode<Key, T> *right_child = node->right;
        MapNode<Key, T> *right_left_child = right_child->left;

        node->right = right_left_child;
        if (right_left_child) {
            right_left_child->parent = node;
        }

        MapNode<Key, T>  *node_p = node->parent;
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

    template<typename Key, typename T, class Less>
    size_t map<Key, T, Less>::size() {
        return this->_size;
    }

    template<typename Key, typename T, class Less>
    map<Key, T, Less>* map<Key, T, Less>::get_map() {
        return this;
    }

}
#endif