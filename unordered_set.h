#pragma once

#include <cstdlib>
#include <cassert>
#include <functional>

#include "hash_internal.h"

#define uset_t typename unordered_set<Key, Hash, Eq>

using namespace hash_internal;

namespace adt {

    template<typename Key, class Hash = std::hash<Key>, class Eq = std::equal_to<Key>>
    class unordered_set {
    public:
        using key_type = Key;
        using value_type = Key;
        using hasher = Hash;
        using key_equal = Eq;
        using pointer = value_type*;
        using reference = value_type&;
        using const_reference = const value_type&;
        using const_pointer = const value_type*;
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        class iterator;
        using const_iterator = iterator;

    private:
        using internal_ptr = value_type *;

        internal_ptr *_slots;
        ctrl_t *_ctrls;
        hasher _hasher;
        size_type _size;
        size_type _capacity;
        size_type _first_elem_pos;
        key_equal _keq;

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

    public:
        class iterator {
            friend class unordered_set;

            template<class Container, typename R, typename K, typename V, typename... Args>
            friend R _hash_insert(Container *cnt, const K &key, V val, Args &&... args);

            template<class Container>
            friend container::iterator _hash_find(Container *cnt, const container::key_type &key);

            template<class Container>
            friend container::find_insert_info _hash_find_or_prepare_insert(Container *cnt, const container::key_type &key, container::size_type pos, ctrl_t h2_hash);

            using internal_ptr = unordered_set::internal_ptr;

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = unordered_set::value_type;
            using reference = unordered_set::const_reference;
            using pointer = unordered_set::const_pointer;
            using difference_type = unordered_set::difference_type;

            iterator(const iterator &other) = default;
            iterator(iterator &&other) = default;

            iterator &operator=(const iterator &rhs) = default;
            iterator &operator=(internal_ptr *ptr) {
                this->_ptr = ptr;
                return *this;
            }

            bool operator==(const iterator &rhs) const { return this->_ptr == rhs._ptr; }
            bool operator==(internal_ptr *ptr) const { return this->_ptr == ptr; }
            bool operator!=(const iterator &rhs) const { return !(*this == rhs); }
            bool operator!=(internal_ptr *ptr) const { return !(*this == ptr); }

            iterator &operator++() {
                /* If its already end(), dont try to increment it.  */
                if (_ptr == nullptr || *_ptr == (internal_ptr) 0x1) {
                    return *this;
                }

                ++_ptr;
                while (*_ptr == nullptr && *_ptr != (internal_ptr) 0x1) {
                    ++_ptr;
                }
                return *this;
            }
            iterator operator++(int) & {
                auto temp(*this);
                ++(*this);
                return temp;
            }

            reference operator*() const { return *(*(this->_ptr)); }
            pointer operator->() const { return *(this->_ptr); }

        private:
            internal_ptr *_ptr;

            iterator(internal_ptr *ptr = nullptr) : _ptr(ptr) {}
        };

        /* Constructors/Destructors.  */
        explicit unordered_set(size_type cap = 16, const hasher& hash = hasher(),
                               const key_equal& keq = key_equal()) noexcept;
        unordered_set(const unordered_set &other);
        unordered_set(unordered_set &&other) noexcept;
        unordered_set& operator=(unordered_set rhs);
        ~unordered_set() noexcept;

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
        std::pair<iterator, bool> insert(const value_type &val);
        std::pair<iterator, bool> insert(value_type &&val);
        template <class... Args>
        std::pair<iterator, bool> emplace(Args&&... args);
        iterator erase(const_iterator pos);
        size_type erase(const value_type &val);
        void clear() noexcept;
        void swap(unordered_set& other);

        /* Element lookup.  */
        iterator find(const key_type &key);
        const_iterator find(const key_type &key) const;
        size_type count(const key_type &key) const;
        std::pair<iterator, iterator> equal_range(const key_type &key);
        std::pair<const_iterator, const_iterator> equal_range(const key_type &key) const;

