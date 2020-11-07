#pragma once

#include <cstdlib>
#include <cassert>
#include <functional>

#include "hash_internal.h"

#define umultiset typename unordered_multiset<Key, Hash, Eq>

namespace adt {

    template<typename Key, class Hash = std::hash<Key>, class Eq = std::equal_to<Key>>
    class unordered_multiset {
    public:
        using key_type = Key;
        using value_type = Key;
        using hasher = Hash;
        using key_equal = Eq;
        using size_type = size_t;
        using pointer = value_type *;
        using reference = value_type &;
        using const_reference = const value_type &;
        using const_pointer = const value_type *;
        using difference_type = std::ptrdiff_t;
        class iterator;
        class const_iterator;

    private:
        struct hash_info {
            size_t pos;
            uint64_t hash;
            ctrl_t h2_hash;
        };

        struct find_insert_info {
            iterator it;
            bool found_deleted;
            size_t del_pos;
            size_t empty_pos;
        };

        struct multiset_node {
            value_type data;
            multiset_node *next;

            multiset_node() : data(), next(nullptr) {}
            multiset_node(const value_type &_data) : data(_data), next(nullptr) {}
            multiset_node(value_type &&_data) : data(_data), next(nullptr) {}
            multiset_node(const multiset_node &other) = default;
            multiset_node(multiset_node &&other) = default;
        };

        using internal_ptr = multiset_node *;

        internal_ptr *_slots;
        ctrl_t *_ctrls;
        hasher _hasher;
        size_type _size;
        size_type _n_slots;
        size_type _capacity;
        size_type _first_elem_pos;
        key_equal _keq;

    public:
        class iterator {
            friend class unordered_multiset;

            template<class Container, typename R, typename K, typename V, typename... Args>
            friend R _hash_insert(Container *cnt, const K &key, V val, Args &&... args);

            template<class Container>
            friend container::iterator _hash_find(Container *cnt, const container::key_type &key);

            template<class Container>
            friend container::find_insert_info _hash_find_or_prepare_insert(Container *cnt, const container::key_type &key, container::size_type pos, ctrl_t h2_hash);

            using internal_ptr = unordered_multiset::internal_ptr;

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = unordered_multiset::value_type;
            using reference = unordered_multiset::const_reference;
            using pointer = unordered_multiset::const_pointer;
            using difference_type = unordered_multiset::difference_type;

            iterator(const iterator &other) = default;
            iterator(iterator &&other) = default;

            iterator &operator=(const iterator &other) = default;
            iterator &operator=(internal_ptr *ptr) {
                this->_ptr = ptr;
                return *this;
            }

            bool operator==(const iterator &other) const { return this->_ptr == other._ptr; }
            bool operator==(internal_ptr *ptr) const { return this->_ptr == ptr; }
            bool operator!=(const iterator &other) const { return !(*this == other); }
            bool operator!=(internal_ptr *ptr) const { return !(*this == ptr); }

            iterator &operator++() {
                /* If its already end(), dont try to increment it.  */
                if (_ptr == nullptr || *_ptr == (internal_ptr) 0x1) return *this;

                if (_it->next != nullptr) {
                    /* Means we have multiple entries with same key, iterate over them first.  */
                    _it = _it->next;
                }
                else {
                    /* Means we should move to the next slot in the table.  */
                    ++_ptr;
                    while (*_ptr == nullptr && *_ptr != (internal_ptr) 0x1) ++_ptr;

                    _it = *_ptr;
                }

                return *this;
            }
            iterator operator++(int) & {
                auto temp(*this);
                ++(*this);
                return temp;
            }

            reference operator*() const {
                return _it->data;
            }
            pointer operator->() const {
                return &(_it->data);
            }

        private:
            internal_ptr *_ptr;
            internal_ptr _it;

            iterator(internal_ptr *ptr = nullptr) : _ptr(ptr) {
                if (ptr != nullptr) _it = *_ptr;
            }
        };

        class const_iterator {
            friend class unordered_multiset;
            using internal_ptr = unordered_multiset::internal_ptr;

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = unordered_multiset::value_type;
            using reference = unordered_multiset::const_reference;
            using pointer = unordered_multiset::const_pointer;
            using difference_type = unordered_multiset::difference_type;

