#pragma once

#include <functional>
#include <utility>
#include <cstddef>
#include <iterator>
#include <cstdint>
#include <iostream>

#include "rbtree_internal.h"

#define map_t typename map<K, V, Less>

using namespace rbtree_internal;

namespace adt {

    template<typename K, typename V, class Less = std::less<K>>
    class map {
    public:
        using key_type = K;
        using mapped_type = V;
        using value_type = std::pair<const K, V>;
        using key_compare = Less;
        using value_compare = Less;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        class iterator;
        class const_iterator;
        class reverse_iterator;
        class const_reverse_iterator;

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
            explicit handle_return_overload(value_type *_ptr) : ptr(_ptr) {}
        };

        struct tag_ignore{};
        struct tag_delete{};

        typedef handle_return_overload<tag_ignore> to_ignore;
        typedef handle_return_overload<tag_delete> to_delete;

        struct enabler {};

    public:
        class iterator {
            friend class map;
            friend class const_iterator;
            friend class reverse_iterator;
            friend class const_reverse_iterator;
            using internal_ptr = map::internal_ptr;

        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = map::value_type;
            using reference = map::reference;
            using pointer = map::pointer;
            using difference_type = map::difference_type;

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

            reference operator*() { return _ptr->_val; }
            const_reference operator*() const { return _ptr->val; }
            pointer operator->() { return &(_ptr->_val); }
            const_pointer operator->() const { return &(_ptr->val); }

            void swap(iterator &lhs, iterator& rhs) { std::swap(lhs, rhs); }

        private:
            internal_ptr _ptr;

            iterator(internal_ptr *ptr = nullptr) : _ptr(ptr) {}

        };

        class const_iterator {
            friend class map;
            friend class iterator;
            using internal_ptr = map::internal_ptr;

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = map::value_type;
            using reference = map::const_reference;
            using pointer = map::const_pointer;
            using difference_type = map::difference_type;

            /* Implicit conversion from iterator.  */
            const_iterator(iterator it) : _it(std::move(it)) {}

            const_iterator &operator=(const const_iterator &rhs) = default;
            const_iterator &operator=(internal_ptr *ptr) {
                this->_it = ptr;
                return *this;
            }

            bool operator==(const const_iterator &other) const { return this->_it == other._it; }
            bool operator==(internal_ptr *ptr) const { return _it == ptr; }
            bool operator!=(const const_iterator &other) const { return !(*this == other); }
            bool operator!=(internal_ptr *ptr) const { return !(*this == ptr); }

            reference operator*() const { return *_it; }
            pointer operator->() const { return _it.operator->(); }

            const_iterator &operator++() {
                ++_it;
                return *this;
            }
            const_iterator operator++(int) { return _it++; }
            const_iterator &operator--() {
                --_it;
                return *this;
            }
            const_iterator operator--(int) { return _it--; }

            void swap(const_iterator &lhs, const_iterator& rhs) { std::swap(lhs, rhs); }

        private:
            iterator _it;

            const_iterator(internal_ptr *ptr) : _it(ptr) {}
        };

        class reverse_iterator {
            friend class map;
            friend class iterator;

            using internal_ptr = map::internal_ptr;
        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = map::value_type;
            using reference = map::reference;
            using const_reference = map::const_reference;
            using pointer = map::pointer;
            using const_pointer = map::const_pointer;
            using difference_type = map::difference_type;

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

            reference operator*() { return *_it; }
            const_reference operator*() const { return *_it; }
            pointer operator->() { return _it.operator->(); }
            const_pointer operator->() const { return _it.operator->() };

            void swap(reverse_iterator &lhs, reverse_iterator& rhs) {
                std::swap(lhs, rhs);
            }

        private:
            iterator _it;

            reverse_iterator(internal_ptr ptr) : _it(ptr) {}
        };

        class const_reverse_iterator {
            friend class map;
            friend class iterator;

            using internal_ptr = map::internal_ptr;
        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = map::value_type;
            using reference = map::const_reference;
            using pointer = map::const_pointer;
            using difference_type = map::difference_type;

            const_reverse_iterator(const const_reverse_iterator &other) = default;
            const_reverse_iterator(const_reverse_iterator &&other) = default;

