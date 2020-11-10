#pragma once

#include <memory>

#define multiset_t typename multiset<Key, Less>

namespace adt {
    
    template<typename Key, class Less = std::less<Key>>
    class multiset {
    public:
        using key_type = Key;
        using value_type = Key;
        using key_compare = Less;
        using value_compare = Less;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        class iterator;
        using const_iterator = iterator;
        class reverse_iterator;
        using const_reverse_iterator = reverse_iterator;

    private:
        struct multiset_node {
            value_type data;
            multiset_node *next;
            multiset_node *previous;

            multiset_node() : data(), next(nullptr), previous(nullptr) {}
            multiset_node(const value_type &_data) : data(_data), next(nullptr), previous(nullptr) {}
            multiset_node(value_type &&_data) : data(_data), next(nullptr), previous(nullptr) {}
            template<typename... Args>
            multiset_node(Args&&... args) : data(std::forward<Args>(args)...), next(nullptr), previous(nullptr) {}
            multiset_node(const multiset_node &other) = default;
            multiset_node(multiset_node &&other) = default;
        };

        using node_type = multiset_node*;
        using internal_ptr = rb_node<node_type>*;

        internal_ptr _root;
        internal_ptr _sentinel;
        size_type _size;
        key_compare _less;

    public:
        class iterator {
            friend class multiset;
            using internal_ptr = multiset::internal_ptr;

        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = multiset::value_type;
            using reference = multiset::const_reference;
            using pointer = multiset::const_pointer;
            using difference_type = multiset::difference_type;

            iterator(const iterator &other) = default;
            iterator(iterator &&other) = default;

            iterator &operator=(const iterator &rhs) = default;
            iterator &operator=(internal_ptr ptr) {
                this->_ptr = ptr;
                return *this;
            }

            bool operator==(const iterator &rhs) const { return this->_ptr == rhs._ptr; }
            bool operator==(internal_ptr ptr) const { return this->_ptr == ptr; }
            bool operator!=(const iterator &rhs) const { return !(*this == rhs); }
            bool operator!=(internal_ptr ptr) const { return !(*this == ptr); }