            /* Implicit conversion from iterator.  */
            const_iterator(iterator it) : _it(std::move(it)) {}

            bool operator==(const const_iterator &other) const {
                return this->_ptr == other._ptr;
            }
            bool operator==(internal_ptr *ptr) const {
                return _it == ptr;
            }
            bool operator!=(const const_iterator &other) const {
                return !(*this == other);
            }
            bool operator!=(internal_ptr *ptr) const {
                return !(*this == ptr);
            }

            reference operator*() const { return *_it; }
            pointer operator->() const { return _it.operator->(); }

            const_iterator &operator++() {
                ++_it;
                return *this;
            }
            const_iterator operator++(int) { return _it++; }

        private:
            iterator _it;

            const_iterator(internal_ptr *ptr) : _it(ptr) {}
        };

        /* Constructors/Destructors.  */
        explicit unordered_multiset(size_type cap = 16, const hasher &hash = hasher(),
                                    const key_equal &keq = key_equal()) noexcept;
        unordered_multiset(const unordered_multiset &other);
        unordered_multiset(unordered_multiset &&other) noexcept;
        unordered_multiset &operator=(unordered_multiset rhs);
        ~unordered_multiset() noexcept;

        /* Capacity.  */
        bool empty() const noexcept;
        size_type size() const noexcept;

        /* Iterators.  */
        iterator begin() const noexcept;
        iterator end() const noexcept;
        const_iterator cbegin() const noexcept;
        const_iterator cend() const noexcept;

        /* Observers.  */
        hasher hash_function() const;
        key_equal key_eq() const;

        /* Modifiers.  */
        iterator insert(const value_type &val);
        iterator insert(value_type &&val);
        template<class... Args>
        iterator emplace(Args &&... args);
        iterator erase(const_iterator pos);
        size_type erase(const key_type &key);
        void clear() noexcept;
        void swap(unordered_multiset &other);

        /* Element lookup.  */
        iterator find(const key_type &key);
        const_iterator find(const key_type &key) const;
        size_type count(const key_type &key) const;
        std::pair<iterator, iterator> equal_range(const key_type &key);
        std::pair<iterator, const_iterator> equal_range(const key_type &key) const;

        friend void swap(unordered_multiset &lhs, unordered_multiset &rhs) {
            using std::swap;

            swap(lhs._slots, rhs._slots);
            swap(lhs._ctrls, rhs._ctrls);
            swap(lhs._size, rhs._size);
            swap(lhs._capacity, rhs._capacity);
            swap(lhs._first_elem_pos, rhs._first_elem_pos);
            swap(lhs._hasher, rhs._hasher);
            swap(lhs._keq, rhs._keq);
        }

        template<class Container>
        friend void _hash_construct(Container *cnt);

        template<class Container>
        friend void _hash_destruct(Container *cnt);

        template<class Container>
        friend void _hash_clear(Container *cnt);

        template<class Container>
        friend void _hash_rehash(Container *cnt);

        template<class Container>
        friend container::find_insert_info _hash_find_or_prepare_insert(Container *cnt, const container::key_type &key, container::size_type pos, ctrl_t h2_hash);

        template<class Container>
        friend container::hash_info _hash_get_hash_info(Container *cnt, const container::key_type &key);

        template<class Container>
        friend void _hash_check_load_factor(Container *cnt, container::size_type, uint64_t hash, container::size_type &pos);

        template<class Container, typename R, typename K, typename V, typename... Args>
        friend R _hash_insert(Container *cnt, const K &key, V val, Args &&... args);

        template<class Container>
        friend std::pair<container::size_type, container::size_type> _hash_erase(Container *cnt, container::internal_ptr *ptr, bool erase_all);

        template<class Container>
        friend container::iterator _hash_find(Container *cnt, const container::key_type &key);

    private:
        void _rehash();
        hash_info _get_hash_info(const key_type &key);
        void _check_load_factor(uint64_t hash, size_type &pos);
        const key_type &_get_slot_key(internal_ptr slot);
        
