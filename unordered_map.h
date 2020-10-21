#pragma once

#include <cstdlib>
#include <functional>

#include "hash_utilities.h"

#define umap typename unordered_map<K, V, Hash, Eq>

/* TODO:
 * Fix compile errors, there will be a bunch.
 * Change all other STLContainers based on umap and uset.
 * Implemenent umultimap & umultiset.  */

namespace adt {

    template<typename T>
    class unordered_map_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;
        using const_reference = const value_type&;
        using const_pointer = const value_type*;

    private:
        struct enabler {};

    public:
        unordered_map_iterator(value_type **ptr = nullptr) : _ptr(ptr) {}

        /* Implicit conversion to const_iterator.  */
        template<class OtherT>
        unordered_map_iterator(const unordered_map_iterator<OtherT>& other,
                               typename std::enable_if<std::is_convertible<OtherT**, T**>::value, enabler>::type = enabler())
                               : _ptr(other._ptr) {}

        unordered_map_iterator& operator=(const unordered_map_iterator& other) = default;
        unordered_map_iterator& operator=(value_type **ptr) {
            this->_ptr = ptr;
            return *this;
        }

        bool operator==(const unordered_map_iterator& other) const {
            return this->_ptr == other._ptr;
        }
        bool operator!=(const unordered_map_iterator& other) const {
            return !(*this == other);
        }

        unordered_map_iterator& operator++() {
            ++_ptr;
            while (*_ptr == nullptr && *_ptr != (pointer) 0x1) {
                ++_ptr;
            }
            return *this;
        }
        unordered_map_iterator operator++(int) & {
            auto temp(*this);
            ++(*this);
            return temp;
        }

        const_reference operator*() const {
            return *(*this->_ptr);
        }
        const_pointer operator->() const {
            return *(this->_ptr);
        }

        reference operator*() {
            return *(*(this->_ptr));
        }
        pointer operator->() {
            return *(this->_ptr);
        }

    private:
        value_type **_ptr;

