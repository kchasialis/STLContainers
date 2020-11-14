#pragma once

#define multimap_t typename multimap<K, V, Less>

#include "rbtree_internal.h"

namespace adt {

    template<typename K, typename V, class Less = std::less<K>>
    class multimap {
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
        struct multimap_node {
            value_type data;
            multimap_node *next;
            multimap_node *previous;

            multimap_node() : data(), next(nullptr), previous(nullptr) {}
            multimap_node(const value_type &_data) : data(_data), next(nullptr), previous(nullptr) {}
            multimap_node(value_type &&_data) : data(_data), next(nullptr), previous(nullptr) {}
            template<typename... Args>
            multimap_node(Args&&... args) : data(std::forward<Args>(args)...), next(nullptr), previous(nullptr) {}
            multimap_node(const multimap_node &other) = default;
            multimap_node(multimap_node &&other) = default;
        };

        struct enabler {};

        using node_type = multimap_node*;
        using internal_ptr = rb_node<node_type>*;

        internal_ptr _root;
        internal_ptr _sentinel;
        size_type _size;
        key_compare _less;

    public:
        class iterator {
            friend class multimap;
            using internal_ptr = multimap::internal_ptr;

        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = multimap::value_type;
            using reference = multimap::reference;
            using const_reference = multimap::const_reference;
            using pointer = multimap::pointer;
            using const_pointer = multimap::const_pointer;
            using difference_type = multimap::difference_type;

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

            reference operator*() { return _inner_ptr->data; }
            const_reference operator*() const { return _inner_ptr->data; }
            pointer operator->() { return &(_inner_ptr->data); }
            const_pointer operator->() const { return &(_inner_ptr->data); }

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

        class const_iterator {
            friend class multimap;
            friend class iterator;
            using internal_ptr = multimap::internal_ptr;

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = multimap::value_type;
            using reference = multimap::const_reference;
            using pointer = multimap::const_pointer;
            using difference_type = multimap::difference_type;

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
            friend class multimap;
            friend class iterator;
            using internal_ptr = multimap::internal_ptr;

        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = multimap::value_type;
            using reference = multimap::reference;
            using const_reference = multimap::const_reference;
            using pointer = multimap::pointer;
            using const_pointer = multimap::const_pointer;
            using difference_type = multimap::difference_type;

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
            const_pointer operator->() const { return _it.operator->(); }

            void swap(reverse_iterator &lhs, reverse_iterator& rhs) {
                std::swap(lhs, rhs);
            }

        private:
            iterator _it;

            reverse_iterator(internal_ptr ptr) : _it(ptr) {}
        };

        class const_reverse_iterator {
            friend class multimap;
            friend class iterator;

            using internal_ptr = multimap::internal_ptr;
        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = multimap::value_type;
            using reference = multimap::const_reference;
            using pointer = multimap::const_pointer;
            using difference_type = multimap::difference_type;

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
        multimap(const key_compare &keq = key_compare()) noexcept;
        multimap(const multimap &other) noexcept;
        multimap(multimap &&other) noexcept;
        ~multimap();
        multimap &operator=(multimap rhs);

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
        template<class P>
        iterator insert(P &&val, typename std::enable_if<std::is_constructible<P&&, value_type>::value, enabler>::type = enabler());
        template <class... Args>
        iterator emplace(Args &&... args);
        iterator erase(const_iterator pos);
        size_type erase(const value_type &val);
        iterator erase(const_iterator first, const_iterator last);
        void clear() noexcept;
        void swap(multimap &other);

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

        friend void swap(multimap &lhs, multimap &rhs) {
            using std::swap;

            swap(lhs._root, rhs._root);
            swap(lhs._sentinel, rhs._sentinel);
            swap(lhs._less, rhs._less);
            swap(lhs._size, rhs._size);
        }

    private:
        internal_ptr _copy_tree(internal_ptr other_root);
        iterator _add_to_list(internal_ptr ptr, multimap_node *new_node);
        internal_ptr _construct_new_element(const value_type &val);
        template<typename P>
        internal_ptr _construct_new_element(P &&val, typename std::enable_if<std::is_constructible<P&&, value_type>::value, enabler>::type = enabler());
        internal_ptr _construct_new_element(multimap_node *val);
        iterator _handle_elem_found(internal_ptr ptr, const value_type &val);
        template<typename P>
        iterator _handle_elem_found(internal_ptr ptr, P &&val, typename std::enable_if<std::is_constructible<P&&, value_type>::value, enabler>::type = enabler());
        iterator _handle_elem_found(internal_ptr ptr, multimap_node *val);
        iterator _handle_elem_not_found(internal_ptr ptr);
        key_type &_get_key(internal_ptr ptr);
        bool _is_equal_key(const key_type &lhs_key, const key_type &rhs_key) const;
        size_type _erase_list(internal_ptr erase_ptr);
        size_type _erase_node(internal_ptr erase_ptr);
        std::pair<internal_ptr, size_type> _erase(const_iterator pos, bool erase_all);
    };