        friend void swap(unordered_set &lhs, unordered_set &rhs) {
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

        template<class Container, typename R, typename K, typename V, typename... Args>
        friend R _hash_insert(Container *cnt, const K &key, V val, Args &&... args);

        template<class Container>
        friend std::pair<container::size_type, container::size_type> _hash_erase(Container *cnt, container::internal_ptr *ptr, bool erase_all);

        template<class Container>
        friend void _hash_clear(Container *cnt);

        template<class Container>
        friend container::iterator _hash_find(Container *cnt, const container::key_type &key);

        template<class Container>
        friend void _hash_rehash(Container *cnt);

        template<class Container>
        friend container::hash_info _hash_get_hash_info(Container *cnt, const container::key_type &key);

        template<class Container>
        friend void _hash_check_load_factor(Container *cnt, container::size_type, uint64_t hash, container::size_type &pos);

        template<class Container>
        friend container::find_insert_info _hash_find_or_prepare_insert(Container *cnt, const container::key_type &key, container::size_type pos, ctrl_t h2_hash);

    private:
        void _rehash();
        hash_info _get_hash_info(const key_type &key);
        void _check_load_factor(uint64_t hash, size_type &pos);
        const key_type &_get_slot_key(internal_ptr slot);
        
        find_insert_info _find_or_prepare_insert(const key_type &key, size_type pos, ctrl_t h2_hash); 
        std::pair<iterator, bool> _handle_elem_found(const iterator &it, to_ignore obj);
        std::pair<iterator, bool> _handle_elem_found(const iterator &it, to_delete obj);
        std::pair<iterator, bool> _handle_elem_not_found(const iterator &it);
        
        internal_ptr _construct_new_element(const_reference val);
        internal_ptr _construct_new_element(internal_ptr val);
        internal_ptr _construct_new_element(value_type &&val);
        
        size_type _delete_all_slots(size_type pos);
        size_type _delete_slot(size_type pos);
        std::pair<size_type, size_type> _erase(internal_ptr *ptr, bool erase_all = false);
    };

    /* Implementation.  */

    /* Public member functions.  */
    template<typename Key, class Hash, class Eq>
    unordered_set<Key, Hash, Eq>::unordered_set(size_type cap, const hasher &hash, const key_equal& keq) noexcept
        : _size(0), _capacity(cap), _hasher(hash), _keq(keq) {
        _hash_construct<>(this);
    }

    template<typename Key, class Hash, class Eq>
    unordered_set<Key, Hash, Eq>::unordered_set(const unordered_set &other) {
        this->_capacity = other._capacity;
        this->_size = other._size;
        this->_first_elem_pos = other._first_elem_pos;
        this->_slots = (internal_ptr *) calloc(sizeof (internal_ptr), this->_capacity + 1);
        this->_slots[this->_capacity] = (internal_ptr) 0x1;
        this->_ctrls = new ctrl_t[this->_capacity];

        /* Deep copy of the data. Avoid this lol.  */
        for (size_t i = 0 ; i < other._capacity ; i++) {
            this->_ctrls[i] = other._ctrls[i];

            if (is_full_slot(other._ctrls[i])) {
                this->_slots[i] = new key_type(*other._slots[i]);
            }
        }
    }

    template<typename Key, class Hash, class Eq>
    unordered_set<Key, Hash, Eq>::unordered_set(unordered_set &&other) noexcept : unordered_set() {
        swap(*this, other);
    }

    template<typename Key, class Hash, class Eq>
    unordered_set<Key, Hash, Eq>& unordered_set<Key, Hash, Eq>::operator=(unordered_set rhs) {
        /* Copy and swap idiom, let the compiler handle the copy of the argument.  */
        swap(*this, rhs);
        return *this;
    }

    template<typename Key, class Hash, class Eq>
    unordered_set<Key, Hash, Eq>::~unordered_set() noexcept {
        _hash_destruct<unordered_set<Key, Hash, Eq>>(this);
    }

