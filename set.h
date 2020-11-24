#pragma once

#include <functional>
#include <utility>
#include <cstddef>
#include <iterator>
#include <cstdint>
#include <iostream>

#include "rbtree_internal.h"

#define set_t typename set<Key, Less>

using namespace rbtree_internal;

namespace adt {

    template<typename Key, class Less = std::less<Key>>
    class set {
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
        using node_type = value_type;
        using internal_ptr = rb_node<node_type>*;

        internal_ptr _root;
        internal_ptr _sentinel;
        size_type _size;
        key_compare _less;

        template<typename U>
        struct handle_return_overload {
            typedef U tag_type;
            internal_ptr ptr;

            handle_return_overload() : ptr(nullptr) {}
            explicit handle_return_overload(internal_ptr _ptr) : ptr(_ptr) {}
        };

        struct tag_ignore{};
        struct tag_delete{};

        typedef handle_return_overload<tag_ignore> to_ignore;
        typedef handle_return_overload<tag_delete> to_delete;

    public:
        class iterator {
            friend class set;
            using internal_ptr = set::internal_ptr;

        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = set::value_type;
            using reference = set::const_reference;
            using pointer = set::const_pointer;
            using difference_type = set::difference_type;

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

                if (_ptr->right) {
                    current = _ptr->right;
                    while (current->left != nullptr) {
                        current = current->left;
                    }
                }
                else {
                    current = _ptr->parent;
                    while (current != nullptr && _ptr == current->right) {
                        _ptr = current;
                        current = current->parent;
                    }
                }

                _ptr = current;
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

                if (_ptr->left) {
                    current = _ptr->left;
                    while (current->right != nullptr) {
                        current = current->right;
                    }
                }
                else {
                    current = _ptr->parent;
                    while (current->parent != nullptr && _ptr == current->left) {
                        _ptr = current;
                        current = current->parent;
                    }
                }

                _ptr = current;
                return *this;
            }
            iterator operator--(int) {
                auto temp(*this);
                ++(*this);
                return temp;
            }

            reference operator*() const { return _ptr->data; }
            pointer operator->() const { return &(_ptr->data); }

            void swap(iterator &lhs, iterator& rhs) {
                std::swap(lhs, rhs);
            }

            template<class Container>
            friend container::iterator rbtree_internal::_rbtree_find(Container *cnt, const container::key_type &key);

        private:
            internal_ptr _ptr;

            iterator(internal_ptr ptr = nullptr) : _ptr(ptr) {}
        };

        class reverse_iterator {
            friend class set;
            friend class iterator;

            using internal_ptr = set::internal_ptr;
        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = set::value_type;
            using reference = set::const_reference;
            using pointer = set::const_pointer;
            using difference_type = set::difference_type;

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
        set(const key_compare &keq = key_compare()) noexcept;
        explicit set(const set &other) noexcept;
        set(set&& other) noexcept;
        ~set();
        set &operator=(set rhs);

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
        std::pair<iterator, bool> insert(const value_type &val);
        std::pair<iterator, bool> insert(value_type &&val);
        template <class... Args>
        std::pair<iterator, bool> emplace(Args &&... args);
        iterator erase(const_iterator pos);
        size_type erase(const value_type &val);
        iterator erase(const_iterator first, const_iterator last);
        void clear() noexcept;
        void swap(set &other);

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

        friend void swap(set &lhs, set &rhs) {
            using std::swap;

            swap(lhs._root, rhs._root);
            swap(lhs._sentinel, rhs._sentinel);
            swap(lhs._less, rhs._less);
            swap(lhs._size, rhs._size);
        }

        template<class Container>
        friend void rbtree_internal::_rbtree_destruct(rb_node<container::node_type> *tnode);

        template<class Container>
        friend rb_node<container::node_type> *rbtree_internal::_rbtree_left_of(rb_node<container::node_type> *tnode);

        template<class Container>
        friend rb_node<container::node_type> *rbtree_internal::_rbtree_right_of(rb_node<container::node_type> *tnode);

        template<class Container>
        friend rb_node<container::node_type> *rbtree_internal::_rbtree_parent_of(rb_node<container::node_type> *tnode);

        template<class Container>
        friend rb_node<container::node_type> *rbtree_internal::_rbtree_grandparent_of(rb_node<container::node_type> *tnode);