    /* Implementation.  */

    /* Public member functions.  */
    template<typename K, typename V, class Less>
    multimap<K, V, Less>::multimap(const key_compare &keq) noexcept {
        _sentinel = new rb_node<node_type>();
    }

    template<typename K, typename V, class Less>
    multimap<K, V, Less>::multimap(const multimap &other) noexcept {
        if (this != &other) {
            /* Create an exact copy of this set, O(n).  */
            _root = _copy_tree(other._root);
            _sentinel = new rb_node<node_type>();
            _sentinel->left = _root;
            _root->parent = _sentinel;
            _less = other._less;
        }
    }

    template<typename K, typename V, class Less>
    multimap<K, V, Less>::multimap(multimap &&other) noexcept {
        swap(*this, other);
    }

    template<typename K, typename V, class Less>
    multimap<K, V, Less>::~multimap() {
        if (_root != nullptr) {
            clear();
            delete _sentinel;
        } else {
            if (_sentinel) delete _sentinel;
        }
    }

    template<typename K, typename V, class Less>
    multimap<K, V, Less> &multimap<K, V, Less>::operator=(multimap rhs) {
        /* Copy and swap idiom, let the compiler handle the copy of the argument.  */
        swap(*this, rhs);

        return *this;
    }

    template<typename K, typename V, class Less>
    multimap_t::iterator multimap<K, V, Less>::begin() noexcept {
        internal_ptr current = _root;

        if (current) {
            while (current->left != nullptr) {
                current = current->left;
            }
        }

        return iterator(current);
    }

    template<typename K, typename V, class Less>
    multimap_t::const_iterator multimap<K, V, Less>::begin() const noexcept {
        return const_cast<multimap<K, V, Less>*>(this)->begin();
    }

    template<typename K, typename V, class Less>
    multimap_t::iterator multimap<K, V, Less>::end() noexcept {
        return iterator(_sentinel);
    }

    template<typename K, typename V, class Less>
    multimap_t::const_iterator multimap<K, V, Less>::end() const noexcept {
        return const_cast<multimap<K, V, Less>*>(this)->end();
    }

    template<typename K, typename V, class Less>
    multimap_t::reverse_iterator multimap<K, V, Less>::rbegin() noexcept {
        internal_ptr current = _root;

        if (current) {
            while (current->right != nullptr) {
                current = current->right;
            }
        }

        return reverse_iterator(current);
    }

    template<typename K, typename V, class Less>
    multimap_t::const_reverse_iterator multimap<K, V, Less>::rbegin() const {
        return const_cast<multimap<K, V, Less>*>(this)->rbegin();
    }

    template<typename K, typename V, class Less>
    multimap_t::reverse_iterator multimap<K, V, Less>::rend() noexcept {
        return reverse_iterator(_sentinel);
    }

    template<typename K, typename V, class Less>
    multimap_t::const_reverse_iterator multimap<K, V, Less>::rend() const noexcept {
        return const_cast<multimap<K, V, Less>*>(this)->rend();
    }

    template<typename K, typename V, class Less>
    multimap_t::const_iterator multimap<K, V, Less>::cbegin() const noexcept {
        return const_cast<multimap<K, V, Less>*>(this)->begin();
    }

    template<typename K, typename V, class Less>
    multimap_t::const_iterator multimap<K, V, Less>::cend() const noexcept {
        return const_cast<multimap<K, V, Less>*>(this)->end();
    }

    template<typename K, typename V, class Less>
    multimap_t::const_reverse_iterator multimap<K, V, Less>::crbegin() const noexcept {
        return const_cast<multimap<K, V, Less>*>(this)->rbegin();
    }

    template<typename K, typename V, class Less>
    multimap_t::const_reverse_iterator multimap<K, V, Less>::crend() const noexcept {
        return const_cast<multimap<K, V, Less>*>(this)->rend();
    }