            /* Inorder successor algorithm. */
            iterator &operator++() {
                internal_ptr current;

                if (_ptr == nullptr) {
                    return *this;
                }

                /* First, check if the pointer to internal list has no other elements.  */
                if (_inner_ptr->next != nullptr) {
                    _inner_ptr = _inner_ptr->next;
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
                _inner_ptr = _ptr ? _ptr->data : nullptr;
                return *this;
            }
            iterator operator++(int) {
                auto temp(*this);
                ++(*this);
                return temp;
            }
            /* Inorder predecessor algorithm. */
            iterator &operator--() {
                internal_ptr current;

                if (_ptr == nullptr) {
                    return *this;
                }

                if (_inner_ptr->previous != nullptr) {
                    _inner_ptr = _inner_ptr->previous;
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
                _inner_ptr = _ptr ? _ptr->data : nullptr;
                return *this;
            }
            iterator operator--(int) {
                auto temp(*this);
                ++(*this);
                return temp;
            }

            reference operator*() const { return _inner_ptr->data; }
            pointer operator->() const { return &(_inner_ptr->data); }

            void swap(iterator &lhs, iterator& rhs) {
                std::swap(lhs, rhs);
            }

        private:
            internal_ptr _ptr;
            node_type _inner_ptr;

            iterator(internal_ptr *ptr = nullptr) : _ptr(ptr) {
                _inner_ptr = _ptr ? _ptr->data : nullptr;
            }
        };

        class reverse_iterator {
            friend class multiset;
            friend class iterator;
            using internal_ptr = multiset::internal_ptr;

        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = multiset::value_type;
            using reference = multiset::const_reference;
            using pointer = multiset::const_pointer;
            using difference_type = multiset::difference_type;

            reverse_iterator(const reverse_iterator &other) = default;
            reverse_iterator(reverse_iterator &&other) = default;

            reverse_iterator &operator=(const reverse_iterator &rhs) = default;
            reverse_iterator &operator=(internal_ptr ptr) {
                this->_it = ptr;
                return *this;
            }

            bool operator==(const reverse_iterator &rhs) const { return this->_it == rhs._it; }
            bool operator==(internal_ptr ptr) const { return this->_it == ptr; }
            bool operator!=(const reverse_iterator &rhs) const { return !(*this == rhs); }
            bool operator!=(internal_ptr ptr) const { return !(*this == ptr); }

            reverse_iterator &operator++() {
                --_it;
                return *this;
            }
            reverse_iterator operator++(int) { return _it--; }
            reverse_iterator &operator--() {
                ++_it;
                return *this;
            }
            reverse_iterator operator--(int) { return _it++; }

            reference operator*() const { return *_it; }
            pointer operator->() const { return _it.operator->(); }

            void swap(reverse_iterator &lhs, reverse_iterator& rhs) {
                std::swap(lhs, rhs);
            }

        private:
            iterator _it;

            reverse_iterator(internal_ptr ptr) : _it(ptr) {}
        };

        /* Constructors/Destructors.  */
        multiset(const key_compare &keq = key_compare()) noexcept;
        explicit multiset(const multiset &other) noexcept;
        multiset(multiset &&other) noexcept;
        ~multiset();
        multiset &operator=(multiset rhs);

        /* Iterators.  */
        iterator begin() noexcept;
        const_iterator begin() const noexcept;
        iterator end() noexcept;
        const_iterator end() const noexcept;
        reverse_iterator rbegin() noexcept;
        const_reverse_iterator rbegin() const;
        reverse_iterator rend() noexcept;
        const_reverse_iterator rend() const noexcept;
        const_iterator cbegin() const noexcept;
        const_iterator cend() const noexcept;
        const_reverse_iterator crbegin() const noexcept;
        const_reverse_iterator crend() const noexcept;

        /* Capacity.  */
        bool empty() const noexcept;
        size_type size() const noexcept;

        /* Observers.  */
        key_compare key_comp() const;
        value_compare value_comp() const;

        /* Modifiers.  */
        iterator insert(const value_type &val);
        iterator insert(value_type &&val);
        template <class... Args>
        iterator emplace(Args &&... args);
        iterator erase(const_iterator pos);
        size_type erase(const value_type &val);
        iterator erase(const_iterator first, const_iterator last);
        void clear() noexcept;
        void swap(multiset &other);

        /* Operations.  */
        iterator find(const key_type &key);
        const_iterator find(const key_type &key) const;
        size_type count(const key_type &key) const;
        iterator lower_bound(const key_type &key);
        const_iterator lower_bound(const key_type &key) const;
        iterator upper_bound(const key_type &key);
        const_iterator upper_bound(const key_type &key) const;
        std::pair<iterator, iterator> equal_range(const key_type &key);
        std::pair<const_iterator, const_iterator> equal_range(const key_type &key) const;

        friend void swap(multiset &lhs, multiset &rhs) {
            using std::swap;

            swap(lhs._root, rhs._root);
            swap(lhs._sentinel, rhs._sentinel);
            swap(lhs._less, rhs._less);
            swap(lhs._size, rhs._size);
        }

    private:
        internal_ptr _copy_tree(internal_ptr other_root);
        iterator _add_to_list(internal_ptr ptr, multiset_node *new_node);
        internal_ptr _construct_new_element(const value_type &val);
        internal_ptr _construct_new_element(value_type &&val);
        internal_ptr _construct_new_element(multiset_node *val);
        iterator _handle_elem_found(internal_ptr ptr, const value_type &val);
        iterator _handle_elem_found(internal_ptr ptr, value_type &&val);
        iterator _handle_elem_found(internal_ptr ptr, multiset_node *val);
        iterator _handle_elem_not_found(internal_ptr ptr);
        key_type &_get_key(rb_node<node_type> *tnode);
        bool _is_equal_key(const key_type &lhs_key, const key_type &rhs_key) const;
        size_type _erase_list(internal_ptr erase_ptr);
        size_type _erase_node(internal_ptr erase_ptr);
        std::pair<rb_node<node_type> *, size_type> _erase(const_iterator pos, bool erase_all);
    };

    /* Implementation.  */

    /* Public member functions.  */
    template<typename Key, class Less>
    multiset<Key, Less>::multiset(const key_compare &keq) noexcept : _root(nullptr), _size(0), _less(keq) {
        _sentinel = new rb_node<node_type>();
    }