            const_reverse_iterator &operator=(const const_reverse_iterator &rhs) = default;
            const_reverse_iterator &operator=(internal_ptr ptr) {
                this->_it = ptr;
                return *this;
            }

            bool operator==(const const_reverse_iterator &rhs) const { return this->_it == rhs._it; }
            bool operator==(internal_ptr ptr) const { return this->_it == ptr; }
            bool operator!=(const const_reverse_iterator &rhs) const { return !(*this == rhs); }
            bool operator!=(internal_ptr ptr) const { return !(*this == ptr); }

            const_reverse_iterator &operator++() {
                --_it;
                return *this;
            }
            const_reverse_iterator operator++(int) { return _it--; }
            const_reverse_iterator &operator--() {
                ++_it;
                return *this;
            }
            const_reverse_iterator operator--(int) { return _it++; }

            reference operator*() const { return *_it; }
            pointer operator->() const { return _it.operator->(); }

            void swap(const_reverse_iterator &lhs, const_reverse_iterator& rhs) {
                std::swap(lhs, rhs);
            }

        private:
            iterator _it;

            const_reverse_iterator(internal_ptr ptr) : _it(ptr) {}
        };

        /* Constructors/Destructors.  */
        map(const key_compare &keq = key_compare()) noexcept;
        explicit map(const map &other) noexcept;
        map(map &&other) noexcept;
        ~map();
        map &operator=(map rhs);

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

        /* Element access.  */
        mapped_type &operator[](const key_type &key);
        mapped_type &operator[](key_type &&key);
        mapped_type &at(const key_type &key) noexcept(false);
        const mapped_type &at(const key_type &key) const noexcept(false);

        /* Modifiers.  */
        std::pair<iterator, bool> insert(const value_type &val);
        template<class P>
        std::pair<iterator, bool> insert(P &&val, typename std::enable_if<std::is_constructible<P&&, value_type>::value, enabler>::type = enabler());
        template<class... Args>
        std::pair<iterator, bool> emplace(Args&&... args);
        iterator erase(const_iterator pos);
        size_type erase(const key_type &key);
        iterator erase(const_iterator first, const_iterator last);
        void clear() noexcept;
        void swap(map &x);

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

        friend void swap (map& lhs, map& rhs) {
            using std::swap;

            swap(lhs._root, rhs._root);
            swap(lhs._sentinel, rhs._sentinel);
            swap(lhs._less, rhs._less);
            swap(lhs._size, rhs._size);
        }