        template<class Container>
        friend void rbtree_internal::_rbtree_set_color(rb_node<container::node_type> *tnode, color_t color);

        template<class Container>
        friend color_t rbtree_internal::_rbtree_get_color(rb_node<container::node_type> *tnode);

        template<class Container>
        friend void rbtree_internal::_rbtree_rotate_right(rb_node<container::node_type> **root, rb_node<container::node_type> *tnode);

        template<class Container>
        friend void rbtree_internal::_rbtree_rotate_left(rb_node<container::node_type> **root, rb_node<container::node_type> *tnode);

        template<class Container>
        friend void rbtree_internal::_rbtree_restore_balance(rb_node<container::node_type> **root, rb_node<container::node_type> *sentinel, rb_node<container::node_type> *tnode, balance_t btype);

        template<class Container>
        friend rb_node<container::node_type> *rbtree_internal::_rbtree_successor(rb_node<container::node_type> *tnode);

        template<typename Container, typename K, typename V>
        friend rb_node<container::node_type> *rbtree_internal::_rbtree_bst_insert(Container *cnt, bool &added_new, const K &key, V val);

        template<class Container, typename R, typename K, typename V, typename... Args>
        friend R rbtree_internal::_rbtree_insert(Container *cnt, const K &key, V val, Args &&... args);

        template<class Container>
        friend rb_node<container::node_type> *rbtree_internal::_rbtree_erase(Container *cnt, rb_node<container::node_type> *current, rb_node<container::node_type> *successor);

        template<class Container>
        friend container::iterator rbtree_internal::_rbtree_find(Container *cnt, const container::key_type &key);

        template<class Container>
        friend rb_node<container::node_type> *rbtree_internal::_rbtree_find_bound(Container *cnt, rb_node<container::node_type> *tnode, const container::key_type &key);

    private:
        internal_ptr _copy_tree(internal_ptr other_root);
        internal_ptr _construct_new_element(const value_type &val);
        internal_ptr _construct_new_element(value_type &&val);
        internal_ptr _construct_new_element(internal_ptr val);
        std::pair<iterator, bool> _handle_elem_found(internal_ptr ptr, to_ignore obj);
        std::pair<iterator, bool> _handle_elem_found(internal_ptr ptr, to_delete obj);
        std::pair<iterator, bool> _handle_elem_not_found(internal_ptr ptr);
        key_type &_get_key(rb_node<node_type> *tnode);
        bool _is_equal_key(const key_type &lhs_key, const key_type &rhs_key) const;
        std::pair<rb_node<node_type> *, size_type> _erase(const_iterator pos);
    };

    /* Implementation.  */

    /* Public member functions.  */
    template<typename Key, class Less>
    set<Key, Less>::set(const key_compare &keq) noexcept : _root(nullptr), _size(0), _less(keq) {
        _sentinel = new rb_node<node_type>();
    }