    template<typename Key, class Less>
    multiset<Key, Less>::multiset(const multiset &other) noexcept {
        if (this != &other) {
            /* Create an exact copy of this set, O(n).  */
            _root = _copy_tree(other._root);
            _sentinel = new rb_node<node_type>();
            _sentinel->left = _root;
            _root->parent = _sentinel;
            _less = other._less;
        }
    }

    template<typename Key, class Less>
    multiset<Key, Less>::multiset(multiset &&other) noexcept : multiset() {
        swap(*this, other);
    }

    template<typename Key, class Less>
    multiset<Key, Less>::~multiset() {
        if (_root != nullptr) {
            clear();
            delete _sentinel;
        } else {
            if (_sentinel) delete _sentinel;
        }
    }

    template<typename Key, class Less>
    multiset<Key, Less> &multiset<Key, Less>::operator=(multiset rhs) {
        /* Copy and swap idiom, let the compiler handle the copy of the argument.  */
        swap(*this, rhs);

        return *this;
    }

    template<typename Key, class Less>
    multiset_t::iterator multiset<Key, Less>::begin() noexcept {
        internal_ptr current = _root;

        if (current) {
            while (current->left != nullptr) {
                current = current->left;
            }
        }

        return iterator(current);
    }

    template<typename Key, class Less>
    multiset_t::const_iterator multiset<Key, Less>::begin() const noexcept {
        return const_cast<multiset<Key, Less>*>(this)->begin();
    }

    template<typename Key, class Less>
    multiset_t::iterator multiset<Key, Less>::end() noexcept {
        return iterator(_sentinel);
    }

    template<typename Key, class Less>
    multiset_t::const_iterator multiset<Key, Less>::end() const noexcept {
        return const_cast<multiset<Key, Less>*>(this)->end();
    }

    template<typename Key, class Less>
    multiset_t::reverse_iterator multiset<Key, Less>::rbegin() noexcept {
        rb_node<node_type> *current = _root;

        if (current) {
            while (current->right != nullptr) {
                current = current->right;
            }
        }

        return reverse_iterator(current);
    }

    template<typename Key, class Less>
    multiset_t::const_reverse_iterator multiset<Key, Less>::rbegin() const {
        return const_cast<multiset<Key, Less>*>(this)->rbegin();
    }

    template<typename Key, class Less>
    multiset_t::reverse_iterator multiset<Key, Less>::rend() noexcept {
        return reverse_iterator(_sentinel);
    }

    template<typename Key, class Less>
    multiset_t::const_reverse_iterator multiset<Key, Less>::rend() const noexcept {
        return const_cast<multiset<Key, Less>*>(this)->rend();
    }

    template<typename Key, class Less>
    multiset_t::const_iterator multiset<Key, Less>::cbegin() const noexcept {
        return const_cast<multiset<Key, Less>*>(this)->begin();
    }

    template<typename Key, class Less>
    multiset_t::const_iterator multiset<Key, Less>::cend() const noexcept {
        return const_cast<multiset<Key, Less>*>(this)->end();
    }

    template<typename Key, class Less>
    multiset_t::const_reverse_iterator multiset<Key, Less>::crbegin() const noexcept {
        return const_cast<multiset<Key, Less>*>(this)->rbegin();
    }

    template<typename Key, class Less>
    multiset_t::const_reverse_iterator multiset<Key, Less>::crend() const noexcept {
        return const_cast<multiset<Key, Less>*>(this)->rend();
    }

    template<typename Key, class Less>
    bool multiset<Key, Less>::empty() const noexcept {
        return _size == 0;
    }

    template<typename Key, class Less>
    multiset_t::size_type multiset<Key, Less>::size() const noexcept {
        return _size;
    }

    template<typename Key, class Less>
    multiset_t::key_compare multiset<Key, Less>::key_comp() const {
        return _less;
    }

    template<typename Key, class Less>
    multiset_t::value_compare multiset<Key, Less>::value_comp() const {
        return _less;
    }

    template<typename Key, class Less>
    multiset_t::iterator multiset<Key, Less>::insert(const value_type &val) {
        return _rbtree_insert<multiset<Key, Less>, iterator, key_type, const value_type&>(this, val, val);
    }