    private:
        internal_ptr _copy_tree(internal_ptr other_root);
        internal_ptr _construct_new_element(const_reference val);
        template<class P>
        internal_ptr _construct_new_element(P &&val, typename std::enable_if<std::is_constructible<P&&, value_type>::value, enabler>::type = enabler());
        internal_ptr _construct_new_element(internal_ptr val);
        internal_ptr _construct_new_element(const key_type &key);
        internal_ptr _construct_new_element(key_type &&key);
        std::pair<iterator, bool> _handle_elem_found(internal_ptr ptr, to_ignore obj);
        std::pair<iterator, bool> _handle_elem_found(internal_ptr ptr, to_delete obj);
        std::pair<iterator, bool> _handle_elem_not_found(internal_ptr ptr);
        key_type &_get_key(rb_node<node_type> *tnode);
        bool _is_equal_key(const key_type &lhs_key, const key_type &rhs_key) const;
        std::pair<rb_node<node_type> *, size_type> _erase(const_iterator pos);
    };

    /* Implementation.  */

    /* Public member functions.  */
    template<typename K, typename V, class Less>
    map<K, V, Less>::map(const key_compare &keq) noexcept : _root(nullptr), _size(0), _less(keq) {
        _sentinel = new rb_node<node_type>();
    }

    template<typename K, typename V, class Less>
    map<K, V, Less>::map(const map &other) noexcept {
        /* Create an exact copy of this map, O(n).  */
        _root = _copy_tree(other._root);
        _sentinel = new rb_node<node_type>();
        _sentinel->left = _root;
        _root->parent = _sentinel;
        _less = other._less;
    }

    template<typename K, typename V, class Less>
    map<K, V, Less>::map(map &&other) noexcept : map() {
        swap(*this, other);
    }

    template<typename K, typename V, class Less>
    map<K, V, Less>::~map() {
        if (_root != nullptr) {
            clear();
            delete _sentinel;
        } else {
            if (_sentinel) delete _sentinel;
        }
    }

    template<typename K, typename V, class Less>
    map<K, V, Less> &map<K, V, Less>::operator=(map rhs) {
        /* Copy and swap idiom, let the compiler handle the copy of the argument.  */
        swap(*this, rhs);

        return *this;
    }

    template<typename K, typename V, class Less>
    map_t::iterator map<K, V, Less>::begin() noexcept {
        internal_ptr current = _root;

        if (current) {
            while (current->left != nullptr) {
                current = current->left;
            }
        }

        return iterator(current);
    }

    template<typename K, typename V, class Less>
    map_t::const_iterator map<K, V, Less>::begin() const noexcept {
        return const_cast<map<K, V, Less>*>(this)->begin();
    }

    template<typename K, typename V, class Less>
    map_t::iterator map<K, V, Less>::end() noexcept {
        return iterator(_sentinel);
    }

    template<typename K, typename V, class Less>
    map_t::const_iterator map<K, V, Less>::end() const noexcept {
        return const_cast<map<K, V, Less>*>(this)->end();
    }

    template<typename K, typename V, class Less>
    map_t::reverse_iterator map<K, V, Less>::rbegin() noexcept {
        internal_ptr current = _root;

        if (current) {
            while (current->right != nullptr) {
                current = current->right;
            }
        }

        return reverse_iterator(current);
    }

    template<typename K, typename V, class Less>
    map_t::const_reverse_iterator map<K, V, Less>::rbegin() const {
        return const_cast<map<K, V, Less>*>(this)->rbegin();
    }

    template<typename K, typename V, class Less>
    map_t::reverse_iterator map<K, V, Less>::rend() noexcept {
        return reverse_iterator(_sentinel);
    }

    template<typename K, typename V, class Less>
    map_t::const_reverse_iterator map<K, V, Less>::rend() const noexcept {
        return const_cast<map<K, V, Less>*>(this)->rend();
    }

    template<typename K, typename V, class Less>
    map_t::const_iterator map<K, V, Less>::cbegin() const noexcept {
        return const_cast<map<K, V, Less>*>(this)->begin();
    }

    template<typename K, typename V, class Less>
    map_t::const_iterator map<K, V, Less>::cend() const noexcept {
        return const_cast<map<K, V, Less>*>(this)->end();
    }

    template<typename K, typename V, class Less>
    map_t::const_reverse_iterator map<K, V, Less>::crbegin() const noexcept {
        return const_cast<map<K, V, Less>*>(this)->rbegin();
    }

    template<typename K, typename V, class Less>
    map_t::const_reverse_iterator map<K, V, Less>::crend() const noexcept {
        return const_cast<map<K, V, Less>*>(this)->rend();
    }

    template<typename K, typename V, class Less>
    bool map<K, V, Less>::empty() const noexcept {
        return _size == 0;
    }

    template<typename K, typename V, class Less>
    map_t::size_type map<K, V, Less>::size() const noexcept {
        return _size;
    }

    template<typename K, typename V, class Less>
    map_t::key_compare map<K, V, Less>::key_comp() const {
        return _less;
    }

    template<typename K, typename V, class Less>
    map_t::value_compare map<K, V, Less>::value_comp() const {
        return _less;
    }

    template<typename K, typename V, class Less>
    map_t::mapped_type &map<K, V, Less>::operator[](const key_type &key) {
        return (*((insert(std::make_pair(key,mapped_type()))).first)).second;
    }

    template<typename K, typename V, class Less>
    map_t::mapped_type &map<K, V, Less>::operator[](key_type &&key) {
        return (*((insert(std::make_pair(key,mapped_type()))).first)).second;
    }

    template<typename K, typename V, class Less>
    map_t::mapped_type &map<K, V, Less>::at(const key_type &key) noexcept(false) {
        iterator it = find(key);

        /* If we found it, return the mapped value.  */
        if (it._ptr != _sentinel) return it->second;

        throw std::out_of_range("Key is not present on the map.");
    }

    template<typename K, typename V, class Less>
    const map_t::mapped_type &map<K, V, Less>::at(const key_type &key) const noexcept(false) {
        return const_cast<map<K, V, Less>*>(this)->at(key);
    }

    template<typename K, typename V, class Less>
    std::pair<map_t::iterator, bool> map<K, V, Less>::insert(const value_type &val) {
        return _rbtree_insert<map<K, V, Less>, std::pair<iterator, bool>, key_type, const value_type&>(this, val.first, val, to_ignore());
    }

    template<typename K, typename V, class Less>
    template<class P>
    std::pair<map_t::iterator, bool> map<K, V, Less>::insert(P &&val, typename std::enable_if<std::is_constructible<P&&, value_type>::value, enabler>::type) {
        return _rbtree_insert<map<K, V, Less>, std::pair<iterator, bool>, key_type, P&&>(this, val.first, std::forward<P>(val), to_ignore());
    }

    template<typename K, typename V, class Less>
    template<class... Args>
    std::pair<map_t::iterator, bool> map<K, V, Less>::emplace(Args &&... args) {
        internal_ptr val = new rb_node<node_type>(std::forward<Args>(args)...);
        return _rbtree_insert<map<K, V, Less>, std::pair<iterator, bool>, key_type, internal_ptr>(this, val->data, val, to_delete(val));
    }

    template<typename K, typename V, class Less>
    map_t::iterator map<K, V, Less>::erase(const_iterator pos) {
        return erase(pos).first;
    }

    template<typename K, typename V, class Less>
    map_t::size_type map<K, V, Less>::erase(const key_type &key) {
        return _erase(find(key)).second;
    }

    template<typename K, typename V, class Less>
    map_t::iterator map<K, V, Less>::erase(const_iterator first, const_iterator last) {
        auto it = first;

        while (it != last) it = erase(it);

        return it;
    }

    template<typename K, typename V, class Less>
    void map<K, V, Less>::clear() noexcept {
        _rbtree_destruct<map<K, V, Less>>(_root);
    }

    template<typename K, typename V, class Less>
    void map<K, V, Less>::swap(map &x) {
        swap(*this, x);
    }

    template<typename K, typename V, class Less>
    map_t::iterator map<K, V, Less>::find(const key_type &key) {
        return _rbtree_find<map<K, V, Less>>(this, key);
    }

    template<typename K, typename V, class Less>
    map_t::const_iterator map<K, V, Less>::find(const key_type &key) const {
        return const_cast<map<K, V, Less>*>(this)->find(key);
    }

    template<typename K, typename V, class Less>
    map_t::size_type map<K, V, Less>::count(const key_type &key) const {
        return find(key)._ptr != _sentinel ? 1 : 0;
    }

    template<typename K, typename V, class Less>
    map_t::iterator map<K, V, Less>::lower_bound(const key_type &key) {
        internal_ptr bound = _rbtree_find_bound<map<K, V, Less>>(this, _root, key);

        return bound == nullptr ? end() : iterator(bound);
    }

    template<typename K, typename V, class Less>
    map_t::const_iterator map<K, V, Less>::lower_bound(const key_type &key) const {
        return const_cast<map<K, V, Less>*>(this)->lower_bound(key);
    }

    template<typename K, typename V, class Less>
    map_t::iterator map<K, V, Less>::upper_bound(const key_type &key) {
        internal_ptr bound = _rbtree_find_bound<map<K, V, Less>>(this, _root, key);

        if (bound == nullptr) {
            return end();
        } else {
            if (_is_equal_key(bound->data, key)) {
                return iterator(_rbtree_successor<map<K, V, Less>>(bound));
            } else {
                return iterator(bound);
            }
        }
    }

    template<typename K, typename V, class Less>
    map_t::const_iterator map<K, V, Less>::upper_bound(const key_type &key) const {
        return const_cast<map<K, V, Less>*>(this)->upper_bound(key);
    }

    template<typename K, typename V, class Less>
    std::pair<map_t::iterator, map_t::iterator> map<K, V, Less>::equal_range(const key_type &key) {
        auto first = find(key);
        auto second(first);

        return {first, ++second};
    }

    template<typename K, typename V, class Less>
    std::pair<map_t::const_iterator, map_t::const_iterator> map<K, V, Less>::equal_range(const key_type &key) const {
        return const_cast<map<K, V, Less>*>(this)->equal_range(key);
    }

    /* Private member functions.  */
    template<typename K, typename V, class Less>
    map_t::internal_ptr map<K, V, Less>::_copy_tree(internal_ptr other_root) {
        internal_ptr new_node;

        if (other_root == nullptr) return nullptr;

        new_node = new rb_node<node_type>(other_root->data);

        new_node->left = _copy_tree(other_root->left);
        if (new_node->left) new_node->left->parent = new_node;

        new_node->right = _copy_tree(other_root->right);
        if (new_node->right) new_node->right->parent = new_node;

        return new_node;
    }

    template<typename K, typename V, class Less>
    map_t::internal_ptr map<K, V, Less>::_construct_new_element(const value_type &val) {
        return new rb_node<node_type>(val);
    }

    template<typename K, typename V, class Less>
    template<class P>
    map_t::internal_ptr map<K, V, Less>::_construct_new_element(P &&val, typename std::enable_if<std::is_constructible<P&&, value_type>::value, enabler>::type) {
        return new rb_node<node_type>(std::forward<node_type>(val));
    }

    template<typename K, typename V, class Less>
    map_t::internal_ptr map<K, V, Less>::_construct_new_element(internal_ptr val) {
        return val;
    }

    template<typename K, typename V, class Less>
    map_t::internal_ptr map<K, V, Less>::_construct_new_element(const key_type &key) {
        return new rb_node<node_type>(key, mapped_type());
    }

    template<typename K, typename V, class Less>
    map_t::internal_ptr map<K, V, Less>::_construct_new_element(key_type &&key) {
        return new rb_node<node_type>(std::forward<key_type>(key), mapped_type());
    }

    template<typename K, typename V, class Less>
    std::pair<map_t::iterator, bool> map<K, V, Less>::_handle_elem_found(internal_ptr ptr, to_ignore obj) {
        return {ptr, false};
    }

    template<typename K, typename V, class Less>
    std::pair<map_t::iterator, bool> map<K, V, Less>::_handle_elem_found(internal_ptr ptr, to_delete obj) {
        delete obj.ptr;
        return {ptr, false};
    }

    template<typename K, typename V, class Less>
    std::pair<map_t::iterator, bool> map<K, V, Less>::_handle_elem_not_found(internal_ptr ptr) {
        return {ptr, true};
    }

    template<typename K, typename V, class Less>
    map_t::key_type &map<K, V, Less>::_get_key(rb_node<node_type> *tnode) {
        return tnode->data.first;
    }

    template<typename K, typename V, class Less>
    bool map<K, V, Less>::_is_equal_key(const key_type &lhs_key, const key_type &rhs_key) const {
        return _less(lhs_key, rhs_key) || _less(rhs_key, lhs_key);
    }

    template<typename K, typename V, class Less>
    std::pair<rb_node<map_t::node_type> *, map_t::size_type> map<K, V, Less>::_erase(map::const_iterator pos) {
        internal_ptr save, successor, next_elem_ptr;
        rb_node<node_type> *erase_ptr;
        size_t count = 0;

        if (pos != end()) {
            save = _sentinel;
            _root->parent = nullptr;

            successor = _rbtree_successor<map<K, V, Less>>(pos._ptr);
            erase_ptr = _rbtree_erase<map<K, V, Less>>(this, pos._ptr, successor);

            if (erase_ptr != _root) {
                next_elem_ptr = erase_ptr == successor ? pos._ptr : successor;

                delete erase_ptr;

                /* Update the sentinel node.  */
                _sentinel = save;
                _root->parent = _sentinel;
                _sentinel->left = _root;
                --_size;
                count++;

                return {successor != nullptr ? next_elem_ptr : end(), count};
            } else {
                delete _root;
                _sentinel->left = nullptr;
                _root = nullptr;
                _size = 0;
            }
        }

        return {end(), count};
    }
}