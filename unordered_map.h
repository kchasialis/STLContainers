#pragma once

#include <cstdlib>
#include <cassert>
#include <functional>

#include "hash_internal.h"

#define umap typename unordered_map<K, V, Hash, Eq>

using namespace hash_internal;

namespace adt {

    template<typename K, typename V, typename Hash = std::hash<K>, typename Eq = std::equal_to<K>>
    class unordered_map {
    public:
        using key_type = K;
        using mapped_type = V;
        using value_type = std::pair<const K, V>;
        using hasher = Hash;
        using key_equal = Eq;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = value_type *;
        using const_pointer = const value_type *;
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        class iterator;
        class const_iterator;

    private:
        using internal_ptr = value_type *;

        internal_ptr *_slots;
        ctrl_t *_ctrls;
        hasher _hasher;
        size_type _size;
        size_type _capacity;
        size_type _first_elem_pos;
        key_equal _keq;

        struct enabler {};

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

            handle_return_overload(value_type *_ptr = nullptr) : ptr(_ptr) {}
        };

        struct tag_ignore{};
        struct tag_delete{};

        typedef handle_return_overload<tag_ignore> to_ignore;
        typedef handle_return_overload<tag_delete> to_delete;

    public:
        class iterator {
            friend class unordered_map;

            template<class Container, typename R, typename Key, typename Value, typename... Args>
            friend R _hash_insert(Container *cnt, const Key &key, Value val, Args &&... args);

            template<class Container>
            friend container::iterator _hash_find(Container *cnt, const container::key_type &key);

            template<class Container, typename Key, typename Value>
            friend container::mapped_type &_hash_map_index_operator(Container *cnt, const Key &key, Value val);

            template<class Container>
            friend container::find_insert_info _hash_find_or_prepare_insert(Container *cnt, const container::key_type &key, container::size_type pos, ctrl_t h2_hash);

            using internal_ptr = unordered_map::internal_ptr;

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = unordered_map::value_type;
            using reference = unordered_map::reference;
            using const_reference = unordered_map::const_reference;
            using pointer = unordered_map::pointer;
            using const_pointer = unordered_map::const_pointer;
            using difference_type = unordered_map::difference_type;

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
                if (_ptr == nullptr || *_ptr == (pointer) 0x1) return *this;

                ++_ptr;
                while (*_ptr == nullptr && *_ptr != (pointer) 0x1) ++_ptr;

                return *this;
            }
            iterator operator++(int) & {
                auto temp(*this);
                ++(*this);
                return temp;
            }

            const_reference operator*() const { return *(*this->_ptr); }
            reference operator*() { return *(*(this->_ptr));
            }
            pointer operator->() {
                return *(this->_ptr);
            }
            const_pointer operator->() const {
                return *(this->_ptr);
            }

        private:
            internal_ptr *_ptr;