    template<typename Key, class Hash, class Eq>
    bool unordered_set<Key, Hash, Eq>::empty() const noexcept {
        return _size == 0;
    }

    template<typename Key, class Hash, class Eq>
    uset_t::size_type unordered_set<Key, Hash, Eq>::size() const noexcept {
        return _size;
    }

    template<typename Key, class Hash, class Eq>
    uset_t::iterator unordered_set<Key, Hash, Eq>::begin() const noexcept {
        return iterator(&_slots[_first_elem_pos]);
    }

    template<typename Key, class Hash, class Eq>
    uset_t::iterator unordered_set<Key, Hash, Eq>::end() const noexcept {
        return iterator(&_slots[_capacity]);
    }

    template<typename Key, class Hash, class Eq>
    uset_t::const_iterator unordered_set<Key, Hash, Eq>::cbegin() const noexcept {
        return const_iterator(&_slots[_first_elem_pos]);
    }

    template<typename Key, class Hash, class Eq>
    uset_t::const_iterator unordered_set<Key, Hash, Eq>::cend() const noexcept {
        return const_iterator(&_slots[_capacity]);
    }

    template<typename Key, class Hash, class Eq>
    uset_t::hasher unordered_set<Key, Hash, Eq>::hash_function() const {
        return _hasher;
    }

    template<typename Key, class Hash, class Eq>
    uset_t::key_equal unordered_set<Key, Hash, Eq>::key_eq() const {
        return _keq;
    }

    template<typename Key, class Hash, class Eq>
    std::pair<uset_t::iterator, bool> unordered_set<Key, Hash, Eq>::insert(const_reference val) {
        return _hash_insert<unordered_set<Key, Hash, Eq>, std::pair<iterator, bool>, key_type, const_reference>(this, val, val, to_ignore());
    }

    template<typename Key, class Hash, class Eq>
    std::pair<uset_t::iterator, bool> unordered_set<Key, Hash, Eq>::insert(value_type &&val) {
        return _hash_insert<unordered_set<Key, Hash, Eq>, std::pair<iterator, bool>, key_type, value_type&&>(this, val, std::forward<value_type>(val), to_ignore());
    }

    template<typename Key, class Hash, class Eq>
    template<class... Args>
    std::pair<uset_t::iterator, bool> unordered_set<Key, Hash, Eq>::emplace(Args &&... args) {
        value_type *val = new value_type(std::forward<Args>(args)...);

        return _hash_insert<unordered_set<Key, Hash, Eq>, std::pair<iterator, bool>, key_type, value_type*>(this, *val, val, to_delete(val));
    }

    template<typename Key, class Hash, class Eq>
    uset_t::iterator unordered_set<Key, Hash, Eq>::erase(const_iterator pos) {
        return iterator(&(_slots[_erase(pos._it._ptr).first]));
    }

    template<typename Key, class Hash, class Eq>
    uset_t::size_type unordered_set<Key, Hash, Eq>::erase(const key_type &key) {
        return _erase(find(key)._ptr).second;
    }

    template<typename Key, class Hash, class Eq>
    void unordered_set<Key, Hash, Eq>::clear() noexcept {
        _hash_clear<unordered_set<Key, Hash, Eq>>(this);
    }

    template<typename Key, class Hash, class Eq>
    void unordered_set<Key, Hash, Eq>::swap(unordered_set &other) {
        this->swap(*this, other);
    }

    template<typename Key, class Hash, class Eq>
    uset_t::iterator unordered_set<Key, Hash, Eq>::find(const key_type &key) {
        return _hash_find<>(this, key);
    }

    template<typename Key, class Hash, class Eq>
    uset_t::const_iterator unordered_set<Key, Hash, Eq>::find(const key_type &key) const {
        return const_cast<unordered_set<Key, Hash, Eq>*>(this)->find(key);
    }

    template<typename Key, class Hash, class Eq>
    uset_t::size_type unordered_set<Key, Hash, Eq>::count(const key_type &key) const {
        return find(key)._it._ptr != &_slots[_capacity] ? 1 : 0;
    }

