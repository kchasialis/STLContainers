#pragma once

#include <cstdlib>
#include <cassert>
#include <functional>

#include "hash_internal.h"

#define umultimap typename unordered_multimap<K, V, Hash, Eq>

namespace adt {

    template<typename K, typename V, typename Hash, typename Eq>
    class unordered_multimap {
    public:
        using key_type = K;
        using mapped_type = V;
        using value_type = std::pair<const K, V>;
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

        struct multimap_node {
            value_type data;
            multimap_node *next;

            multimap_node() : data(), next(nullptr) {}
            multimap_node(const value_type &_data) : data(_data), next(nullptr) {}
            multimap_node(value_type &&_data) : data(_data), next(nullptr) {}
            multimap_node(const multimap_node &other) = default;
            multimap_node(multimap_node &&other) = default;
        };

        struct enabler {};

        using internal_ptr = multimap_node *;

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
            friend class unordered_multimap;

            template<class Container, typename R, typename Key, typename Value, typename... Args>
            friend R _hash_insert(Container *cnt, const Key &key, Value val, Args &&... args);

            template<class Container>
            friend container::iterator _hash_find(Container *cnt, const container::key_type &key);

            template<class Container>
            friend container::find_insert_info _hash_find_or_prepare_insert(Container *cnt, const container::key_type &key, container::size_type pos, ctrl_t h2_hash);

            using internal_ptr = unordered_multimap::internal_ptr;

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = unordered_multimap::value_type;
            using reference = unordered_multimap::reference;
            using const_reference = unordered_multimap::const_reference;
            using pointer = unordered_multimap::pointer;
            using const_pointer = unordered_multimap::const_pointer;
            using difference_type = unordered_multimap::difference_type;

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

            reference operator*() { return _it->data; }
            const_reference operator*() const { return _it->data; }
            pointer operator->() { return &(_it->data); }
            const_pointer operator->() const { return &(_it->data); }

        private:
            internal_ptr *_ptr;
            internal_ptr _it;

            iterator(internal_ptr *ptr = nullptr) : _ptr(ptr) {
                if (ptr != nullptr) _it = *_ptr;
            }
        };

        class const_iterator {
            friend class unordered_multimap;
            using internal_ptr = unordered_multimap::internal_ptr;

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = unordered_multimap::value_type;
            using reference = unordered_multimap::reference;
            using const_reference = unordered_multimap::const_reference;
            using pointer = unordered_multimap::pointer;
            using const_pointer = unordered_multimap::const_pointer;
            using difference_type = unordered_multimap::difference_type;

            /* Implicit conversion from iterator.  */
            const_iterator(iterator it) : _it(std::move(it)) {}

            bool operator==(const const_iterator &other) const { return this->_ptr == other._ptr; }
            bool operator==(internal_ptr *ptr) const { return _it == ptr; }
            bool operator!=(const const_iterator &other) const { return !(*this == other); }
            bool operator!=(internal_ptr *ptr) const { return !(*this == ptr); }

            reference operator*() { return *_it; }
            const_reference operator*() const { return *_it; }
            pointer operator->() { return _it.operator->(); }
            const_pointer operator->() const { return _it.operator->(); }

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
        explicit unordered_multimap(size_type cap = 16, const hasher &hash = hasher(),
                                    const key_equal &keq = key_equal()) noexcept;
        unordered_multimap(const unordered_multimap &other);
        unordered_multimap(unordered_multimap &&other) noexcept;
        unordered_multimap &operator=(unordered_multimap rhs);
        ~unordered_multimap() noexcept;

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
        template<class P>
        iterator insert(P &&val, typename std::enable_if<std::is_constructible<P, value_type>::value, enabler>::type = enabler());
        template<class... Args>
        iterator emplace(Args &&... args);
        iterator erase(const_iterator pos);
        size_type erase(const key_type &key);
        void clear() noexcept;
        void swap(unordered_multimap &other);

        /* Element lookup.  */
        iterator find(const key_type &key);
        const_iterator find(const key_type &key) const;
        size_type count(const key_type &key) const;
        std::pair<iterator, iterator> equal_range(const key_type &key);
        std::pair<iterator, const_iterator> equal_range(const key_type &key) const;

        friend void swap(unordered_multimap &lhs, unordered_multimap &rhs) {
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

        template<class Container, typename R, typename Key, typename Value, typename... Args>
        friend R _hash_insert(Container *cnt, const Key &key, Value val, Args &&... args);

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
        template<typename P>
        iterator _handle_elem_found(const iterator &it, P &&val, typename std::enable_if<std::is_constructible<P, value_type>::value, enabler>::type = enabler());
        iterator _handle_elem_found(const iterator &it, internal_ptr new_node);
        iterator _handle_elem_not_found(const iterator &it);

        iterator _add_to_list(const iterator &it, internal_ptr new_node);
        internal_ptr _construct_new_element(const_reference val);
        template<typename P>
        internal_ptr _construct_new_element(P &&val, typename std::enable_if<std::is_constructible<P, value_type>::value, enabler>::type = enabler());
        internal_ptr _construct_new_element(internal_ptr val);

        size_type _delete_all_slots(size_type pos);
        size_type _delete_slot(size_type pos);
        std::pair<size_type, size_type> _erase(internal_ptr *ptr, bool erase_all);
    };