            iterator(internal_ptr *ptr = nullptr) : _ptr(ptr) {}
        };

        class const_iterator {
            friend class unordered_map;
            friend class iterator;
            
            using internal_ptr = unordered_map::internal_ptr;

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = unordered_map::value_type;
            using reference = unordered_map::const_reference;
            using const_reference = unordered_map::const_reference;
            using pointer = unordered_map::const_pointer;
            using difference_type = unordered_map::difference_type;

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

        private:
            iterator _it;

            const_iterator(internal_ptr *ptr) : _it(ptr) {}
        };

        /* Constructors/Destructors.  */
        explicit unordered_map(size_type cap = 16, const hasher& hash = hasher(),
                               const key_equal& keq = key_equal()) noexcept;
        unordered_map(const unordered_map &other);
        unordered_map(unordered_map &&other) noexcept;
        unordered_map& operator=(unordered_map rhs);
        ~unordered_map() noexcept;

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
        template<class P>
        std::pair<iterator, bool> insert(P &&val, typename std::enable_if<std::is_constructible<P, value_type>::value, enabler>::type = enabler());
        template <class... Args>
        std::pair<iterator, bool> emplace(Args&&... args);
        iterator erase(const_iterator pos);
        size_type erase(const key_type &key);
        void clear() noexcept;
        void swap(unordered_map &other);

        /* Element lookup.  */
        mapped_type &operator[](const key_type &key);
        mapped_type &operator[](key_type &&k);
        mapped_type &at(const key_type &key) noexcept(false);
        const mapped_type &at(const key_type &key) const noexcept(false);
        iterator find(const key_type &key);
        const_iterator find(const key_type &key) const;
        size_type count(const key_type &key) const;
        std::pair<iterator, iterator> equal_range(const key_type &key);
        std::pair<const_iterator, const_iterator> equal_range(const key_type &key) const;

        friend void swap(unordered_map& lhs, unordered_map& rhs) {
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
        friend void _hash_check_load_factor(Container *cnt, container::size_type n_slots, uint64_t hash, container::size_type &pos);

        template<class Container, typename R, typename Key, typename Value, typename... Args>
        friend R _hash_insert(Container *cnt, const Key &key, Value val, Args &&... args);

        template<class Container, typename Key, typename Value>
        friend container::mapped_type &_hash_map_index_operator(Container *cnt, const Key &key, Value val);

        template<class Container>
        friend std::pair<container::size_type, container::size_type> _hash_erase(Container *cnt, container::internal_ptr *ptr, bool erase_all);

        template<class Container>
        friend container::iterator _hash_find(Container *cnt, const container::key_type &key);

    private:
        void _rehash();
        void _check_load_factor(uint64_t hash, size_type& pos);
        hash_info _get_hash_info(const key_type &key);
        const key_type &_get_slot_key(internal_ptr slot);
        
        find_insert_info _find_or_prepare_insert(const key_type &key, size_type pos, ctrl_t h2_hash);
        std::pair<iterator, bool> _handle_elem_found(const iterator &it, to_ignore obj);
        std::pair<iterator, bool> _handle_elem_found(const iterator &it, to_delete obj);
        std::pair<iterator, bool> _handle_elem_not_found(const iterator &it);

        internal_ptr _construct_new_element(const_reference val);
        template<class P>
        internal_ptr _construct_new_element(P &&val, typename std::enable_if<std::is_constructible<P, value_type>::value, enabler>::type = enabler());
        internal_ptr _construct_new_element(internal_ptr val);
        internal_ptr _construct_new_element(const key_type &key);
        internal_ptr _construct_new_element(key_type &&key);
        
        size_type _delete_all_slots(size_type pos);
        size_type _delete_slot(size_type pos);
        std::pair<size_type, size_type> _erase(internal_ptr *ptr, bool erase_all = false);
    };

    /* Implementation.  */

    /* Public member functions.  */
    template<typename K, typename V, typename Hash, typename Eq>
    unordered_map<K, V, Hash, Eq>::unordered_map(unordered_map::size_type cap, const hasher &hash,
                                                 const key_equal& keq) noexcept
        : _size(0), _capacity(cap), _hasher(hash), _keq(keq) {
       _hash_construct<>(this);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    unordered_map<K, V, Hash, Eq>::unordered_map(const unordered_map &other) {
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
                this->_slots[i] = new value_type(other._slots[i]);
            }
        }
    }

    template<typename K, typename V, typename Hash, typename Eq>
    unordered_map<K, V, Hash, Eq>::unordered_map(unordered_map &&other) noexcept : unordered_map() {
        swap(*this, other);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    unordered_map<K, V, Hash, Eq>::~unordered_map() noexcept {
        _hash_destruct<>(this);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    unordered_map<K, V, Hash, Eq> &unordered_map<K, V, Hash, Eq>::operator=(unordered_map rhs) {
        /* Copy and swap idiom, let the compiler handle the copy of the argument.  */
        swap(*this, rhs);
        return *this;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    bool unordered_map<K, V, Hash, Eq>::empty() const noexcept {
        return _size == 0;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::size_type unordered_map<K, V, Hash, Eq>::size() const noexcept {
        return _size;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::iterator unordered_map<K, V, Hash, Eq>::begin() const noexcept {
        return iterator(&_slots[_first_elem_pos]);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::iterator unordered_map<K, V, Hash, Eq>::end() const noexcept {
        return iterator(&_slots[_capacity]);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::const_iterator unordered_map<K, V, Hash, Eq>::cbegin() const noexcept {
        return iterator(&_slots[_first_elem_pos]);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::const_iterator unordered_map<K, V, Hash, Eq>::cend() const noexcept {
        return iterator(&_slots[_capacity]);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::hasher unordered_map<K, V, Hash, Eq>::hash_function() const {
        return _hasher;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::key_equal unordered_map<K, V, Hash, Eq>::key_eq() const {
        return _keq;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    std::pair<umap::iterator, bool> unordered_map<K, V, Hash, Eq>::insert(const_reference val) {
        return _hash_insert<unordered_map<K, V, Hash, Eq>, std::pair<iterator, bool>, key_type, const_reference>(this, val.first, val, to_ignore());
    }

    template<typename K, typename V, typename Hash, typename Eq>
    template<class P>
    std::pair<umap::iterator, bool> unordered_map<K, V, Hash, Eq>::insert(P &&val, typename std::enable_if<std::is_constructible<P, value_type>::value, enabler>::type) {
        return _hash_insert<unordered_map<K, V, Hash, Eq>, std::pair<iterator, bool>, key_type, P&&>(this, val.first, std::forward<P>(val), to_ignore());
    }

    template<typename K, typename V, typename Hash, typename Eq>
    template<class... Args>
    std::pair<umap::iterator, bool> unordered_map<K, V, Hash, Eq>::emplace(Args &&... args) {
        auto *val = new value_type(std::forward<Args>(args)...);
        
        return _hash_insert<unordered_map<K, V, Hash, Eq>, std::pair<iterator, bool>, key_type, value_type*>(this, val->first, val, to_delete(val));
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::unordered_map::iterator unordered_map<K, V, Hash, Eq>::erase(const_iterator pos) {
        return iterator(&(_slots[_erase(pos._it._ptr).first]));
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::unordered_map::size_type unordered_map<K, V, Hash, Eq>::erase(const key_type &key) {
        return _erase(find(key)._ptr).second;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    void unordered_map<K, V, Hash, Eq>::clear() noexcept {
        _hash_clear<>(this);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    void unordered_map<K, V, Hash, Eq>::swap(unordered_map &other) {
        this->swap(*this, other);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::mapped_type &unordered_map<K, V, Hash, Eq>::operator[](const key_type &key) {
        return _hash_map_index_operator<unordered_map<K, V, Hash, Eq>, key_type, const key_type&>(this, key, key);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::mapped_type &unordered_map<K, V, Hash, Eq>::operator[](key_type &&key) {
        return _hash_map_index_operator<unordered_map<K, V, Hash, Eq>, key_type, key_type&&>(this, key, std::forward<key_type>(key));
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::mapped_type &unordered_map<K, V, Hash, Eq>::at(const key_type &key) noexcept(false) {
        iterator it = find(key);

        /* If we found it, return the mapped value.  */
        if (it._ptr != &_slots[_capacity]) {
            return it->second;
        }

        throw std::out_of_range("Key is not present on the map.");
    }

    template<typename K, typename V, typename Hash, typename Eq>
    const umap::mapped_type &unordered_map<K, V, Hash, Eq>::at(const key_type &key) const noexcept(false) {
        return const_cast<unordered_map<K, V, Hash, Eq>*>(this)->at(key);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::iterator unordered_map<K, V, Hash, Eq>::find(const key_type &key) {
        return _hash_find<>(this, key);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::const_iterator unordered_map<K, V, Hash, Eq>::find(const key_type &key) const {
        return const_cast<unordered_map<K, V, Hash, Eq>*>(this)->find(key);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::size_type unordered_map<K, V, Hash, Eq>::count(const key_type &key) const {
        return find(key)._it._ptr != &_slots[_capacity] ? 1 : 0;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    std::pair<umap::iterator, umap::iterator> unordered_map<K, V, Hash, Eq>::equal_range(const key_type &key) {
        auto first = find(key);
        auto second(first);

        return {first, ++second};
    }

    template<typename K, typename V, typename Hash, typename Eq>
    std::pair<umap::const_iterator, umap::const_iterator> unordered_map<K, V, Hash, Eq>::equal_range(const key_type &key) const {
        return const_cast<unordered_map<K, V, Hash, Eq>*>(this)->equal_range(key);
    }

    /* Private member functions.  */
    template<typename K, typename V, typename Hash, typename Eq>
    void unordered_map<K, V, Hash, Eq>::_rehash() {
        _hash_rehash<>(this);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::hash_info unordered_map<K, V, Hash, Eq>::_get_hash_info(const key_type &key) {
        return _hash_get_hash_info<>(this, key);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    const umap::key_type& unordered_map<K, V, Hash, Eq>::_get_slot_key(internal_ptr slot) {
        return slot->first;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::find_insert_info unordered_map<K, V, Hash, Eq>::_find_or_prepare_insert(const key_type &key, size_type pos, ctrl_t h2_hash) {
        return _hash_find_or_prepare_insert<>(this, key, pos, h2_hash);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    std::pair<umap::iterator, bool> unordered_map<K, V, Hash, Eq>::_handle_elem_found(const iterator &it, to_ignore obj) {
        return {it, false};
    }

    template<typename K, typename V, typename Hash, typename Eq>
    std::pair<umap::iterator, bool> unordered_map<K, V, Hash, Eq>::_handle_elem_found(const iterator &it, to_delete obj) {
        delete obj.ptr;
        return {it, false};
    }

    template<typename K, typename V, typename Hash, typename Eq>
    std::pair<umap::iterator, bool> unordered_map<K, V, Hash, Eq>::_handle_elem_not_found(const iterator &it) {
        return {it, true};
    }

    template<typename K, typename V, typename Hash, typename Eq>
    void unordered_map<K, V, Hash, Eq>::_check_load_factor(uint64_t hash, size_type& pos) {
        _hash_check_load_factor<>(this, _size, hash, pos);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::internal_ptr unordered_map<K, V, Hash, Eq>::_construct_new_element(const_reference val) {
        return new value_type(val);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    template<class P>
    umap::internal_ptr unordered_map<K, V, Hash, Eq>::_construct_new_element(P &&val, typename std::enable_if<std::is_constructible<P, value_type>::value, enabler>::type) {
        return new value_type(std::forward<P>(val));
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::internal_ptr unordered_map<K, V, Hash, Eq>::_construct_new_element(internal_ptr val) {
        return val;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::internal_ptr unordered_map<K, V, Hash, Eq>::_construct_new_element(const key_type &key) {
        return new value_type(key, mapped_type());
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::internal_ptr unordered_map<K, V, Hash, Eq>::_construct_new_element(key_type &&key) {
        return new value_type(std::forward<key_type>(key), mapped_type());
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::size_type unordered_map<K, V, Hash, Eq>::_delete_all_slots(size_type pos) {
        /* Keys are unique.  */
        delete _slots[pos];
        _slots[pos] = nullptr;
        --_size;

        return 1;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::size_type unordered_map<K, V, Hash, Eq>::_delete_slot(size_type pos) {
        delete _slots[pos];
        _slots[pos] = nullptr;
        --_size;

        return 1;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    std::pair<umap::size_type, umap::size_type> unordered_map<K, V, Hash, Eq>::_erase(internal_ptr *ptr, bool erase_all) {
        return _hash_erase<>(this, ptr, erase_all);
    }
}