    template<typename Key, class Less>
    multiset_t::iterator multiset<Key, Less>::insert(value_type &&val) {
        return _rbtree_insert<set<Key, Less>, iterator, key_type, value_type&&>(this, val, std::forward<value_type>(val));
    }

    template<typename Key, class Less>
    template<class... Args>
    multiset_t::iterator multiset<Key, Less>::emplace(Args &&... args) {
        internal_ptr val = new rb_node<node_type>(std::forward<Args>(args)...);
        return _rbtree_insert<set<Key, Less>, std::pair<iterator, bool>, key_type, internal_ptr>(this, val->data, val);
    }

    template<typename Key, class Less>
    multiset_t::iterator multiset<Key, Less>::erase(const_iterator pos) {
        return _erase(pos, false).first;
    }

    template<typename Key, class Less>
    multiset_t::size_type multiset<Key, Less>::erase(const value_type &val) {
        return _erase(find(val), true).second;
    }

    template<typename Key, class Less>
    multiset_t::iterator multiset<Key, Less>::erase(multiset::const_iterator first, multiset::const_iterator last) {
        auto it = first;

        while (it != last) it = erase(it);

        return it;
    }

    template<typename Key, class Less>
    void multiset<Key, Less>::clear() noexcept {
        _rbtree_destruct<multiset<Key, Less>>(_root);
    }

    template<typename Key, class Less>
    void multiset<Key, Less>::swap(multiset &other) {
        swap(*this, other);
    }

    template<typename Key, class Less>
    multiset_t::iterator multiset<Key, Less>::find(const key_type &key) {
        return _rbtree_find<multiset<Key, Less>>(this, key);
    }

    template<typename Key, class Less>
    multiset_t::const_iterator multiset<Key, Less>::find(const key_type &key) const {
        return const_cast<multiset<Key, Less>*>(this)->find(key);
    }

    template<typename Key, class Less>
    multiset_t::size_type multiset<Key, Less>::count(const key_type &key) const {
        multiset_node *current;
        multiset_node *found_ptr = const_cast<multiset<Key, Less>*>(this)->find(key)._ptr;
        size_t count = 0;

        if (found_ptr == _sentinel) return count;

        current = found_ptr->data;
        while (current != nullptr) {
            count++;
            current = current->next;
        }

        return count;
    }

    template<typename Key, class Less>
    multiset_t::iterator multiset<Key, Less>::lower_bound(const key_type &key) {
        internal_ptr bound = _rbtree_find_bound<multiset<Key, Less>>(this, _root, key);

        return bound == nullptr ? end() : iterator(bound);
    }

    template<typename Key, class Less>
    multiset_t::const_iterator multiset<Key, Less>::lower_bound(const key_type &key) const {
        return const_cast<multiset<Key, Less>*>(this)->lower_bound(key);
    }

    template<typename Key, class Less>
    multiset_t::iterator multiset<Key, Less>::upper_bound(const key_type &key) {
        internal_ptr bound = _rbtree_find_bound<set<Key, Less>>(this, _root, key);

        if (bound == nullptr) {
            return end();
        } else {
            if (_is_equal_key(bound->data, key)) {
                return iterator(_rbtree_successor<set<Key, Less>>(bound));
            } else {
                return iterator(bound);
            }
        }
    }

    template<typename Key, class Less>
    multiset_t::const_iterator multiset<Key, Less>::upper_bound(const key_type &key) const {
        return const_cast<multiset<Key, Less>*>(this)->upper_bound(key);
    }

    template<typename Key, class Less>
    std::pair<multiset_t::iterator, multiset_t::iterator> multiset<Key, Less>::equal_range(const key_type &key) {
        auto first = find(key);
        auto second(first);

        return {first, ++second};
    }

    template<typename Key, class Less>
    std::pair<multiset_t::const_iterator, multiset_t::const_iterator> multiset<Key, Less>::equal_range(const key_type &key) const {
        return const_cast<multiset<Key, Less>*>(this)->equal_range(key);
    }

    /* Private member functions.  */
    template<typename Key, class Less>
    multiset_t::internal_ptr multiset<Key, Less>::_copy_tree(internal_ptr other_root) {
        internal_ptr new_node;

        if (other_root == nullptr) return nullptr;

        new_node = new rb_node<node_type>(other_root->data);

        new_node->left = _copy_tree(other_root->left);
        if (new_node->left) new_node->left->parent = new_node;

        new_node->right = _copy_tree(other_root->right);
        if (new_node->right) new_node->right->parent = new_node;

        return new_node;
    }

