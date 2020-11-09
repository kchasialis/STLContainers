#pragma once

#include <functional>
#include <utility>
#include <cstddef>
#include <iterator>
#include <cstdint>
#include <iostream>

#include "rbtree_internal.h"

#define set_t set_t

using namespace rbtree_internal;

namespace adt {

    template<typename T, class Less = std::less<T>>
    class set {
    private:
        using key_type = T;
        using value_type = T;
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
        using node_type = value_type;
        using internal_ptr = rb_node<node_type>*;

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
            iterator operator--(int) {
                auto temp(*this);
                ++(*this);
                return temp;
            }

            reference operator*() const { return _ptr->_val; }
            pointer operator->() const { return &(_ptr->_val); }

            void swap(iterator &lhs, iterator& rhs) {
                std::swap(lhs, rhs);
            }

        private:
            internal_ptr _ptr;

            iterator(internal_ptr *ptr = nullptr) : _ptr(ptr) {}
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

            /* Inorder successor algorithm. */
            reverse_iterator &operator++() {
                --_it;
                return *this;
            }
            reverse_iterator operator++(int) { return _it--; }
            /* Inorder predecessor algorithm. */
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

        internal_ptr _root;
        internal_ptr _sentinel;
        Less _less;
        size_t _size;

    public:
        /* Constructors/Destructors.  */
        set() noexcept;
        explicit set(const set &rhs) noexcept;
        set(set&& rhs) noexcept;
        ~set();
        set& operator=(set rhs);

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
            swap(lhs._end_node, rhs._end_node);
            swap(lhs._less, rhs._less);
            swap(lhs._size, rhs._size);
        }

    private:
        internal_ptr _bst_insert(bool &added_new, const value_type &val);
        internal_ptr _bst_insert(bool &added_new, value_type &&val);
        internal_ptr _erase(internal_ptr current, internal_ptr successor);
        bool _is_equal_key(const key_type &lhs_key, const key_type &rhs_key) const;
    };

    /* Implementation.  */

    /* Public member functions.  */
    template<typename T, class Less>
    set<T, Less>::set() noexcept : _root(nullptr), _size(0) {
        _sentinel = new rb_node<node_type>();
    }

    template<typename T, class Less>
    set<T, Less>::set(const set &other) noexcept {
        /* Create an exact copy of this set, O(n).  */
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
    }

    /* Private member functions.  */
}