        find_insert_info _find_or_prepare_insert(const key_type &key, size_type pos, ctrl_t h2_hash);
        iterator _handle_elem_found(const iterator &it, const value_type &val);
        iterator _handle_elem_found(const iterator &it, value_type &&val);
        iterator _handle_elem_found(const iterator &it, internal_ptr new_node);
        iterator _handle_elem_not_found(const iterator &it);
        
        iterator _add_to_list(const iterator &it, internal_ptr new_node);
        internal_ptr _construct_new_element(const_reference val);
        internal_ptr _construct_new_element(value_type &&val);
        internal_ptr _construct_new_element(internal_ptr val);

        size_type _delete_all_slots(size_type pos);
        size_type _delete_slot(size_type pos);
        std::pair<size_type, size_type> _erase(internal_ptr *ptr, bool erase_all);
    };

    /* Implementation.  */

    /* Public member functions.  */
    template<typename Key, class Hash, class Eq>
    unordered_multiset<Key, Hash, Eq>::unordered_multiset(size_type cap, const hasher &hash, const key_equal &keq) noexcept
        : _size(0), _capacity(cap), _hasher(hash), _keq(keq) {
        _hash_construct<>(this);
        _n_slots = 0;
    }

    template<typename Key, class Hash, class Eq>
    unordered_multiset<Key, Hash, Eq>::unordered_multiset(const unordered_multiset &other) {
        internal_ptr this_current, other_current;

        this->_capacity = other._capacity;
        this->_size = other._size;
        this->_n_slots = other._n_slots;
        this->_first_elem_pos = other._first_elem_pos;
        this->_slots = (internal_ptr *) calloc(sizeof(internal_ptr), this->_capacity + 1);
        this->_slots[this->_capacity] = (internal_ptr) 0x1;
        this->_ctrls = new ctrl_t[this->_capacity];

        /* Deep copy of the data. Avoid this lol.  */
        for (size_t i = 0 ; i < other._capacity ; i++) {
            this->_ctrls[i] = other._ctrls[i];

            if (is_full_slot(other._ctrls[i])) {
                this_current = new multiset_node(other._slots[i]->data);
                this->_slots[i] = this_current;

                other_current = other._slots[i]->next;
                while (other_current != nullptr) {
                    this_current->next = new multiset_node(other_current->data);
                    other_current = other_current->next;
                    this_current = this_current->next;
                }
            }
        }
    }

    template<typename Key, class Hash, class Eq>
    unordered_multiset<Key, Hash, Eq>::unordered_multiset(unordered_multiset &&other) noexcept : unordered_multiset() {
        swap(*this, other);
    }

    template<typename Key, class Hash, class Eq>
    unordered_multiset<Key, Hash, Eq> &unordered_multiset<Key, Hash, Eq>::operator=(unordered_multiset rhs) {
        /* Copy and swap idiom, let the compiler handle the copy of the argument.  */
        swap(*this, rhs);
        return *this;
    }

    template<typename Key, class Hash, class Eq>
    unordered_multiset<Key, Hash, Eq>::~unordered_multiset() noexcept {
        _hash_destruct<>(this);
    }

    template<typename Key, class Hash, class Eq>
    bool unordered_multiset<Key, Hash, Eq>::empty() const noexcept {
        return _size == 0;
    }

    template<typename Key, class Hash, class Eq>
    umultiset::size_type unordered_multiset<Key, Hash, Eq>::size() const noexcept {
        return _size;
    }

    template<typename Key, class Hash, class Eq>
    umultiset::iterator unordered_multiset<Key, Hash, Eq>::begin() const noexcept {
        return iterator(&_slots[_first_elem_pos]);
    }

    template<typename Key, class Hash, class Eq>
    umultiset::iterator unordered_multiset<Key, Hash, Eq>::end() const noexcept {
        return iterator(&_slots[_capacity]);
    }

    template<typename Key, class Hash, class Eq>
    umultiset::const_iterator unordered_multiset<Key, Hash, Eq>::cbegin() const noexcept {
        return const_iterator(&_slots[_first_elem_pos]);
    }

    template<typename Key, class Hash, class Eq>
    umultiset::const_iterator unordered_multiset<Key, Hash, Eq>::cend() const noexcept {
        return const_iterator(&_slots[_capacity]);
    }

    template<typename Key, class Hash, class Eq>
    umultiset::hasher unordered_multiset<Key, Hash, Eq>::hash_function() const {
        return _hasher;
    }