    template<typename Key, class Less>
    set<Key, Less>::set(const set &other) noexcept {
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
    set<Key, Less>::set(set &&other) noexcept : set() {
        swap(*this, other);
    }

    template<typename Key, class Less>
    set<Key, Less>::~set() {
        if (_root != nullptr) {
            clear();
            delete _sentinel;
        } else {
            if (_sentinel) delete _sentinel;
        }
    }

    template<typename Key, class Less>
    set<Key, Less>& set<Key, Less>::operator=(set rhs) {
        /* Copy and swap idiom, let the compiler handle the copy of the argument.  */
        swap(*this, rhs);

        return *this;
    }

    template<typename Key, class Less>
    set_t::iterator set<Key, Less>::begin() noexcept {
        internal_ptr current = _root;

        if (current) {
            while (current->left != nullptr) {
                current = current->left;
            }
        } else {
            current = _sentinel;
        }

        return iterator(current);
    }

    template<typename Key, class Less>
    set_t::const_iterator set<Key, Less>::begin() const noexcept {
        return const_cast<set<Key, Less>*>(this)->begin();
    }

    template<typename Key, class Less>
    set_t::iterator set<Key, Less>::end() noexcept {
        return iterator(_sentinel);
    }

    template<typename Key, class Less>
    set_t::const_iterator set<Key, Less>::end() const noexcept {
        return const_cast<set<Key, Less>*>(this)->end();
    }

    template<typename Key, class Less>
    set_t::reverse_iterator set<Key, Less>::rbegin() noexcept {
        internal_ptr current = _root;

        if (current) {
            while (current->right != nullptr) {
                current = current->right;
            }
        } else {
            current = _sentinel;
        }

        return reverse_iterator(current);
    }

    template<typename Key, class Less>
    set_t::const_reverse_iterator set<Key, Less>::rbegin() const {
        return const_cast<set<Key, Less>*>(this)->rbegin();
    }

    template<typename Key, class Less>
    set_t::reverse_iterator set<Key, Less>::rend() noexcept {
        return reverse_iterator(_sentinel);
    }

    template<typename Key, class Less>
    set_t::const_reverse_iterator set<Key, Less>::rend() const noexcept {
        return const_cast<set<Key, Less>*>(this)->rend();
    }

    template<typename Key, class Less>
    set_t::const_iterator set<Key, Less>::cbegin() const noexcept {
        return const_cast<set<Key, Less>*>(this)->begin();
    }

    template<typename Key, class Less>
    set_t::const_iterator set<Key, Less>::cend() const noexcept {
        return const_cast<set<Key, Less>*>(this)->end();
    }

    template<typename Key, class Less>
    set_t::const_reverse_iterator set<Key, Less>::crbegin() const noexcept {
        return const_cast<set<Key, Less>*>(this)->rbegin();
    }

    template<typename Key, class Less>
    set_t::const_reverse_iterator set<Key, Less>::crend() const noexcept {
        return const_cast<set<Key, Less>*>(this)->rend();
    }

    template<typename Key, class Less>
    bool set<Key, Less>::empty() const noexcept {
        return _size == 0;
    }

    template<typename Key, class Less>
    set_t::size_type set<Key, Less>::size() const noexcept {
        return _size;
    }

    template<typename Key, class Less>
    set_t::key_compare set<Key, Less>::key_comp() const {
        return _less;
    }

    template<typename Key, class Less>
    set_t::value_compare set<Key, Less>::value_comp() const {
        return _less;
    }

    template<typename Key, class Less>
    std::pair<set_t::iterator, bool> set<Key, Less>::insert(const value_type &val) {
        return _rbtree_insert<set<Key, Less>, std::pair<iterator, bool>, key_type, const value_type&>(this, val, val, to_ignore());
    }

    template<typename Key, class Less>
    std::pair<set_t::iterator, bool> set<Key, Less>::insert(value_type &&val) {
        return _rbtree_insert<set<Key, Less>, std::pair<iterator, bool>, key_type, value_type&&>(this, val, std::forward<value_type>(val), to_ignore());
    }

    template<typename Key, class Less>
    template<class... Args>
    std::pair<set_t::iterator, bool> set<Key, Less>::emplace(Args &&... args) {
        internal_ptr val = new rb_node<node_type>(std::forward<Args>(args)...);

        return _rbtree_insert<set<Key, Less>, std::pair<iterator, bool>, key_type, internal_ptr>(this, val->data, val, to_delete(val));
    }

    template<typename Key, class Less>
    set_t::iterator set<Key, Less>::erase(const_iterator pos) {
        return _erase(pos).first;
    }

    template<typename Key, class Less>
    set_t::size_type set<Key, Less>::erase(const value_type &val) {
        return _erase(find(val)).second;
    }

    template<typename Key, class Less>
    set_t::iterator set<Key, Less>::erase(const_iterator first, const_iterator last) {
        auto it = first;

        while (it != last) it = erase(it);

        return it;
    }

    template<typename Key, class Less>
    void set<Key, Less>::clear() noexcept {
        _rbtree_destruct<set<Key, Less>>(_root);
        _root = nullptr;
        _size = 0;
    }

    template<typename Key, class Less>
    void set<Key, Less>::swap(set &other) {
        swap(*this, other);
    }

    template<typename Key, class Less>
    set_t::iterator set<Key, Less>::find(const key_type &key) {
        return _rbtree_find<set<Key, Less>>(this, key);
    }

    template<typename Key, class Less>
    set_t::const_iterator set<Key, Less>::find(const key_type &key) const {
        return const_cast<set<Key, Less>*>(this)->find(key);
    }

    template<typename Key, class Less>
    set_t::size_type set<Key, Less>::count(const key_type &key) const {
        return find(key)._ptr != _sentinel ? 1 : 0;
    }

    template<typename Key, class Less>
    set_t::iterator set<Key, Less>::lower_bound(const key_type &key) {
        internal_ptr bound = _rbtree_find_bound<set<Key, Less>>(this, _root, key);

        return bound == nullptr ? end() : iterator(bound);
    }

    template<typename Key, class Less>
    set_t::const_iterator set<Key, Less>::lower_bound(const key_type &key) const {
        return const_cast<set<Key, Less>*>(this)->lower_bound(key);
    }

    template<typename Key, class Less>
    set_t::iterator set<Key, Less>::upper_bound(const key_type &key) {
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
    set_t::const_iterator set<Key, Less>::upper_bound(const key_type &key) const {
        return const_cast<set<Key, Less>*>(this)->upper_bound(key);
    }

    template<typename Key, class Less>
    std::pair<set_t::iterator, set_t::iterator> set<Key, Less>::equal_range(const key_type &key) {
        auto first = find(key);
        auto second(first);

        return {first, ++second};
    }

    template<typename Key, class Less>
    std::pair<set_t::const_iterator, set_t::const_iterator> set<Key, Less>::equal_range(const key_type &key) const {
        return const_cast<set<Key, Less>*>(this)->equal_range(key);
    }

    /* Private member functions.  */
    template<typename Key, class Less>
    set_t::internal_ptr set<Key, Less>::_copy_tree(internal_ptr other_root) {
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
    set_t::internal_ptr set<Key, Less>::_construct_new_element(const value_type &val) {
        return new rb_node<node_type>(val);
    }

    template<typename Key, class Less>
    set_t::internal_ptr set<Key, Less>::_construct_new_element(value_type &&val) {
        return new rb_node<node_type>(std::forward<node_type>(val));
    }

    template<typename Key, class Less>
    set_t::internal_ptr set<Key, Less>::_construct_new_element(internal_ptr val) {
        return val;
    }

    template<typename Key, class Less>
    std::pair<set_t::iterator, bool> set<Key, Less>::_handle_elem_found(internal_ptr ptr, to_ignore obj) {
        return {ptr, false};
    }

    template<typename Key, class Less>
    std::pair<set_t::iterator, bool> set<Key, Less>::_handle_elem_found(internal_ptr ptr, to_delete obj) {
        delete obj.ptr;
        return {ptr, false};
    }

    template<typename Key, class Less>
    std::pair<set_t::iterator, bool> set<Key, Less>::_handle_elem_not_found(internal_ptr ptr) {
        return {ptr, true};
    }

    template<typename Key, class Less>
    set_t::key_type &set<Key, Less>::_get_key(rb_node<node_type> *tnode) {
        return tnode->data;
    }

    template<typename Key, class Less>
    bool set<Key, Less>::_is_equal_key(const key_type &lhs_key, const key_type &rhs_key) const {
        return _less(lhs_key, rhs_key) || _less(rhs_key, lhs_key);
    }

    template<typename Key, class Less>
    std::pair<rb_node<set_t::node_type> *, set_t::size_type> set<Key, Less>::_erase(const_iterator pos) {
        internal_ptr save, successor, next_elem_ptr, erase_ptr;
        size_type count = 0;

        if (pos != end()) {
            save = _sentinel;
            _root->parent = nullptr;

            successor = _rbtree_successor<set<Key, Less>>(pos._ptr);
            erase_ptr = _rbtree_erase<set<Key, Less>>(this, pos._ptr, successor);

            if (erase_ptr != _root) {
                next_elem_ptr = erase_ptr == successor ? pos._ptr : successor;

                delete erase_ptr;

                /* Update the sentinel node.  */
                _sentinel = save;
                _root->parent = _sentinel;
                _sentinel->left = _root;
                --_size;
                count++;

                return {successor != nullptr ? next_elem_ptr : _sentinel, count};
            }
            else {
                delete _root;
                _sentinel->left = nullptr;
                _root = nullptr;
                _size = 0;
            }
        }

        return {_sentinel, count};
    }
}