    template<typename K, typename V, class Less>
    bool multimap<K, V, Less>::empty() const noexcept {
        return _size == 0;
    }

    template<typename K, typename V, class Less>
    multimap_t::size_type multimap<K, V, Less>::size() const noexcept {
        return _size;
    }

    template<typename K, typename V, class Less>
    multimap_t::key_compare multimap<K, V, Less>::key_comp() const {
        return _less;
    }

    template<typename K, typename V, class Less>
    multimap_t::value_compare multimap<K, V, Less>::value_comp() const {
        return _less;
    }

    template<typename K, typename V, class Less>
    multimap_t::iterator multimap<K, V, Less>::insert(const value_type &val) {
        return _rbtree_insert<multimap<K, V, Less>, iterator, key_type, const value_type&>(this, val.first, val, val);
    }

    template<typename K, typename V, class Less>
    template<class P>
    multimap_t::iterator multimap<K, V, Less>::insert(P &&val, typename std::enable_if<std::is_constructible<P&&, value_type>::value, enabler>::type) {
        return _rbtree_insert<multimap<K, V, Less>, iterator, key_type, P&&>(this, val.first, std::forward<P>(val), std::forward<P>(val));
    }

    template<typename K, typename V, class Less>
    template<class... Args>
    multimap_t::iterator multimap<K, V, Less>::emplace(Args &&... args) {
        multimap_node *val = new multimap_node(std::forward<Args>(args)...);
        return _rbtree_insert<multimap<K, V, Less>, iterator, key_type, multimap_node*>(this, val->data.first, val, val);
    }

    template<typename K, typename V, class Less>
    multimap_t::iterator multimap<K, V, Less>::erase(const_iterator pos) {
        return _erase(pos, false).first;
    }

    template<typename K, typename V, class Less>
    multimap_t::size_type multimap<K, V, Less>::erase(const value_type &val) {
        return _erase(find(val), true).second;
    }

    template<typename K, typename V, class Less>
    multimap_t::iterator multimap<K, V, Less>::erase(const_iterator first, const_iterator last) {
        auto it = first;

        while (it != last) it = erase(it);

        return it;
    }

    template<typename K, typename V, class Less>
    void multimap<K, V, Less>::clear() noexcept {
        _rbtree_destruct<multimap<K, V, Less>>(_root);
    }

    template<typename K, typename V, class Less>
    void multimap<K, V, Less>::swap(multimap &other) {
        swap(*this, other);
    }

    template<typename K, typename V, class Less>
    multimap_t::iterator multimap<K, V, Less>::find(const key_type &key) {
        return _rbtree_find<multimap<K, V, Less>>(this, key);
    }

    template<typename K, typename V, class Less>
    multimap_t::const_iterator multimap<K, V, Less>::find(const key_type &key) const {
        return const_cast<multimap<K, V, Less>*>(this)->find(key);
    }

    template<typename K, typename V, class Less>
    multimap_t::size_type multimap<K, V, Less>::count(const key_type &key) const {
        multimap_node *current;
        multimap_node *found_ptr = const_cast<multimap<K, V, Less>*>(this)->find(key)._ptr;
        size_t count = 0;

        if (found_ptr == _sentinel) return count;

        current = found_ptr->data;
        while (current != nullptr) {
            count++;
            current = current->next;
        }

        return count;
    }

    template<typename K, typename V, class Less>
    multimap_t::iterator multimap<K, V, Less>::lower_bound(const key_type &key) {
        internal_ptr bound = _rbtree_find_bound<multimap<K, V, Less>>(this, _root, key);

        return bound == nullptr ? end() : iterator(bound);
    }

    template<typename K, typename V, class Less>
    multimap_t::const_iterator multimap<K, V, Less>::lower_bound(const key_type &key) const {
        return const_cast<multimap<K, V, Less>*>(this)->lower_bound(key);
    }

    template<typename K, typename V, class Less>
    multimap_t::iterator multimap<K, V, Less>::upper_bound(const key_type &key) {
        internal_ptr bound = _rbtree_find_bound<multimap<K, V, Less>>(this, _root, key);

        if (bound == nullptr) {
            return end();
        } else {
            if (_is_equal_key(bound->data, key)) {
                return iterator(_rbtree_successor<multimap<K, V, Less>>(bound));
            } else {
                return iterator(bound);
            }
        }
    }

    template<typename K, typename V, class Less>
    multimap_t::const_iterator multimap<K, V, Less>::upper_bound(const key_type &key) const {
        return const_cast<multimap<K, V, Less>*>(this)->upper_bound(key);
    }