    /* Implementation.  */

    /* Public member functions.  */
    template<typename K, typename V, typename Hash, typename Eq>
    unordered_multimap<K, V, Hash, Eq>::unordered_multimap(size_type cap, const hasher &hash, const key_equal &keq) noexcept
            : _size(0), _capacity(cap), _hasher(hash), _keq(keq) {
        _hash_construct<>(this);
        _n_slots = 0;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    unordered_multimap<K, V, Hash, Eq>::unordered_multimap(const unordered_multimap &other) {
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
                this_current = new multimap_node(other._slots[i]->data);
                this->_slots[i] = this_current;

                other_current = other._slots[i]->next;
                while (other_current != nullptr) {
                    this_current->next = new multimap_node(other_current->data);
                    other_current = other_current->next;
                    this_current = this_current->next;
                }
            }
        }
    }

    template<typename K, typename V, typename Hash, typename Eq>
    unordered_multimap<K, V, Hash, Eq>::unordered_multimap(unordered_multimap &&other) noexcept : unordered_multimap() {
        swap(*this, other);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    unordered_multimap<K, V, Hash, Eq> &unordered_multimap<K, V, Hash, Eq>::operator=(unordered_multimap rhs) {
        /* Copy and swap idiom, let the compiler handle the copy of the argument.  */
        swap(*this, rhs);
        return *this;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    unordered_multimap<K, V, Hash, Eq>::~unordered_multimap() noexcept {
        _hash_destruct<>(this);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    bool unordered_multimap<K, V, Hash, Eq>::empty() const noexcept {
        return _size == 0;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umultimap::size_type unordered_multimap<K, V, Hash, Eq>::size() const noexcept {
        return _size;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umultimap::iterator unordered_multimap<K, V, Hash, Eq>::begin() const noexcept {
        return iterator(&_slots[_first_elem_pos]);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umultimap::iterator unordered_multimap<K, V, Hash, Eq>::end() const noexcept {
        return iterator(&_slots[_capacity]);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umultimap::const_iterator unordered_multimap<K, V, Hash, Eq>::cbegin() const noexcept {
        return const_iterator(&_slots[_first_elem_pos]);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umultimap::const_iterator unordered_multimap<K, V, Hash, Eq>::cend() const noexcept {
        return const_iterator(&_slots[_capacity]);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umultimap::hasher unordered_multimap<K, V, Hash, Eq>::hash_function() const {
        return _hasher;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umultimap::key_equal unordered_multimap<K, V, Hash, Eq>::key_eq() const {
        return _keq;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umultimap::iterator unordered_multimap<K, V, Hash, Eq>::insert(const_reference val) {
        return _hash_insert<unordered_multimap<K, V, Hash, Eq>, iterator, key_type, const_reference>(this, val, val, val);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    template<typename P>
    umultimap::iterator unordered_multimap<K, V, Hash, Eq>::insert(P &&val, typename std::enable_if<std::is_constructible<P, value_type>::value, enabler>::type) {
        return _hash_insert<unordered_multimap<K, V, Hash, Eq>, iterator, key_type, P&&>(this, val.first, std::forward<P>(val), std::forward<P>(val));
    }

    template<typename K, typename V, typename Hash, typename Eq>
    template<class... Args>
    umultimap::iterator unordered_multimap<K, V, Hash, Eq>::emplace(Args &&... args) {
        internal_ptr val = new multimap_node(std::forward<Args>(args)...);

        return _hash_insert<unordered_multimap<K, V, Hash, Eq>, iterator, key_type, internal_ptr>(this, val->data, val, val);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umultimap::iterator unordered_multimap<K, V, Hash, Eq>::erase(const_iterator pos) {

        if ((*(pos._it._ptr))->next != nullptr) {
            _erase(pos._it._ptr, false);
            return pos._it;
        } else {
            return iterator(&(_slots[_erase(pos._it._ptr, false).first]));
        }
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umultimap::size_type unordered_multimap<K, V, Hash, Eq>::erase(const key_type &key) {
        return _erase(find(key)._ptr, true).second;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    void unordered_multimap<K, V, Hash, Eq>::clear() noexcept {
        _hash_clear<>(this);
        _n_slots = 0;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    void unordered_multimap<K, V, Hash, Eq>::swap(unordered_multimap &other) {
        this->swap(*this, other);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umultimap::iterator unordered_multimap<K, V, Hash, Eq>::find(const key_type &key) {
        return _hash_find<>(this, key);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umultimap::const_iterator unordered_multimap<K, V, Hash, Eq>::find(const key_type &key) const {
        return const_cast<unordered_multimap<K, V, Hash, Eq>*>(this)->find(key);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umultimap::size_type unordered_multimap<K, V, Hash, Eq>::count(const key_type &key) const {
        multimap_node *current;
        size_t count = 0;
        multimap_node **found_ptr = const_cast<unordered_multimap<K, V, Hash, Eq>*>(this)->find(key)._ptr;

        if (found_ptr == &(_slots[_capacity])) return count;

        current = *found_ptr;
        while (current != nullptr) {
            count++;
            current = current->next;
        }

        return count;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    std::pair<umultimap::iterator, umultimap::iterator> unordered_multimap<K, V, Hash, Eq>::equal_range(const key_type &key) {
        multimap_node **first = find(key)._ptr;
        multimap_node **second = &(_slots[first - _slots]);

        if (first == &(_slots[_capacity])) return {end(), end()};

        ++second;
        while (*second == nullptr && *second != (multimap_node *) 0x1) ++second;

        return {first, second};
    }

    template<typename K, typename V, typename Hash, typename Eq>
    std::pair<umultimap::iterator, umultimap::const_iterator> unordered_multimap<K, V, Hash, Eq>::equal_range(const key_type &key) const {
        return const_cast<unordered_multimap<K, V, Hash, Eq>*>(this)->equal_range(key);
    }

    /* Private member functions.  */
    template<typename K, typename V, typename Hash, typename Eq>
    void unordered_multimap<K, V, Hash, Eq>::_rehash() {
        _hash_rehash<>(this);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umultimap::hash_info unordered_multimap<K, V, Hash, Eq>::_get_hash_info(const key_type &key) {
        return _hash_get_hash_info<>(this, key);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    void unordered_multimap<K, V, Hash, Eq>::_check_load_factor(uint64_t hash, size_type &pos) {
        return _hash_check_load_factor<>(this, _n_slots, hash, pos);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    const umultimap::key_type &unordered_multimap<K, V, Hash, Eq>::_get_slot_key(internal_ptr slot) {
        return slot->data;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umultimap::find_insert_info unordered_multimap<K, V, Hash, Eq>::_find_or_prepare_insert(const key_type &key, size_type pos, ctrl_t h2_hash) {
        return _hash_find_or_prepare_insert<>(this, key, pos, h2_hash);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umultimap::iterator unordered_multimap<K, V, Hash, Eq>::_add_to_list(const iterator &it, internal_ptr new_node) {
        multimap_node *head = *(it._ptr);

        for (; head->next != nullptr ; head = head->next);

        head->next = new_node;
        _size++;

        return {&(head->next)};
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umultimap::iterator unordered_multimap<K, V, Hash, Eq>::_handle_elem_found(const iterator &it, const_reference val) {
        return _add_to_list(it, new multimap_node(val));
    }

    template<typename K, typename V, typename Hash, typename Eq>
    template<typename P>
    umultimap::iterator unordered_multimap<K, V, Hash, Eq>::_handle_elem_found(const iterator &it, P &&val, typename std::enable_if<std::is_constructible<P, value_type>::value, enabler>::type) {
        return _add_to_list(it, new multimap_node(std::forward<value_type>(val)));
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umultimap::iterator unordered_multimap<K, V, Hash, Eq>::_handle_elem_found(const iterator &it, internal_ptr new_node) {
        return _add_to_list(it, new_node);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umultimap::iterator unordered_multimap<K, V, Hash, Eq>::_handle_elem_not_found(const iterator &it) {
        _n_slots++;
        return it;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umultimap::internal_ptr unordered_multimap<K, V, Hash, Eq>::_construct_new_element(const_reference val) {
        return new multimap_node(val);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    template<typename P>
    umultimap::internal_ptr unordered_multimap<K, V, Hash, Eq>::_construct_new_element(P &&val, typename std::enable_if<std::is_constructible<P, value_type>::value, enabler>::type) {
        return new multimap_node(std::forward<value_type>(val));
    }


    template<typename K, typename V, typename Hash, typename Eq>
    umultimap::internal_ptr unordered_multimap<K, V, Hash, Eq>::_construct_new_element(internal_ptr val) {
        return val;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umultimap::size_type unordered_multimap<K, V, Hash, Eq>::_delete_all_slots(size_type pos) {
        multimap_node *to_delete;
        multimap_node *current = _slots[pos];
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

    template<typename K, typename V, typename Hash, typename Eq>
    umultimap::size_type unordered_multimap<K, V, Hash, Eq>::_delete_slot(size_type pos) {
        multimap_node *to_delete = _slots[pos]->next;

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

    template<typename K, typename V, typename Hash, typename Eq>
    std::pair<umultimap::size_type, umultimap::size_type> unordered_multimap<K, V, Hash, Eq>::_erase(internal_ptr *ptr, bool erase_all) {
        return _hash_erase<>(this, ptr, erase_all);
    }
}