    template<typename Key, class Hash, class Eq>
    std::pair<uset_t::iterator, uset_t::iterator> unordered_set<Key, Hash, Eq>::equal_range(const key_type &key) {
        auto first = find(key);
        auto second(first);

        return {first, ++second};
    }

    template<typename Key, class Hash, class Eq>
    std::pair<uset_t::const_iterator, uset_t::const_iterator> unordered_set<Key, Hash, Eq>::equal_range(const key_type &key) const {
        return const_cast<unordered_set<Key, Hash, Eq>*>(this)->equal_range(key);
    }

    /* Private member functions.  */
    template<typename Key, class Hash, class Eq>
    void unordered_set<Key, Hash, Eq>::_rehash() {
        _hash_rehash<>(this);
    }

    template<typename Key, class Hash, class Eq>
    uset_t::hash_info unordered_set<Key, Hash, Eq>::_get_hash_info(const key_type &key) {
        return _hash_get_hash_info<>(this, key);
    }

    template<typename Key, class Hash, class Eq>
    uset_t::find_insert_info unordered_set<Key, Hash, Eq>::_find_or_prepare_insert(const key_type &key, size_type pos, ctrl_t h2_hash) {
        return _hash_find_or_prepare_insert<>(this, key, pos, h2_hash);
    }

    template<typename Key, class Hash, class Eq>
    std::pair<uset_t::iterator, bool> unordered_set<Key, Hash, Eq>::_handle_elem_found(const iterator &it, to_ignore obj) {
        return {it, false};
    }

    template<typename Key, class Hash, class Eq>
    std::pair<uset_t::iterator, bool> unordered_set<Key, Hash, Eq>::_handle_elem_found(const iterator &it, to_delete obj) {
        delete obj.ptr;
        return {it, false};
    }

    template<typename Key, class Hash, class Eq>
    std::pair<uset_t::iterator, bool> unordered_set<Key, Hash, Eq>::_handle_elem_not_found(const iterator &it) {
        return {it, true};
    }

    template<typename Key, class Hash, class Eq>
    void unordered_set<Key, Hash, Eq>::_check_load_factor(uint64_t hash, size_type &pos) {
        return _hash_check_load_factor<>(this, _size, hash, pos);
    }

    template<typename Key, class Hash, class Eq>
    const uset_t::key_type& unordered_set<Key, Hash, Eq>::_get_slot_key(internal_ptr slot) {
        return *slot;
    }

    template<typename Key, class Hash, class Eq>
    uset_t::internal_ptr unordered_set<Key, Hash, Eq>::_construct_new_element(const_reference val) {
        return new value_type(val);
    }

    template<typename Key, class Hash, class Eq>
    uset_t::internal_ptr unordered_set<Key, Hash, Eq>::_construct_new_element(internal_ptr val) {
        return val;
    }

    template<typename Key, class Hash, class Eq>
    uset_t::internal_ptr unordered_set<Key, Hash, Eq>::_construct_new_element(value_type &&val) {
        return new value_type(std::forward<value_type>(val));
    }

    template<typename Key, typename Hash, typename Eq>
    uset_t::size_type unordered_set<Key, Hash, Eq>::_delete_all_slots(size_type pos) {
        /* Keys are unique.  */
        delete _slots[pos];
        _slots[pos] = nullptr;
        --_size;

        return 1;
    }

    template<typename Key, class Hash, class Eq>
    uset_t::size_type unordered_set<Key, Hash, Eq>::_delete_slot(size_type pos) {
        delete _slots[pos];
        _slots[pos] = nullptr;
        --_size;

        return 1;
    }

    template<typename Key, class Hash, class Eq>
    std::pair<uset_t::size_type, uset_t::size_type> unordered_set<Key, Hash, Eq>::_erase(internal_ptr *ptr, bool erase_all) {
        return _hash_erase<>(this, ptr, erase_all);
    }
}