    template<typename K, typename V, class Less>
    std::pair<multimap_t::iterator, multimap_t::iterator> multimap<K, V, Less>::equal_range(const key_type &key) {
        auto first = find(key);
        auto second(first);

        return {first, ++second};
    }

    template<typename K, typename V, class Less>
    std::pair<multimap_t::const_iterator, multimap_t::const_iterator> multimap<K, V, Less>::equal_range(const key_type &key) const {
        return const_cast<multimap<K, V, Less>*>(this)->equal_range(key);
    }

    /* Private member functions.  */
    template<typename K, typename V, class Less>
    multimap_t::internal_ptr multimap<K, V, Less>::_copy_tree(internal_ptr other_root) {
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
    multimap_t::iterator multimap<K, V, Less>::_add_to_list(internal_ptr ptr, multimap_node *new_node) {
        new_node->next = ptr->data;
        ptr->data = new_node;
        new_node->next->previous = new_node;
        _size++;

        return ptr;
    }

    template<typename K, typename V, class Less>
    multimap_t::internal_ptr multimap<K, V, Less>::_construct_new_element(const value_type &val) {
        return new rb_node<node_type>(new multimap_node(val));
    }

    template<typename K, typename V, class Less>
    template<typename P>
    multimap_t::internal_ptr multimap<K, V, Less>::_construct_new_element(P &&val, typename std::enable_if<std::is_constructible<P&&, value_type>::value, enabler>::type) {
        return new rb_node<node_type>(new multimap_node(std::forward<P>(val)));
    }

    template<typename K, typename V, class Less>
    multimap_t::internal_ptr multimap<K, V, Less>::_construct_new_element(multimap_node *val) {
        return new rb_node<node_type>(val);
    }

    template<typename K, typename V, class Less>
    multimap_t::iterator multimap<K, V, Less>::_handle_elem_found(internal_ptr ptr, const value_type &val) {
        return _add_to_list(ptr, new multimap_node(val));
    }

    template<typename K, typename V, class Less>
    template<typename P>
    multimap_t::iterator multimap<K, V, Less>::_handle_elem_found(internal_ptr ptr, P &&val, typename std::enable_if<std::is_constructible<P&&, value_type>::value, enabler>::type) {
        return _add_to_list(ptr, new multimap_node(std::forward<P>(val)));
    }

    template<typename K, typename V, class Less>
    multimap_t::iterator multimap<K, V, Less>::_handle_elem_found(internal_ptr ptr, multimap_node *val) {
        return _add_to_list(ptr, val);
    }

    template<typename K, typename V, class Less>
    multimap_t::iterator multimap<K, V, Less>::_handle_elem_not_found(internal_ptr ptr) {
        return ptr;
    }

    template<typename K, typename V, class Less>
    multimap_t::key_type &multimap<K, V, Less>::_get_key(internal_ptr ptr) {
        return ptr->data->data.first;
    }

    template<typename K, typename V, class Less>
    bool multimap<K, V, Less>::_is_equal_key(const key_type &lhs_key, const key_type &rhs_key) const {
        return _less(lhs_key, rhs_key) || _less(rhs_key, lhs_key);
    }

    template<typename K, typename V, class Less>
    multimap_t::size_type multimap<K, V, Less>::_erase_list(internal_ptr erase_ptr) {
        multimap_node *current, *to_delete;
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

    template<typename K, typename V, class Less>
    multimap_t::size_type multimap<K, V, Less>::_erase_node(internal_ptr erase_ptr) {
        multimap_node *head = erase_ptr->data;

        erase_ptr->data = head->next;
        delete head;

        return 1;
    }

    template<typename K, typename V, class Less>
    std::pair<multimap_t::internal_ptr, multimap_t::size_type> multimap<K, V, Less>::_erase(const_iterator pos, bool erase_all) {
        internal_ptr save, successor, next_elem_ptr, erase_ptr;
        size_t count = 0;

        if (pos != end()) {
            save = _sentinel;
            _root->parent = nullptr;

            successor = _rbtree_successor<multimap<K, V, Less>>(pos._ptr);
            erase_ptr = _rbtree_erase<multimap<K, V, Less>>(this, pos._ptr, successor);

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
                delete _root->data;
                delete _root;
                _sentinel->left = nullptr;
                _root = nullptr;
                _size = 0;
            }
        }

        return {end(), count};
    }
}