    template<typename Key, class Hash, class Eq>
    umultiset::key_equal unordered_multiset<Key, Hash, Eq>::key_eq() const {
        return _keq;
    }

    template<typename Key, class Hash, class Eq>
    umultiset::iterator unordered_multiset<Key, Hash, Eq>::insert(const_reference val) {
        return _hash_insert<unordered_multiset<Key, Hash, Eq>, iterator, key_type, const_reference>(this, val, val, val);
    }

    template<typename Key, class Hash, class Eq>
    umultiset::iterator unordered_multiset<Key, Hash, Eq>::insert(value_type &&val) {
        return _hash_insert<unordered_multiset<Key, Hash, Eq>, iterator, key_type, value_type&&>(this, val, std::forward<value_type>(val), std::forward<value_type>(val));
    }

    template<typename Key, class Hash, class Eq>
    template<class... Args>
    umultiset::iterator unordered_multiset<Key, Hash, Eq>::emplace(Args &&... args) {
        internal_ptr val = new multiset_node(std::forward<Args>(args)...);

        return _hash_insert<unordered_multiset<Key, Hash, Eq>, iterator, key_type, internal_ptr>(this, val->data, val, val);
    }

    template<typename Key, class Hash, class Eq>
    umultiset::iterator unordered_multiset<Key, Hash, Eq>::erase(const_iterator pos) {

        if ((*(pos._it._ptr))->next != nullptr) {
          _erase(pos._it._ptr, false);
          return pos._it;
        } else {
          return iterator(&(_slots[_erase(pos._it._ptr, false).first]));
        }
    }

    template<typename Key, class Hash, class Eq>
    umultiset::size_type unordered_multiset<Key, Hash, Eq>::erase(const key_type &key) {
        return _erase(find(key)._ptr, true).second;
    }

    template<typename Key, class Hash, class Eq>
    void unordered_multiset<Key, Hash, Eq>::clear() noexcept {
        _hash_clear<>(this);
        _n_slots = 0;
    }

    template<typename Key, class Hash, class Eq>
    void unordered_multiset<Key, Hash, Eq>::swap(unordered_multiset &other) {
        this->swap(*this, other);
    }

    template<typename Key, class Hash, class Eq>
    umultiset::iterator unordered_multiset<Key, Hash, Eq>::find(const key_type &key) {
        return _hash_find<>(this, key);
    }

    template<typename Key, class Hash, class Eq>
    umultiset::const_iterator unordered_multiset<Key, Hash, Eq>::find(const key_type &key) const {
        return const_cast<unordered_multiset<Key, Hash, Eq>*>(this)->find(key);
    }

    template<typename Key, class Hash, class Eq>
    umultiset::size_type unordered_multiset<Key, Hash, Eq>::count(const key_type &key) const {
        multiset_node *current;
        size_t count = 0;
        multiset_node **found_ptr = const_cast<unordered_multiset<Key, Hash, Eq>*>(this)->find(key)._ptr;

        if (found_ptr == &(_slots[_capacity])) return count;

        current = *found_ptr;
        while (current != nullptr) {
            count++;
            current = current->next;
        }

        return count;
    }

    template<typename Key, class Hash, class Eq>
    std::pair<umultiset::iterator, umultiset::iterator> unordered_multiset<Key, Hash, Eq>::equal_range(const key_type &key) {
        multiset_node **first = find(key)._ptr;
        multiset_node **second = &(_slots[first - _slots]);

        if (first == &(_slots[_capacity])) return {end(), end()};

        ++second;
        while (*second == nullptr && *second != (multiset_node *) 0x1) ++second;

        return {first, second};
    }

    template<typename Key, class Hash, class Eq>
    std::pair<umultiset::iterator, umultiset::const_iterator> unordered_multiset<Key, Hash, Eq>::equal_range(const key_type &key) const {
        return const_cast<unordered_multiset<Key, Hash, Eq>*>(this)->equal_range(key);
    }

    /* Private member functions.  */
    template<typename Key, class Hash, class Eq>
    void unordered_multiset<Key, Hash, Eq>::_rehash() {
        _hash_rehash<>(this);
    }