        template<typename K, typename V, typename Hash = std::hash<K>, typename Eq = std::equal_to<K>>
        friend class unordered_map;
    };

    template<typename K, typename V, typename Hash = std::hash<K>, typename Eq = std::equal_to<K>>
    class unordered_map {
    public:
        using key_type = K;
        using mapped_type = V;
        using value_type = std::pair<const K, V>;
        using hasher = Hash;
        using key_equal = Eq;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using size_type = size_t;
        using iterator = unordered_map_iterator<value_type>;
        using const_iterator = unordered_map_iterator<const value_type>;

    private:
        pointer *_slots;
        ctrl_t *_ctrls;
        hasher _hasher;
        size_type _size;
        size_type _capacity;
        size_type _first_elem_pos;
        key_equal _keq;
        struct enabler {};

        /* Constructors/Destructors.  */
        explicit unordered_map(size_type cap = 16, const hasher& hash = hasher(),
        const key_equal keq = key_equal()) noexcept;
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
        size_type erase(const value_type &val);
        void clear() noexcept;
        void swap(unordered_map &other);

        /* Element lookup.  */
        mapped_type& operator[](const key_type &key);
        mapped_type& operator[](key_type &&k);
        mapped_type& at(const key_type &key) noexcept(false);
        const mapped_type& at(const key_type &key) const noexcept(false);
        iterator find(const key_type &key);
        const_iterator find(const key_type &key) const;


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

    private:
        void _rehash();
        std::pair<iterator, bool> _insert_unique(const_pointer val, bool copy_element);
        std::pair<size_type, size_type> _erase(pointer *_ptr);
    };

    /* Implementation.  */

    /* Public member functions.  */
    template<typename K, typename V, typename Hash, typename Eq>
    unordered_map<K, V, Hash, Eq>::unordered_map(unordered_map::size_type cap, const hasher &hash,
                                                 const key_equal keq) noexcept {
        _capacity = normalize_capacity(_capacity);

        assert(is_valid_capacity(_capacity) && "capacity should always be a power of 2");

        _first_elem_pos = _capacity;
        _ctrls = new ctrl_t[_capacity];
        /* Add one extra slot so we can determine when our hash table ends.  */
        _slots = (pointer *) calloc (sizeof (key_type *), _capacity + 1);
        _slots[_capacity] = (pointer) 0x1;

        for (size_t i = 0 ; i < _capacity ; i++) {
            _ctrls[i] = ctrl_empty;
        }
    }

    template<typename K, typename V, typename Hash, typename Eq>
    unordered_map<K, V, Hash, Eq>::unordered_map(const unordered_map &other) {
        this->_capacity = other._capacity;
        this->_size = other._size;
        this->_first_elem_pos = other._first_elem_pos;
        this->_slots = (pointer *) calloc(sizeof (key_type *), this->_capacity + 1);
        this->_slots[this->_capacity] = (pointer) 0x1;
        this->_ctrls = new ctrl_t[this->_capacity];

        /* Deep copy of the data. Avoid this lol.  */
        for (size_t i = 0 ; i < other._capacity ; i++) {
            this->_ctrls[i] = other._ctrls[i];

            if (is_full_slot(other._ctrls[i])) {
                this->_slots[i] = new key_type;
                *(this->_slots[i]) = *(other._slots[i]);
            }
        }
    }

    template<typename K, typename V, typename Hash, typename Eq>
    unordered_map<K, V, Hash, Eq>::unordered_map(unordered_map &&other) noexcept : unordered_map() {
        swap(*this, other);
    }


    template<typename K, typename V, typename Hash, typename Eq>
    unordered_map<K, V, Hash, Eq> &unordered_map<K, V, Hash, Eq>::operator=(unordered_map rhs) {
        /* Copy and swap idiom, let the compiler handle the copy of the argument.  */
        swap(*this, rhs);
        return *this;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    unordered_map<K, V, Hash, Eq>::~unordered_map() noexcept {
        clear();

        if (_ctrls && _slots) {
            delete[] _ctrls;
            free(_slots);

            _slots = nullptr;
            _ctrls = nullptr;
        }
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
        return const_iterator(&_slots[_first_elem_pos]);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::const_iterator unordered_map<K, V, Hash, Eq>::cend() const noexcept {
        return const_iterator(&_slots[_capacity]);
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
    std::pair<umap::iterator, bool> unordered_map<K, V, Hash, Eq>::insert(const unordered_map::value_type &val) {
        return _insert_unique(&val, true);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    template<class P>
    std::pair<umap::iterator, bool> unordered_map<K, V, Hash, Eq>::insert(P &&val, typename std::enable_if<std::is_constructible<P, value_type>::value, enabler>::type) {
        value_type *new_val = new value_type(std::forward<value_type>(val));
        return _insert_unique(new_val, false);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    template<class... Args>
    std::pair<umap::iterator, bool> unordered_map<K, V, Hash, Eq>::emplace(Args &&... args) {
        value_type *val = new value_type(std::forward<Args>(args)...);
        return _insert_unique(val, false);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::unordered_map::iterator unordered_map<K, V, Hash, Eq>::erase(unordered_map::const_iterator pos) {
        return iterator(&(_slots[_erase(pos._ptr).first]));
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::unordered_map::size_type unordered_map<K, V, Hash, Eq>::erase(const unordered_map::value_type &val) {
        return _erase(find(val.first)._ptr).second;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    void unordered_map<K, V, Hash, Eq>::clear() noexcept {
        for (size_t i = 0 ; i < _capacity ; i++) {
            if (_slots[i] != nullptr) {
                delete _slots[i];
                _slots[i] = nullptr;
            }
        }

        _size = 0;
        _capacity = 16;
        _first_elem_pos = _capacity;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    void unordered_map<K, V, Hash, Eq>::swap(unordered_map &other) {
        this->swap(*this, other);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::mapped_type &unordered_map<K, V, Hash, Eq>::operator[](const key_type &key) {
        return (_insert_unique({key, mapped_type()}).first)->second;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::mapped_type &unordered_map<K, V, Hash, Eq>::operator[](key_type &&k) {
        return operator[](std::move(k));
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::mapped_type &unordered_map<K, V, Hash, Eq>::at(const key_type &key) noexcept(false) {
        iterator it = find(key);

        /* If we found it, return the mapped value.  */
        if (it != &_slots[_capacity]) {
            return it->second;
        }

        throw std::out_of_range("Key is not present on the map.");
    }

    template<typename K, typename V, typename Hash, typename Eq>
    const umap::mapped_type &unordered_map<K, V, Hash, Eq>::at(const key_type &key) const noexcept(false) {
        return at(key);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::iterator unordered_map<K, V, Hash, Eq>::find(const key_type &key) {
        uint64_t hash, h1_hash;
        size_t pos;
        ctrl_t h2_hash;

        if (empty()) {
            return end();
        }

        hash = _hasher(key);
        h1_hash = h1(hash, _ctrls);
        h2_hash = h2(hash);
        pos = mod(h1_hash, _capacity);

        while (1) {
            if (is_empty_slot(_ctrls[pos])) {
                return end();
            }
            if (is_full_slot(_ctrls[pos]) && _ctrls[pos] == h2_hash) {
                if (_keq(*_slots[pos], key)) {
                    return iterator(&_slots[pos]);
                }
            }
            pos = mod(pos + 1, _capacity);
        }
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::const_iterator unordered_map<K, V, Hash, Eq>::find(const key_type &key) const {
        return find(key);
    }

    /* Private member functions.  */
    template<typename K, typename V, typename Hash, typename Eq>
    void unordered_map<K, V, Hash, Eq>::_rehash() {
        uint64_t hash, h1_hash;
        size_t pos;
        auto old_slots = _slots;
        auto old_ctrls = _ctrls;
        auto old_cap = _capacity;
        auto new_cap = old_cap * 2;

        assert (is_valid_capacity (_capacity) && "capacity should always be a power of 2");

        _ctrls = new ctrl_t[new_cap];
        _slots = (pointer *) calloc (sizeof (key_type *), new_cap + 1);
        _slots[new_cap] = (pointer) 0x1;
        _capacity = new_cap;
        _first_elem_pos = new_cap;

        for (size_t i = 0 ; i < new_cap ; i++) {
            _ctrls[i] = ctrl_empty;
        }

        for (size_t i = 0 ; i < old_cap ; i++) {

            if (is_full_slot(old_ctrls[i])) {
                hash = _hasher(*(old_slots[i]));
                h1_hash = h1(hash, _ctrls);
                pos = mod(h1_hash, _capacity);

                while (1) {
                    if (is_empty_or_deleted(_ctrls[pos])) {
                        _ctrls[pos] = h2(hash);
                        _slots[pos] = old_slots[i];

                        if (pos < _first_elem_pos) {
                            _first_elem_pos = pos;
                        }

                        break;
                    }
                    pos = mod(pos + 1, _capacity);
                }
            }
        }

        delete[] old_ctrls;
        free(old_slots);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    std::pair<umap::iterator, bool> unordered_map<K, V, Hash, Eq>::_insert_unique(const unordered_map::const_pointer val, bool copy_element) {
        uint64_t hash, h1_hash;
        size_t del_pos, empty_pos, pos;
        ctrl_t h2_hash;
        double load_factor;
        bool found_deleted;
        key_type& key = val->first;

        /* Keys are unique, try and find it first.  */
        hash = _hasher(key);
        h1_hash = h1(hash, _ctrls);
        h2_hash = h2(hash);
        pos = mod(h1_hash, _capacity);

        found_deleted = false;
        while (1) {
            if (is_empty_slot(_ctrls[pos])) {
                empty_pos = pos;
                break;
            } else if (is_deleted_slot(_ctrls[pos]) && !found_deleted) {
                found_deleted = true;
                del_pos = pos;
            } else if (_ctrls[pos] == h2_hash) {
                if (_keq((*_slots[pos]).first, key)) {
                    if (!copy_element) {
                        /* If we constructed this element in emplace(), we have to delete it.  */
                        delete val;
                    }
                    return {iterator(&_slots[pos]), false};
                }
            }
            pos = mod(pos + 1, _capacity);
        }

        pos = found_deleted ? del_pos : empty_pos;
        load_factor = (double) _size / (double) _capacity;

        if (load_factor >= 0.75) {
            _rehash();
            h1_hash = h1(hash, _ctrls);
            pos = mod(h1_hash, _capacity);
        }

        while (1) {
            if (is_empty_or_deleted(_ctrls[pos])) {
                _ctrls[pos] = h2_hash;
                if (copy_element) {
                    _slots[pos] = new value_type;
                    *(_slots[pos]) = *val;
                }
                else {
                    _slots[pos] = val;
                }
                _size++;

                // Update first element position
                if (pos < _first_elem_pos) {
                    _first_elem_pos = pos;
                }

                return {iterator(&_slots[pos]), true};
            }
            pos = mod(pos + 1, _capacity);
        }
    }

    template<typename K, typename V, typename Hash, typename Eq>
    std::pair<umap::size_type, umap::size_type> unordered_map<K, V, Hash, Eq>::_erase(unordered_map::pointer *_ptr) {
        size_type pos;
        value_type *to_delete;

        if (_ptr != &_slots[_capacity]) {
            pos = _ptr - _slots;

            _ctrls[pos] = ctrl_deleted;
            to_delete = _slots[pos];
            _slots[pos] = nullptr;
            _size--;

            if (pos == _first_elem_pos) {
                while (_slots[_first_elem_pos] == nullptr && _first_elem_pos != _capacity) {
                    _first_elem_pos++;
                }
            }

            delete to_delete;

            return {pos + 1, 1};
        }

        return {_capacity, 0};
    }
}
