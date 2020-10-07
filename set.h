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

        using key_type = T;
        using value_type = T;
        using key_compare = Less;
        using size_type = size_t;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = T*;
        using const_pointer = const T*;

        class SetNode {
        private:
            friend class set<T, Less>;

            enum {
                RED,
                BLACK
            };

            value_type _val;
            SetNode *_left;
            SetNode *_right;
            SetNode *_parent;
            int8_t color;

            SetNode() : _val(), _left(nullptr), _right(nullptr), _parent(nullptr), color(RED) {}
            explicit SetNode(const value_type &val) : _val(val), _left(nullptr), _right(nullptr), _parent(nullptr), color(RED) {}
            explicit SetNode(value_type &&val) : _val(std::forward<T>(val)), _left(nullptr), _right(nullptr), _parent(nullptr), color(RED) {}
            explicit SetNode(const SetNode &node) = default;
            explicit SetNode(SetNode &&node) = default;

            SetNode& operator=(SetNode &&node) = default;
        };

        class set_iterator : public std::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t , T*, T&> {
        public:
            using const_data_reference = const T&;
            using const_data_pointer = const T*;

            set_iterator(SetNode *ptr = nullptr) : _ptr(ptr) {}
            set_iterator(const set_iterator &rhs) : _ptr(rhs._ptr) {}

            set_iterator& operator=(const set_iterator &rhs) = default;
            set_iterator& operator=(SetNode *rhs) {
                _ptr = rhs;
                return *this;
            }

            bool operator==(const set_iterator &rhs) const {
                return _ptr == rhs._ptr;
            }
            bool operator!=(const set_iterator &rhs) const {
                return !(*this == rhs);
            }

            /* Inorder successor algorithm. */
            set_iterator &operator++() {
                SetNode *current;

                if (_ptr == nullptr) {
                    return *this;
                }

                if (_ptr->_right) {
                    current = _ptr->_right;
                    while (current->_left != nullptr) {
                        current = current->_left;
                    }
                }
                else {
                    current = _ptr->_parent;
                    while (current != nullptr && _ptr == current->_right) {
                        _ptr = current;
                        current = current->_parent;
                    }
                }

                _ptr = current;
                return *this;
            }
            set_iterator operator++(int) {
                set_iterator tmp(*this);
                ++(*this);

                return tmp;
            }

            /* Inorder predecessor algorithm. */
            set_iterator& operator--() {
                SetNode *current;

                if (_ptr == nullptr) {
                    return *this;
                }

                if (_ptr->_left) {
                    current = _ptr->_left;
                    while (current->_right != nullptr) {
                        current = current->_right;
                    }
                }
                else {
                    current = _ptr->_parent;
                    while (current->_parent != nullptr && _ptr == current->_left) {
                        _ptr = current;
                        current = current->_parent;
                    }
                }

                _ptr = current;
                return *this;
            }
            set_iterator operator--(int) {
                set_iterator tmp(*this);
                --(*this);

                return tmp;
            }

            const_data_reference operator*() const {
                return _ptr->_val;
            }
            const_data_pointer operator->() const {
                return &(_ptr->_val);
            }

            void swap(set_iterator &lhs, set_iterator& rhs) {
                std::swap(lhs, rhs);
            }

        private:
            friend class set<T, Less>;
            SetNode *_ptr;
        };

        class set_r_iterator : public set_iterator {
        public:
            set_r_iterator(SetNode *ptr = nullptr) : set_iterator(ptr) {}
            set_r_iterator(const set_r_iterator &itr) = default;

            set_r_iterator& operator=(const set_r_iterator &itr) = default;
            set_r_iterator& operator=(SetNode *ptr) {
                this->_ptr = ptr;
                return *this;
            }

            /* Inorder predecessor algorithm. */
            set_r_iterator& operator++() {
                SetNode* current;

                if (this->_ptr == nullptr) {
                    return *this;
                }

                if (this->_ptr->_left) {
                    current = this->_ptr->_left;
                    while (current->_right != nullptr) {
                        current = current->_right;
                    }
                }
                else {
                    current = this->_ptr->_parent;
                    while (current->_parent != nullptr && this->_ptr == current->_left) {
                        this->_ptr = current;
                        current = current->_parent;
                    }
                }

                this->_ptr = current;
                return *this;
            }

            /* Inorder successor algorithm. */
            set_r_iterator& operator--() {
                if (this->_ptr == nullptr) {
                    return *this;
                }
                SetNode* current;
                if (this->_ptr->_right) {
                    current = this->_ptr->_right;
                    while (current->_left != nullptr) {
                        current = current->_left;
                    }
                }
                else {
                    current = this->_ptr->_parent;
                    while (current != nullptr && this->_ptr == current->_right) {
                        this->_ptr = current;
                        current = current->_parent;
                    }
                }

                this->_ptr = current;
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

        using const_iterator = set_iterator;
        using const_r_iterator = set_r_iterator;


        SetNode *_root;
        SetNode *_end_node;
        Less _less;
        size_t _size;

        void _rotate_left(SetNode *tnode);
        void _rotate_right(SetNode *tnode);
        SetNode *_set_parent_of(SetNode *tnode);
        SetNode *_set_grand_parent_of(SetNode *tnode);
        SetNode *_set_left_of(SetNode *tnode);
        SetNode *_set_right_of(SetNode *tnode);
        void _set_assign_color(SetNode *tnode, char color);
        char _set_color_of(SetNode *tnode);
        std::pair<const_iterator, bool>_update_tree(SetNode *tnode, bool added_new);
        SetNode *_bst_insert(bool &added_new, const value_type &val);
        SetNode *_bst_insert(bool &added_new, value_type &&val);
        SetNode *_copy_tree(SetNode *other_root);
        void _tree_destroy(SetNode *current);
        SetNode *_remove(SetNode *current, SetNode *successor);
        SetNode *_successor(SetNode *tode) const;
        bool _is_equal_key(const key_type &lhs_key, const key_type &rhs_key) const;
        SetNode *_find_bound(SetNode *_root, const key_type &key) const;
        void _restore_balance(SetNode *tnode, int8_t type);
    public:
        /* Constructors/Destructors.  */
        set() noexcept;
        explicit set(const set &rhs) noexcept;
        set(set&& rhs) noexcept;
        set& operator=(set rhs);
        ~set();

        /* Capacity.  */
        bool empty() const noexcept;
        size_type size() const noexcept;

        /* Iterators.  */
        const_iterator begin() const noexcept;
        const_iterator end() const noexcept;
        const_r_iterator rbegin() const noexcept;
        const_r_iterator rend() const noexcept;

        /* Observers.  */
        key_compare key_comp() const;

        /* Modifiers.  */
        std::pair<const_iterator, bool> insert(const value_type &val);
        std::pair<const_iterator, bool> insert(value_type &&val);
        template <class... Args>
        std::pair<const_iterator, bool> emplace(Args &&... args);
        const_iterator erase(const_iterator pos);
        size_type erase(const value_type &val);
        const_iterator erase(const_iterator first, const_iterator last);
        void clear() noexcept;
        void swap(set& other);

        /* Element lookup.  */
        const_iterator find(const key_type &key) const;
        const_iterator lower_bound(const key_type &key) const;
        const_iterator upper_bound(const key_type &key) const;


        /* Math operations. */
        /*TODO : join, intersection, difference */
        set& join(const set &other);
        set& intersection(const set &other);
        set& difference(const set &other);

        friend void swap(set &lhs, set &rhs) {
            using std::swap;

            swap(lhs._root, rhs._root);
            swap(lhs._end_node, rhs._end_node);
            swap(lhs._less, rhs._less);
            swap(lhs._size, rhs._size);
        }
    };

    /* Implementation.  */
    #define set_parent_of(tnode) _set_parent_of(tnode)
    #define set_grand_parent_of(tnode) _set_grand_parent_of(tnode)
    #define set_assign_color(tnode, color) _set_assign_color(tnode, color)
    #define set_color_of(tnode) _set_color_of(tnode)

    #define _set_balance_insertion(side, rotate_1, rotate_2)         \
    do  {                                                           \
            {                                                       \
                SetNode *uncle = side(set_grand_parent_of(tnode)); \
                                                                    \
                if (set_color_of(uncle) == RED) {                   \
                    /*Uncle RED means color-flip*/                  \
                                                                    \
                    set_assign_color(set_parent_of(tnode), BLACK);   \
                    set_assign_color(set_grand_parent_of(tnode), RED);\
                    set_assign_color(uncle, BLACK);                 \
                    tnode = set_grand_parent_of(tnode);                \
                }                                                   \
                else {                                              \
                    /*Uncle BLACK means rotations*/                 \
                    if (tnode == side(set_parent_of(tnode))) {        \
                        tnode = set_parent_of(tnode);                 \
                        rotate_2(tnode);                             \
                    }                                               \
                    set_assign_color(set_parent_of(tnode), BLACK);   \
                    set_assign_color(set_grand_parent_of(tnode), RED);\
                    rotate_1(set_grand_parent_of(tnode));             \
                }                                                   \
            }                                                       \
    } while(0)

    #define _set_balance_deletion(side1, side2, rotate_side1, rotate_side2)   \
    do {                                                                 \
            {                                                            \
                SetNode* sibling = side1(set_parent_of(tnode));        \
                if (set_color_of(sibling) == RED) {                      \
                    set_assign_color(sibling, BLACK);                    \
                    set_assign_color(set_parent_of(tnode), RED);          \
                    rotate_side1(set_parent_of(tnode));                   \
                    sibling = side1(set_parent_of(tnode));                \
                }                                                        \
                if (set_color_of(side2(sibling)) == BLACK && set_color_of(side1(sibling)) == BLACK) { \
                    set_assign_color(sibling, RED);                      \
                    tnode = set_parent_of(tnode);                          \
                }                                                        \
                else {                                                   \
                    if (set_color_of(side1(sibling)) == BLACK) {         \
                        set_assign_color(side2(sibling), BLACK);         \
                        set_assign_color(sibling, RED);                  \
                        rotate_side2(sibling);                           \
                        sibling = side1(set_parent_of(tnode));            \
                    }                                                    \
                    set_assign_color(sibling, set_color_of(set_parent_of(tnode)));\
                    set_assign_color(set_parent_of(tnode), BLACK);        \
                    set_assign_color(side1(sibling), BLACK);             \
                    rotate_side1(set_parent_of(tnode));                   \
                    tnode = _root;                                   \
                }                                                        \
            }                                                            \
    } while(0)


    /* Public member functions.  */

    template<typename T, class Less>
    set<T, Less>::set() noexcept : _root(nullptr), _end_node(nullptr) , _size(0) {}

    template<typename T, class Less>
    set<T, Less>::set(const set &other) noexcept {
        /*Create an exact copy of this set, O(n)*/
        _root = _copy_tree(other._root);
        _end_node = new SetNode();
        _end_node->_left = _root;
        _root->_parent = _end_node;
    }

    template<typename T, class Less>
    set<T, Less>::set(set &&other) noexcept : set() {
        swap(*this, other);
    }

    template<typename T, class Less>
    set<T, Less>& set<T, Less>::operator=(set rhs) {
        /* Copy and swap idiom, let the compiler handle the copy of the argument.  */
        swap(*this, rhs);

        return *this;
    }

    template<typename T, class Less>
    set<T, Less>::~set() {
        if (_root) {
            _tree_destroy(_root);
            delete _end_node;
        }
    }


    template<typename T, class Less>
    bool set<T, Less>::empty() const noexcept {
        return _root == nullptr;
    }

    template<typename T, class Less>
    size_t set<T, Less>::size() const noexcept {
        return _size;
    }


    template<typename T, class Less>
    typename set<T, Less>::const_iterator set<T, Less>::begin() const noexcept {
        SetNode *current = _root;
        if (current == nullptr) {
            return const_iterator();
        }
        while (current->_left != nullptr) {
            current = current->_left;
        }

        return const_iterator(current);
    }

    template<typename T, class Less>
    typename set<T, Less>::const_iterator set<T, Less>::end() const noexcept {
        return const_iterator(_end_node);
    }

    template<typename T, class Less>
    typename set<T, Less>::const_r_iterator set<T, Less>::rbegin() const noexcept {
        SetNode *current = _root;
        while (current && current->_right) {
            current = current->_right;
        }

        return const_r_iterator(current);
    }

    template<typename T, class Less>
    typename set<T, Less>::const_r_iterator set<T, Less>::rend() const noexcept {
        return const_r_iterator(_end_node);
    }

    template<typename T, class Less>
    typename set<T, Less>::key_compare set<T, Less>::key_comp() const {
        return _less;
    }

    template<typename T, class Less>
    std::pair<typename set<T, Less>::const_iterator, bool> set<T, Less>::insert(const value_type &val) {
        bool added_new;
        SetNode *current = _bst_insert(added_new, val);

        if (!added_new) {
            return {nullptr, false};
        }

        _restore_balance(current, INSERTION);

        this->_root->color = BLACK;
        this->_size++;
        this->_end_node->_left = this->_root;
        this->_root->_parent = this->_end_node;

        return {nullptr, true};
    }

    template<typename T, class Less>
    std::pair<typename set<T, Less>::const_iterator, bool> set<T, Less>::insert(value_type &&key) {
        bool added_new;
        SetNode *current = _bst_insert(added_new, std::forward<T>(key));

        if (!added_new) {
            return {nullptr, false};
        }

        _restore_balance(current, INSERTION);

        this->_root->color = BLACK;
        this->_size++;
        this->_root->_parent = this->_end_node;
        this->_end_node->_left = this->_root;

        return {nullptr, true};

    }

    template<typename T, class Less>
    template<class... Args>
    std::pair<typename set<T, Less>::const_iterator, bool> set<T, Less>::emplace(Args &&... args) {
        bool added_new;
        SetNode *current, *new_node;
        auto* val = new value_type(std::forward<Args>(args)...);


        if (empty()) {
            added_new = true;
            _root = new SetNode(val);
            _end_node = new SetNode();
            _end_node->_left = _root;
            _root->_parent = _end_node;

            return _update_tree(_root, added_new);
        }

        current = _root;
        while (true) {

            if (_less(val, current->_val)) {
                if (current->_left != nullptr) {
                    current = current->_left;
                }
                else {
                    added_new = true;
                    new_node = new SetNode(val);
                    current->_left = new_node;
                    new_node->_parent = current;

                    return _update_tree(new_node, added_new);
                }
            }
            else if (_less(current->_val, val)) {
                if (current->_right != nullptr) {
                    current = current->_right;
                }
                else {
                    added_new = true;
                    new_node = new SetNode(val);
                    current->_right = new_node;
                    new_node->_parent = current;

                    return _update_tree(new_node, added_new);
                }
            }
            else {
                added_new = false;
                return _update_tree(current, added_new);
            }
        }
    }

    template<typename T, class Less>
    typename set<T, Less>::const_iterator set<T, Less>::erase(set::const_iterator pos) {
        SetNode *save, *successor, *current;
        const_iterator ret_value;

        if (pos == end()) {
            return pos;
        }

        save = _end_node;
        _root->_parent = nullptr;

        successor =  (++set_iterator(pos))._ptr;
        current = _remove(pos._ptr, successor);

        if (current != nullptr) {
            ret_value = current == successor ? pos._ptr : successor;

            delete current;

            /*Update the secret node*/
            _end_node = save;
            _root->_parent = _end_node;
            _end_node->_left = _root;
            _size--;

            if (successor != nullptr) {
                return ret_value;
            }
            else {
                return end();
            }
        }

        return end();
    }

    template<typename T, class Less>
    typename set<T, Less>::size_type set<T, Less>::erase(const value_type &key) {
        SetNode *current, *save, *successor;
        auto result = search(key);

        if (result == end()) {
            return 0;
        }

        save = _end_node;
        _root->_parent = nullptr;

        successor =  (++set_iterator(result)).ptr;
        current = _remove(result.ptr, successor);

        if (current != nullptr) {
            delete current;

            /*Update the secret node*/
            _end_node = save;
            _root->_parent = _end_node;
            _end_node->_left = _root;
            _size--;
        }

        return 1;
    }

    template<typename T, class Less>
    typename set<T, Less>::const_iterator set<T, Less>::erase(set::const_iterator first, set::const_iterator last) {
        for (auto it = first ; it != last ; ) {
            it = erase(it);
        }
        return last;
    }

    template<typename T, class Less>
    void set<T, Less>::clear() noexcept {
        if (_root) {
            _tree_destroy(_root);
            delete _end_node;
            _root = nullptr;
            _end_node = nullptr;
            _size = 0;
        }
    }

    template<typename T, class Less>
    void set<T, Less>::swap(set& other) {
        swap(*this, other);
    }

    template<typename T, class Less>
    typename set<T, Less>::const_iterator set<T, Less>::find(const key_type &key) const {
        SetNode *current;

        if (_root == nullptr) {
            return end();
        }

        current = _root;
        while (current) {
            if (_less(key, current->_val)) {
                current = current->_left;
            }
            else if (_less(current->_val, key)) {
                current = current->_right;
            }
            else {
                return const_iterator(current);
            }
        }

        return end();
    }

    template<typename T, class Less>
    typename set<T, Less>::const_iterator set<T, Less>::lower_bound(const key_type &key) const {
        SetNode *retnode = _find_bound(_root, key);

        if (retnode == nullptr) {
            return end();
        }
        else {
            return const_iterator(retnode);
        }
    }

    template<typename T, class Less>
    typename set<T, Less>::const_iterator set<T, Less>::upper_bound(const key_type &key) const {
        SetNode *retnode = _find_bound(_root, key);

        if (retnode == nullptr) {
            return end();
        }
        else {
            if (_is_equal_key(retnode->_val, key)) {
                return const_iterator(_successor(retnode));
            }
            else {
                return const_iterator(retnode);
            }
        }
    }

    /* Private member functions.  */

    template<typename T, class Less>
    void set<T, Less>::_rotate_left(SetNode *tnode) {
        SetNode *_right_child, *_right_left_child, *p_node;

        if (!tnode) {
            return;
        }

        _right_child = tnode->_right;
        _right_left_child = _right_child->_left;

        tnode->_right = _right_left_child;
        if (_right_left_child) {
            _right_left_child->_parent = tnode;
        }

        p_node = tnode->_parent;
        _right_child->_parent = p_node;

        if (p_node == nullptr) {
            _root = _right_child;
        }
        else if (p_node->_left == tnode) {
            p_node->_left = _right_child;
        }
        else {
            p_node->_right = _right_child;
        }

        _right_child->_left = tnode;
        tnode->_parent = _right_child;
    }

    template<typename T, class Less>
    void set<T, Less>::_rotate_right(SetNode *tnode) {
        SetNode *_left_child, *_left_right_child, *p_node;

        if (!tnode) {
            return;
        }

        _left_child = tnode->_left;
        _left_right_child = _left_child->_right;

        tnode->_left = _left_right_child;
        if (_left_right_child) {
            _left_right_child->_parent = tnode;
        }

        p_node = tnode->_parent;
        _left_child->_parent = p_node;

        if (p_node == nullptr) {
            _root = _left_child;
        }
        else if (p_node->_right == tnode) {
            p_node->_right = _left_child;
        }
        else {
            p_node->_left = _left_child;
        }

        _left_child->_right = tnode;
        tnode->_parent = _left_child;
    }

    template<typename T, class Less>
    set_node set<T, Less>::_set_parent_of(set_node tnode) {
        return tnode ? tnode->_parent : nullptr;
    }

    template<typename T, class Less>
    set_node set<T, Less>::_set_grand_parent_of(set_node tnode) {
        if (tnode) {
            if (tnode->_parent) {
                return tnode->_parent->_parent;
            }
        }
        return nullptr;
    }

    template<typename T, class Less>
    set_node set<T, Less>::_set_left_of(set_node tnode) {
        return tnode ? tnode->_left : nullptr;
    }

    template<typename T, class Less>
    set_node set<T, Less>::_set_right_of(set_node tnode) {
        return tnode ? tnode->_right : nullptr;
    }
    template<typename T, class Less>
    void set<T, Less>::_set_assign_color(set_node tnode, char color) {
        if (tnode) {
            tnode->color = color;
        }
    }

    template<typename T, class Less>
    char set<T, Less>::_set_color_of(set_node tnode) {
        return tnode ? tnode->color : (int8_t) SetNode::BLACK;
    }

    template<typename T, class Less>
    std::pair<typename set<T, Less>::const_iterator, bool> set<T, Less>::_update_tree(SetNode *tnode, bool added_new) {
        if (!added_new) {
            return {const_iterator(tnode), added_new};
        }

        _restore_balance(tnode, INSERTION);

        _root->color = BLACK;
        _end_node->_left = _root;
        _root->_parent = _end_node;
        _size++;

        return {const_iterator(tnode), added_new};
    }

    template<typename T, class Less>
    set_node set<T, Less>::_bst_insert(bool &added_new, const value_type &val) {
        SetNode *new_node, *current;

        if (empty()) {
            added_new = true;
            _root = new SetNode(val);
            _end_node = new SetNode();
            _end_node->_left = _root;
            _root->_parent = _end_node;
            return _root;
        }

        current = _root;
        while (true) {

            if (_less(val, current->_val)) {
                if (current->_left != nullptr) {
                    current = current->_left;
                }
                else {
                    added_new = true;
                    new_node = new SetNode(val);
                    current->_left = new_node;
                    new_node->_parent = current;

                    return new_node;
                }
            }
            else if (_less(current->_val, val)) {
                if (current->_right != nullptr) {
                    current = current->_right;
                }
                else {
                    added_new = true;
                    new_node = new SetNode(val);
                    current->_right = new_node;
                    new_node->_parent = current;

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
    set_node set<T, Less>::_bst_insert(bool &added_new, value_type &&val) {
        SetNode *new_node, *current;

        if (empty()) {
            added_new = true;
            _root = new SetNode(std::forward<T>(val));
            _end_node = new SetNode();
            _end_node->_left = _root;
            _root->_parent = _end_node;
            return _root;
        }

        current = _root;
        while (true) {

            if (_less(val, current->_val)) {
                if (current->_left != nullptr) {
                    current = current->_left;
                }
                else {
                    added_new = true;
                    new_node = new SetNode(std::forward<T>(val));
                    current->_left = new_node;
                    new_node->_parent = current;

                    return new_node;
                }
            }
            else if (_less(current->_val, val)) {
                if (current->_right != nullptr) {
                    current = current->_right;
                }
                else {
                    added_new = true;
                    new_node = new SetNode(std::forward<T>(val));
                    current->_right = new_node;
                    new_node->_parent = current;

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
    set_node set<T, Less>::_copy_tree(SetNode *other_root) {
        SetNode *new_node;

        if (other_root == nullptr) {
            return nullptr;
        }

        new_node = new SetNode(other_root->_val);

        new_node->_left = _copy_tree(other_root->_left);
        if (new_node->_left) {
            new_node->_left->_parent = new_node;
        }

        new_node->_right = _copy_tree(other_root->_right);
        if (new_node->_right) {
            new_node->_right->_parent = new_node;
        }

        return new_node;
    }

    template<typename T, class Less>
    void set<T, Less>::_tree_destroy(SetNode *current) {

        if (!current) {
            return;
        }

        _tree_destroy(current->_left);
        _tree_destroy(current->_right);

        delete current;
    }

    template<typename T, class Less>
    set_node set<T, Less>::_remove(SetNode *current, SetNode *successor) {
        SetNode *r_node, *_parent_node;

        /*If this node is not a leaf and has both children*/
        if (current->_left != nullptr && current->_right != nullptr) {
            /*Get the minimum value of the _right subtree*/
            current->_val = successor->_val;
            current = successor;
        }

        r_node = current->_left != nullptr ? current->_left : current->_right;

        /*If node has one children*/
        if (r_node != nullptr) {
            r_node->_parent = current->_parent;
            _parent_node = current->_parent;
            if (_parent_node == nullptr) {
                _root = r_node;
            }
            else if (current == _parent_node->_left) {
                _parent_node->_left = r_node;
            }
            else {
                _parent_node->_right = r_node;
            }

            current->_right = nullptr;
            current->_left = nullptr;
            current->_parent = nullptr;

            if (set_color_of(current) == BLACK) {
                /*Balance only if its a black node*/
                _restore_balance(r_node, DELETION);
            }
        }
        else if (current->_parent == nullptr) {
            _size = 0;
            delete _root;
            delete _end_node;
            _root = nullptr;
            _end_node = nullptr;
            return _root;
        }
        else {
            /*Its a leaf*/
            if (set_color_of(current) == BLACK) {
                /*Balance only if its a black node*/
                _restore_balance(current, DELETION);
            }

            _parent_node = current->_parent;
            if (_parent_node != nullptr) {
                if (current == _parent_node->_left) {
                    _parent_node->_left = nullptr;
                }
                else if (current == _parent_node->_right) {
                    _parent_node->_right = nullptr;
                }
                current->_parent = nullptr;
            }
        }

        return current;
    }

    template<typename T, class Less>
    set_node set<T, Less>::_successor(SetNode *tnode) const {
        SetNode *current;

        if (tnode == nullptr) {
            return nullptr;
        }

        if (tnode->_right) {
            current = tnode->_right;
            while (current->_left != nullptr) {
                current = current->_left;
            }
        }
        else {
            current = tnode->_parent;
            while (current != nullptr && tnode == current->_right) {
                tnode = current;
                current = current->_parent;
            }
        }

        return current;
    }

    template<typename T, class Less>
    bool set<T, Less>::_is_equal_key(const key_type &lhs_key, const key_type &rhs_key) const {
        if (_less(lhs_key, rhs_key)) {
            return false;
        }
        else if (_less(rhs_key, lhs_key)) {
            return false;
        }
        return true;
    }

    template<typename T, class Less>
    set_node set<T, Less>::_find_bound(SetNode *root, const key_type &key) const {
        SetNode *retnode;

        if (root == nullptr) {
            return nullptr;
        }

        if (_is_equal_key(root->_val, key)) {
            return root;
        }

        if (_less(root->_val, key)) {
            return _find_bound(root->_right, key);
        }

        retnode = _find_bound(root->_left, key);
        return retnode == nullptr ? root : retnode;
    }

    template<typename T, class Less>
    void set<T, Less>::_restore_balance(SetNode* tnode, int8_t type) {

        if (type == DELETION) {
            while (tnode != _root && set_color_of(tnode) == BLACK) {
                if (tnode == _set_left_of(_set_parent_of(tnode))) {
                    _set_balance_deletion(_set_right_of, _set_left_of, _rotate_left, _rotate_right);
                }
                else {
                    _set_balance_deletion(_set_left_of, _set_right_of, _rotate_right, _rotate_left);
                }
            }

            if (tnode != nullptr && set_color_of(tnode) != BLACK) {
                set_assign_color(tnode, BLACK);
            }
        }
        else if (type == INSERTION) {
            tnode->color = RED;
            _root->_parent = nullptr;

            while (tnode != _end_node && tnode != _root) {
                if (tnode->_parent->color != RED) {
                    break;
                }

                if (set_parent_of(tnode) == _set_left_of(_set_grand_parent_of(tnode))) {
                    _set_balance_insertion(_set_right_of, _rotate_right, _rotate_left);
                }
                else {
                    _set_balance_insertion(_set_left_of, _rotate_left, _rotate_right);
                }
            }
        }
        else {
            throw std::invalid_argument("This should not have happened, pls debug me :)");
        }
    }
}

