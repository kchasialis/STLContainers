#pragma once

#include <functional>
#include <utility>
#include <cstddef>
#include <iterator>
#include <cstdint>
#include <iostream>

namespace adt {

    #define map_node typename map<K, V, Less>::MapNode*

    template<typename K, typename V, class Less = std::less<K>>
    class map {
    private:
        enum {
            RED,
            BLACK
        };
        enum MapBalanceType {
            DELETION,
            INSERTION
        };

        class MapNode {
        private:
            friend class map<K, V, Less>;
            friend class map_iterator;

            enum {
                RED,
                BLACK
            };

            typedef std::pair<const K, V> value_type;

            value_type data;
            MapNode* left;
            MapNode* right;
            MapNode* parent;
            int8_t color;

            MapNode() : data(), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
            MapNode(const K& key, const V& value) : data(std::make_pair(key, value)), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
            MapNode(const K& key, V&& value) : data(std::make_pair(key, std::forward<V>(value))), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
            MapNode(K&& key, V&& value) : data(std::make_pair(std::forward<const K>(key), std::forward<V>(value))), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
            MapNode(K&& key, const V& value) : data(std::make_pair(std::forward<const K>(key), value)), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
            explicit MapNode(const value_type& value) : data(value), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
            explicit MapNode(const MapNode& node) = default;
            explicit MapNode(MapNode&& node) = default;

            MapNode& operator=(MapNode&& node) = default;
        };

        MapNode* root;
        MapNode* endNode;
        Less less;
        size_t _size;
        void rotate_left(MapNode *);
        void rotate_right(MapNode *);
        MapNode* _map_parent_of(MapNode* node);
        MapNode* _map_grandparent_of(MapNode* node);
        MapNode* map_left_of(MapNode* node);
        MapNode* map_right_of(MapNode* node);
        void _map_assign_color(MapNode* node, char color);
        char _map_color_of(MapNode* node);
        MapNode* bst_insert(bool& added_new, const K& key, const V& value);
        MapNode* bst_insert(bool& added_new, const K& key, V&& value);
        MapNode* bst_insert(bool& added_new, K&& key, V&& value);
        MapNode* bst_insert(bool& added_new, K&& key, const V& value);
        MapNode* _copy_tree(MapNode *);
        MapNode* _remove(MapNode *, MapNode*);
        MapNode* _find_bound(MapNode *, const K&);
        inline bool _is_equal_key(const K&, const K&);
        void restore_balance(MapNode *, int8_t type);
        void tree_destroy(MapNode *);
    public:
        class map_iterator : public std::iterator<std::bidirectional_iterator_tag, V, std::ptrdiff_t, V*, V&> {
        protected:
            MapNode* ptr;
            friend class map<K, V, Less>;
        public:
            using value_type = std::pair<const K, V>;

            map_iterator(MapNode* rhs = nullptr) : ptr(rhs) {}
            map_iterator(const map_iterator& rhs) : ptr(rhs.ptr) {}
            
            map_iterator& operator=(const map_iterator& rhs) {
                this->ptr = rhs.ptr;
                return *this;
            }
            map_iterator& operator=(MapNode* rhs) {
                this->ptr = rhs;
                return *this;
            }