    template<typename Key, class Hash, class Eq>
    umultiset::hash_info unordered_multiset<Key, Hash, Eq>::_get_hash_info(const key_type &key) {
        return _hash_get_hash_info<>(this, key);
    }

    template<typename Key, class Hash, class Eq>
    void unordered_multiset<Key, Hash, Eq>::_check_load_factor(uint64_t hash, size_type &pos) {
        return _hash_check_load_factor<>(this, _n_slots, hash, pos);
    }

    template<typename Key, class Hash, class Eq>
    const umultiset::key_type &unordered_multiset<Key, Hash, Eq>::_get_slot_key(internal_ptr slot) {
        return slot->data;
    }

    template<typename Key, class Hash, class Eq>
    umultiset::find_insert_info unordered_multiset<Key, Hash, Eq>::_find_or_prepare_insert(const key_type &key, size_type pos, ctrl_t h2_hash) {
        return _hash_find_or_prepare_insert<>(this, key, pos, h2_hash);
    }

    template<typename Key, class Hash, class Eq>
    umultiset::iterator unordered_multiset<Key, Hash, Eq>::_add_to_list(const iterator &it, internal_ptr new_node) {
        multiset_node *head = *(it._ptr);

        for (; head->next != nullptr ; head = head->next);

        head->next = new_node;
        _size++;

        return {&(head->next)};
    }

    template<typename Key, class Hash, class Eq>
    umultiset::iterator unordered_multiset<Key, Hash, Eq>::_handle_elem_found(const iterator &it, const_reference val) {
        return _add_to_list(it, new multiset_node(val));
    }

    template<typename Key, class Hash, class Eq>
    umultiset::iterator unordered_multiset<Key, Hash, Eq>::_handle_elem_found(const iterator &it, value_type &&val) {
        return _add_to_list(it, new multiset_node(std::forward<value_type>(val)));
    }

    template<typename Key, class Hash, class Eq>
    umultiset::iterator unordered_multiset<Key, Hash, Eq>::_handle_elem_found(const iterator &it, internal_ptr new_node) {
        return _add_to_list(it, new_node);
    }

    template<typename Key, class Hash, class Eq>
    umultiset::iterator unordered_multiset<Key, Hash, Eq>::_handle_elem_not_found(const iterator &it) {
        _n_slots++;
        return it;
    }

    template<typename Key, class Hash, class Eq>
    umultiset::internal_ptr unordered_multiset<Key, Hash, Eq>::_construct_new_element(const_reference val) {
        return new multiset_node(val);
    }

    template<typename Key, class Hash, class Eq>
    umultiset::internal_ptr unordered_multiset<Key, Hash, Eq>::_construct_new_element(value_type &&val) {
        return new multiset_node(std::forward<value_type>(val));
    }


    template<typename Key, class Hash, class Eq>
    umultiset::internal_ptr unordered_multiset<Key, Hash, Eq>::_construct_new_element(internal_ptr val) {
        return val;
    }

    template<typename Key, class Hash, class Eq>
    umultiset::size_type unordered_multiset<Key, Hash, Eq>::_delete_all_slots(size_type pos) {
        multiset_node *to_delete;
        multiset_node *current = _slots[pos];
        size_type count = 0;

        while (current->next != nullptr) {
            to_delete = current->next;
            current->next = to_delete->next;

            delete to_delete;

            count++;
        }

        delete _slots[pos];
        _slots[pos] = nullptr;
        count++;

        _size -= count;
    
        return count;
    }

    template<typename Key, class Hash, class Eq>
    umultiset::size_type unordered_multiset<Key, Hash, Eq>::_delete_slot(size_type pos) {
        multiset_node *to_delete = _slots[pos]->next;

        if (to_delete == nullptr) {
            delete _slots[pos];
            _slots[pos] = nullptr;
        } else {
            _slots[pos]->next = to_delete->next;
            delete to_delete;
        }

        --_size;

        return 1;
    }

    template<typename Key, class Hash, class Eq>
    std::pair<umultiset::size_type, umultiset::size_type> unordered_multiset<Key, Hash, Eq>::_erase(internal_ptr *ptr, bool erase_all) {
        return _hash_erase<>(this, ptr, erase_all);
    }
}