    template<typename Key, class Less>
    multiset_t::iterator multiset<Key, Less>::_add_to_list(internal_ptr ptr, multiset_node *new_node) {
        new_node->next = ptr->data;
        ptr->data = new_node;
        new_node->next->previous = new_node;
        _size++;

        return ptr;
    }

    template<typename Key, class Less>
    multiset_t::internal_ptr multiset<Key, Less>::_construct_new_element(const value_type &val) {
        return new rb_node<node_type>(new multiset_node(val));
    }

    template<typename Key, class Less>
    multiset_t::internal_ptr multiset<Key, Less>::_construct_new_element(value_type &&val) {
        return new rb_node<node_type>(new multiset_node(std::forward<value_type>(val)));
    }

    template<typename Key, class Less>
    multiset_t::internal_ptr multiset<Key, Less>::_construct_new_element(multiset_node *val) {
        return new rb_node<node_type>(val);
    }

    template<typename Key, class Less>
    multiset_t::iterator multiset<Key, Less>::_handle_elem_found(internal_ptr ptr, const value_type &val) {
        return _add_to_list(ptr, new multiset_node(val));
    }

    template<typename Key, class Less>
    multiset_t::iterator multiset<Key, Less>::_handle_elem_found(internal_ptr ptr, value_type &&val) {
        return _add_to_list(ptr, new multiset_node(std::forward<value_type>(val)));
    }

    template<typename Key, class Less>
    multiset_t::iterator multiset<Key, Less>::_handle_elem_found(internal_ptr ptr, multiset_node *val) {
        return _add_to_list(ptr, val);
    }

    template<typename Key, class Less>
    multiset_t::iterator multiset<Key, Less>::_handle_elem_not_found(internal_ptr ptr) {
        return ptr;
    }

    template<typename Key, class Less>
    multiset_t::key_type &multiset<Key, Less>::_get_key(internal_ptr tnode) {
        return tnode->data->data;
    }

    template<typename Key, class Less>
    bool multiset<Key, Less>::_is_equal_key(const key_type &lhs_key, const key_type &rhs_key) const {
        return _less(lhs_key, rhs_key) || _less(rhs_key, lhs_key);
    }

    template<typename Key, class Less>
    multiset_t::size_type multiset<Key, Less>::_erase_list(internal_ptr erase_ptr) {
        multiset_node *current, *to_delete;
        size_t count = 0;

        current = erase_ptr->data;
        while (current->next != nullptr) {
            to_delete = current->next;
            current->next = to_delete->next;

            delete to_delete;
            count++;
        }

        delete erase_ptr->data;
        delete erase_ptr;
        count++;

        return count;
    }

    template<typename Key, class Less>
    multiset_t::size_type multiset<Key, Less>::_erase_node(internal_ptr erase_ptr) {
        multiset_node *head = erase_ptr->data;

        erase_ptr->data = head->next;
        delete head;

        return 1;
    }

    template<typename Key, class Less>
    std::pair<multiset_t::internal_ptr, multiset_t::size_type> multiset<Key, Less>::_erase(const_iterator pos, bool erase_all) {
        internal_ptr save, successor, next_elem_ptr, erase_ptr;
        multiset_node *current, *to_delete;
        size_t count = 0;

        if (pos != end()) {
            save = _sentinel;
            _root->parent = nullptr;

            successor = _rbtree_successor<multiset<Key, Less>>(pos._ptr);
            erase_ptr = _rbtree_erase<set<Key, Less>>(this, pos._ptr, successor);

            if (erase_ptr != _root) {
                next_elem_ptr = erase_ptr == successor ? pos._ptr : successor;

                count = erase_all ? _erase_list(erase_ptr) : _erase_node(erase_ptr);

                /* Update the sentinel node.  */
                _sentinel = save;
                _root->parent = _sentinel;
                _sentinel->left = _root;
                _size -= count;

                return {successor != nullptr ? next_elem_ptr : end(), count};
            } else {

            }
        }

        return {end(), count};
    }
}