            bool operator==(const map_iterator& rhs) const {
                return this->ptr == rhs.ptr;
            }
            bool operator!=(const map_iterator& rhs) const {
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
            map_iterator& operator++ () {
              if (this->ptr == nullptr) {
                 return *this;
              }

              MapNode* current;
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

            map_iterator operator++(int) {
              map_iterator tmp (*this);
              ++(*this);

              return tmp;
            }

            /*Inorder predecessor algorithm*/
            map_iterator& operator-- () {
              
              if (this->ptr == nullptr) {
                return map_iterator();
              }

              MapNode* current;
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

            void swap(map_iterator& lhs, map_iterator& rhs) {
                std::swap(lhs, rhs);
            }
        };

        class map_r_iterator : public map_iterator {
        public:
            map_r_iterator (MapNode* _ptr = nullptr) : map_iterator (_ptr) {}
            map_r_iterator (const map_r_iterator& other) = default;
            
            map_r_iterator& operator= (const map_r_iterator& other) = default;
            map_r_iterator& operator= (MapNode* _ptr) {
                this->ptr = _ptr;
                return *this;
            }

            /* Inorder predecessor algorithm.  */
            map_r_iterator& operator++ () {
                if (this->ptr == nullptr) {
                    return *this;
                }

                MapNode* current;
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

            map_r_iterator operator++ (int) {
                map_r_iterator tmp(*this);
                ++(*this);

                return tmp;
            }

            /* Inorder successor algorithm.  */
            map_r_iterator& operator-- () {
                
                if (this->ptr == nullptr) {
                    return *this;
                }
                MapNode* current;
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

            map_r_iterator operator-- (int) {
                map_r_iterator tmp (*this);
                --(*this);

                return tmp;
            }
        };

        map () noexcept;
        map (const map& rhs) noexcept;
        map (map&& rhs) noexcept;
        ~map();

        map& operator= (map x);
        
        bool add (const K& key, const V& value);
        bool add (const K& key, V&& value);
        bool add (K&& key, V&& value);
        bool add (K&& key, const V& value);
        bool remove (const K& key);
        map_iterator remove (map_iterator itr);
        void clear () noexcept;
        void clear () const noexcept;

        bool empty ();
        size_t size ();

        map_iterator lower_bound (const K& key);
        map_iterator upper_bound (const K& key);
        map_iterator begin ();
        map_iterator end ();
        map_r_iterator rbegin ();
        map_r_iterator rend ();
        map_iterator search (const K& key);
        V& operator[] (const K& key);
        const V& at (const K& key) const;
        V& at (const K& key);

        friend void swap (map& lhs, map& rhs) {
            using std::swap;

            swap(lhs.root, rhs.root);
            swap(lhs.endNode, rhs.endNode);
            swap(lhs.less, rhs.less);
            swap(lhs._size, rhs._size);
        }
    };

    #define map_parent_of(node) _map_parent_of(node)
    #define map_grandparent_of(node) _map_grandparent_of(node)
    #define map_assign_color(node, color) _map_assign_color(node, color)
    #define map_color_of(node) _map_color_of(node)

    #define map_balance_insertion(side, rotate_1, rotate_2)                 \
        do  {                                                               \
                {                                                           \
                    map_node uncle = side(map_grandparent_of(node));  \
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
                    MapNode* sibling = side1(map_parent_of(node));    \
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

    template<typename K, typename V, class Less>
    map<K, V, Less>::map() noexcept : root(nullptr), endNode(nullptr) , _size(0) {}

    template<typename K, typename V, class Less>
    map_node map<K, V, Less>::_copy_tree(MapNode* other_root) {
        if (other_root == nullptr) {
            return nullptr;
        }

        MapNode* new_node = new MapNode(other_root->get_value());

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

    template<typename K, typename V, class Less>
    map<K, V, Less>::map(const map& other) noexcept {
        /* Create an exact copy of this map, O(n). */
        this->root = _copy_tree(other.root);
        this->endNode = new MapNode();
        this->endNode->left = this->root;
        this->root->parent = this->endNode;
    }

    template<typename K, typename V, class Less>
    map<K, V, Less>::map(map&& other) noexcept : map() {
        swap(*this, other);
    }

    template<typename K, typename V, class Less>
    map<K, V, Less>& map<K, V, Less>::operator=(map other) {
        /* Copy and swap idiom, let the compiler handle the copy of the argument. */
        swap(*this, other);

        return *this;
    }

    template<typename K, typename V, class Less>
    void map<K, V, Less>::tree_destroy(map_node current) {

        if (!current) {
            return;
        }

        tree_destroy(current->left);
        tree_destroy(current->right);

        delete current;
    }

    template<typename K, typename V, class Less>
    map<K, V, Less>::~map() {
        if (this->root) {
            tree_destroy(this->root);
            delete this->endNode;
        }
    }

    template<typename K, typename V, class Less>
    map_node map<K, V, Less>::bst_insert(bool &added_new, const K& key, const V& value) {

        if (empty()) {
            added_new = true;
            this->root = new MapNode(key, value);
            this->endNode = new MapNode();
            this->endNode->left = this->root;
            this->root->parent = this->endNode;
            return this->root;
        }

        map_node current = this->root;
        while (1) {

            if (less(key, current->data.first)) {
                if (current->left != nullptr) {
                    current = current->left;
                }
                else {
                    added_new = true;
                    map_node new_node = new MapNode(key, value);
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
                    map_node new_node = new MapNode(key, value);
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

    template<typename K, typename V, class Less>
    map_node map<K, V, Less>::bst_insert(bool &added_new, const K& key, V&& value) {

        if (empty()) {
            added_new = true;
            this->root = new MapNode(key, std::forward<V>(value));
            this->endNode = new MapNode();
            this->endNode->left = this->root;
            this->root->parent = this->endNode;
            return this->root;
        }

        map_node current = this->root;
        while (1) {

            if (less(key, current->data.first)) {
                if (current->left != nullptr) {
                    current = current->left;
                }
                else {
                    added_new = true;
                    map_node new_node = new MapNode(key, std::forward<V>(value));
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
                    map_node new_node = new MapNode(key, std::forward<V>(value));
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

    template<typename K, typename V, class Less>
    map_node map<K, V, Less>::bst_insert(bool &added_new, K&& key, const V& value) {
        if (empty()) {
            added_new = true;
            this->root = new MapNode(std::forward<K>(key), value);
            this->endNode = new MapNode();
            this->endNode->left = this->root;
            this->root->parent = this->endNode;
            return this->root;
        }

        map_node current = this->root;
        while (1) {

            if (less(key, current->data.first)) {
                if (current->left != nullptr) {
                    current = current->left;
                }
                else {
                    added_new = true;
                    map_node new_node = new MapNode(std::forward<K>(key), value);
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
                    map_node new_node = new MapNode(std::forward<K>(key), value);
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


    template<typename K, typename V, class Less>
    map_node map<K, V, Less>::bst_insert(bool &added_new, K&& key, V&& value) {

        if (empty()) {
            added_new = true;
            this->root = new MapNode(std::forward<K>(key), std::forward<V>(value));
            this->endNode = new MapNode();
            this->endNode->left = this->root;
            this->root->parent = this->endNode;
            return this->root;
        }

        map_node current = this->root;
        while (1) {

            if (less(key, current->data.first)) {
                if (current->left != nullptr) {
                    current = current->left;
                }
                else {
                    added_new = true;
                    map_node new_node = new MapNode(std::forward<K>(key), std::forward<V>(value));
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
                    map_node new_node = new MapNode(std::forward<K>(key), std::forward<V>(value));
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

    template<typename K, typename V, class Less>
    bool map<K, V, Less>::add(const K& key, const V& value) {

        bool added_new;
        map_node current = bst_insert(added_new, key, value);

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

    template<typename K, typename V, class Less>
    bool map<K, V, Less>::add(const K& key, V&& value) {

        bool added_new;
        map_node current = bst_insert(added_new, key, value);

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

    template<typename K, typename V, class Less>
    bool map<K, V, Less>::add(K&& key, const V& value) {

        bool added_new;
        map_node current = bst_insert(added_new, key, value);

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

    template<typename K, typename V, class Less>
    bool map<K, V, Less>::add(K&& key, V&& value) {

        bool added_new;
        map_node current = bst_insert(added_new, std::forward<K>(key), std::forward<V>(value));

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

    template<typename K, typename V, class Less>
    void map<K, V, Less>::clear() noexcept {
        if (this->root) {
            tree_destroy(this->root);
            delete this->endNode;
            this->root = nullptr;
            this->endNode = nullptr;
            this->_size = 0;
        }
    }

    template<typename K, typename V, class Less>
    void map<K, V, Less>::clear() const noexcept {
        if (this->root) {
            tree_destroy(this->root);
            delete this->endNode;
            this->root = nullptr;
            this->endNode = nullptr;
            this->_size = 0;
        }
    }

    template<typename K, typename V, class Less>
    bool map<K, V, Less>::empty() {
        return this->root == nullptr;
    }

    template<typename K, typename V, class Less>
    void map<K, V, Less>::restore_balance(MapNode* node, int8_t type) {

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

    template<typename K, typename V, class Less>
    map_node map<K, V, Less>::_remove(map_node current, map_node successor) {

        /*If this node is not a leaf and has both children*/
        if (current->left != nullptr && current->right != nullptr) {
            /*Get the minimum value of the right subtree*/
            using std::swap;
            swap((K&) current->data.first, (K&) successor->data.first);
            swap(current->data.second, successor->data.second);
            current = successor;
        }

        map_node r_node = current->left != nullptr ? current->left : current->right;

        /*If node has one children*/
        if (r_node != nullptr) {
            r_node->parent = current->parent;
            map_node parent_node = current->parent;
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

            if (map_color_of (current) == BLACK) {
                /*Balance only if its a black node*/
                restore_balance (r_node, DELETION);
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
            if (map_color_of (current) == BLACK) {
                /*Balance only if its a black node*/
                restore_balance (current, DELETION);
            }

            MapNode* parent_node = current->parent;
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

    template<typename K, typename V, class Less>
    typename map<K, V, Less>::map_iterator map<K, V, Less>::begin() {
        MapNode* current = this->root;
        if (current == nullptr) {
            return map_iterator();
        }
        while (current->left != nullptr) {
            current = current->left;
        }

        return map_iterator(current);
    }

    template<typename K, typename V, class Less>
    typename map<K, V, Less>::map_iterator map<K, V, Less>::end() {
        return map_iterator(this->endNode);
    }

    template<typename K, typename V, class Less>
    typename map<K, V, Less>::map_r_iterator map<K, V, Less>::rbegin() {
        MapNode *current = this->root;
        while (current && current->right) {
            current = current->right;
        }

        return map_r_iterator(current);
    }

    template<typename K, typename V, class Less>
    typename map<K, V, Less>::map_r_iterator map<K, V, Less>::rend() {
        return map_r_iterator(this->endNode);
    }

    template<typename K, typename V, class Less>
    inline bool map<K, V, Less>::_is_equal_key(const K& lhs, const K& rhs) {
        if (less(lhs, rhs)) {
            return false;
        }
        else if (less(rhs, lhs)) {
            return false;
        }
        return true;
    }

    template<typename K, typename V, class Less>
    map_node map<K, V, Less>::_find_bound(map_node _root, const K &key) {
        if (_root == nullptr) {
            return nullptr;
        }

        if (_is_equal_key(_root->data.first, key)) {
            return _root;
        }

        if (less(_root->data.first, key)) {
            return _find_bound(_root->right, key);
        }

        map_node retnode = _find_bound(_root->left, key);
        return retnode == nullptr ? _root : retnode;
    }

    template<typename K, typename V, class Less>
    typename map<K, V, Less>::map_iterator map<K, V, Less>::upper_bound(const K& key) {
        map_node retnode = _find_bound(this->root, key);
        if (retnode == nullptr) {
            return this->end();
        }
        else {
            return ++typename map<K, V, Less>::map_iterator(retnode);
        }
    }

    template<typename K, typename V, class Less>
    typename map<K, V, Less>::map_iterator map<K, V, Less>::lower_bound(const K& key) {
        map_node retnode = _find_bound(this->root, key);
        if (retnode == nullptr) {
            return this->end();
        }
        else {
            return typename map<K, V, Less>::map_iterator(retnode);
        }
    }


    template<typename K, typename V, class Less>
    bool map<K, V, Less>::remove(const K &key) {

        MapNode* current = nullptr;
        auto result = search(key);

        if (result == end()) {
            return false;
        }

        MapNode* save = this->endNode;
        this->root->parent = nullptr;

        MapNode* successor =  (++typename map<K, V, Less>::map_iterator(result)).ptr;
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

    template<typename K, typename V, class Less>
    typename map<K, V, Less>::map_iterator map<K, V, Less>::remove(typename map<K, V, Less>::map_iterator itr) {

        if (itr == end()) {
            return itr;
        }

        MapNode* save = this->endNode;
        this->root->parent = nullptr;

        MapNode* successor =  (++typename map<K, V, Less>::map_iterator(itr)).ptr;
        MapNode* current = _remove(itr.ptr, successor);

        if (current != nullptr) {
            typename map<K, V, Less>::map_iterator retrnValue(current == successor ? itr.ptr : successor);

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

    template<typename K, typename V, class Less>
    typename map<K, V, Less>::map_iterator map<K, V, Less>::search(const K &key) {

        if (this->root == nullptr) {
            return this->end();
        }
        map_node current = this->root;
        while (current) {
            if (less(key, current->data.first)) {
                current = current->left;
            }
            else if (less(current->data.first, key)) {
                current = current->right;
            }
            else {
                return typename map<K, V, Less>::map_iterator(current);
            }
        }

        return this->end();
    }

    template<typename K, typename V, class Less>
    V& map<K, V, Less>::operator[](const K& key) {

        bool added_new;
        map_node current = bst_insert(added_new, key, V());

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

    template<typename K, typename V, class Less>
    const V& map<K, V, Less>::at(const K& key) const {

        auto it = search(key);
        if (it == end()) {
            throw std::out_of_range("Element is not present on map");
        }

        return it->second;
    }

    template<typename K, typename V, class Less>
    V& map<K, V, Less>::at(const K& key) {

        auto it = search(key);
        if (it == end()) {
            throw std::out_of_range("Element is not present on map");
        }

        return it->second;
    }

    template<typename K, typename V, class Less>
    void map<K, V, Less>::rotate_right(map_node node) {

        if (!node) {
            return;
        }

        map_node left_child = node->left;
        map_node left_right_child = left_child->right;

        node->left = left_right_child;
        if (left_right_child) {
            left_right_child->parent = node;
        }

        map_node node_p = node->parent;
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

    template<typename K, typename V, class Less>
    void map<K, V, Less>::rotate_left(map_node node) {

        if (!node) {
            return;
        }

        map_node right_child = node->right;
        map_node right_left_child = right_child->left;

        node->right = right_left_child;
        if (right_left_child) {
            right_left_child->parent = node;
        }

        MapNode  *node_p = node->parent;
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

    template<typename K, typename V, class Less>
    map_node map<K, V, Less>::_map_parent_of(map_node node) {
        return node ? node->parent : nullptr;
    }

    template<typename K, typename V, class Less>
    map_node map<K, V, Less>::_map_grandparent_of(map_node node) {
        if (node) {
            if (node->parent) {
                return node->parent->parent;
            }
        }
        return nullptr;
    }

    template<typename K, typename V, class Less>
    map_node map<K, V, Less>::map_left_of(map_node node) {
        return node ? node->left : nullptr;
    }

    template<typename K, typename V, class Less>
    map_node map<K, V, Less>::map_right_of(map_node node) {
        return node ? node->right : nullptr;
    }

    template<typename K, typename V, class Less>
    void map<K, V, Less>::_map_assign_color(map_node node, char color) {
        if (node) {
            node->color = color;
        }
    }

    template<typename K, typename V, class Less>
    char map<K, V, Less>::_map_color_of(map_node node) {
        return node ? node->color : (int8_t) MapNode::BLACK;
    }

    template<typename K, typename V, class Less>
    size_t map<K, V, Less>::size() {
        return this->_size;
    }
}