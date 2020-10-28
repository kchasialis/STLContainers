#pragma once

#include <cstdlib>
#include <cassert>
#include <functional>

#include "hash_utilities.h"

#define umap typename unordered_map<K, V, Hash, Eq>

/* TODO:
 * Change all other STLContainers based on umap and uset.
 * Implemenent umultimap & umultiset.
 * Maybe implement some more functions like try_emplace() etc. */

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
                               typename std::enable_if<std::is_convertible<OtherT*, T*>::value, enabler>::type = enabler())
                               : _ptr(const_cast<value_type**>(other._ptr)) {}

        unordered_map_iterator& operator=(const unordered_map_iterator& other) = default;
        unordered_map_iterator& operator=(const value_type **ptr) {
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

    public:
        value_type **_ptr;

        template<typename K, typename V, typename Hash, typename Eq>
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

        struct hash_info {
            size_t pos;
            uint64_t hash;
            ctrl_t h2_hash;
        };

        struct find_insert_info {
            std::pair<iterator, bool> find_info;
            bool found_deleted;
            size_t del_pos;
            size_t empty_pos;
        };

        template<typename U>
        struct _handle_return_overload {
            typedef U tag_type;
            value_type *ptr;

            _handle_return_overload(value_type *_ptr = nullptr) : ptr(_ptr) {}
        };

        struct tag_ignore{};
        struct tag_delete{};

        typedef _handle_return_overload<tag_ignore> to_ignore;
        typedef _handle_return_overload<tag_delete> to_delete;

    public:
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
        hash_info _get_hash_info(const key_type& key);
        find_insert_info _find_or_prepare_insert(const key_type &key, size_type pos, ctrl_t h2_hash);
        std::pair<iterator, bool> _handle_elem_found(const std::pair<iterator, bool>& retval, to_ignore obj);
        std::pair<iterator, bool> _handle_elem_found(const std::pair<iterator, bool>& retval, to_delete obj);
        void _check_load_factor(uint64_t hash, size_type& pos);
        pointer _construct_new_element(const_reference val);
        pointer _construct_new_element(pointer val);
        template<class P>
        pointer _construct_new_element(P &&val, typename std::enable_if<std::is_constructible<P, value_type>::value, enabler>::type = enabler());
        pointer _construct_new_element(const key_type &key);
        pointer _construct_new_element(key_type &&key);
        std::pair<size_type, size_type> _erase(pointer *_ptr);

        #define _umap_insert_unique_operator(key, val)                           \
            do {                                                                 \
                size_t pos;                                                      \
                /* Hash the value and return information.  */                    \
                auto info = _get_hash_info(key);                                 \
                                                                                 \
                /* Keys are unique, try and find it first.  */                   \
                auto p = _find_or_prepare_insert(key, info.pos, info.h2_hash);   \
                /* If we found it, return early.  */                             \
                if (!p.find_info.second) {                                       \
                    return p.find_info.first->second;                            \
                }                                                                \
                pos = p.found_deleted ? p.del_pos : p.empty_pos;                 \
                                                                                 \
                _check_load_factor(info.hash, pos);                              \
                                                                                 \
                while (1) {                                                      \
                    if (is_empty_or_deleted(_ctrls[pos])) {                      \
                        _ctrls[pos] = info.h2_hash;                              \
                        _slots[pos] = _construct_new_element(val);               \
                        _size++;                                                 \
                                                                                 \
                        /* Update first element position.  */                    \
                        if (pos < _first_elem_pos) {                             \
                            _first_elem_pos = pos;                               \
                        }                                                        \
                                                                                 \
                        return iterator(&_slots[pos])->second;                   \
                    }                                                            \
                    pos = mod(pos + 1, _capacity);                               \
                }                                                                \
            } while(0)
    };

    /* Implementation.  */

    /* Public member functions.  */
    template<typename K, typename V, typename Hash, typename Eq>
    unordered_map<K, V, Hash, Eq>::unordered_map(unordered_map::size_type cap, const hasher &hash,
                                                 const key_equal& keq) noexcept
                                                 : _size(0), _capacity(cap), _hasher(hash), _keq(keq) {
        _capacity = normalize_capacity(_capacity);

        assert(is_valid_capacity(_capacity) && "capacity should always be a power of 2");

        _first_elem_pos = _capacity;
        _ctrls = new ctrl_t[_capacity];
        /* Add one extra slot so we can determine when our hash table ends.  */
        _slots = (pointer *) calloc (sizeof (value_type *), _capacity + 1);
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
        this->_slots = (pointer *) calloc(sizeof (value_type *), this->_capacity + 1);
        this->_slots[this->_capacity] = (pointer) 0x1;
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
        _hash_insert_unique(val.first, val, to_ignore());
    }

    template<typename K, typename V, typename Hash, typename Eq>
    template<class P>
    std::pair<umap::iterator, bool> unordered_map<K, V, Hash, Eq>::insert(P &&val, typename std::enable_if<std::is_constructible<P, value_type>::value, enabler>::type) {
        _hash_insert_unique(val.first, std::forward<P>(val), to_ignore());
    }

    template<typename K, typename V, typename Hash, typename Eq>
    template<class... Args>
    std::pair<umap::iterator, bool> unordered_map<K, V, Hash, Eq>::emplace(Args &&... args) {
        auto *val = new value_type(std::forward<Args>(args)...);
        _hash_insert_unique(val->first, val, to_delete(val));
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::unordered_map::iterator unordered_map<K, V, Hash, Eq>::erase(const_iterator pos) {
        return iterator(&(_slots[_erase(const_cast<pointer*>(pos._ptr)).first]));
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::unordered_map::size_type unordered_map<K, V, Hash, Eq>::erase(const key_type &key) {
        return _erase(const_cast<pointer*>(find(key)._ptr)).second;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    void unordered_map<K, V, Hash, Eq>::clear() noexcept {
        for (size_t i = 0 ; i < _capacity ; i++) {
            if (_slots[i] != nullptr) {
                delete _slots[i];
                _slots[i] = nullptr;
            }
            _ctrls[i] = ctrl_empty;
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
        _umap_insert_unique_operator(key, key);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::mapped_type &unordered_map<K, V, Hash, Eq>::operator[](key_type &&key) {
        _umap_insert_unique_operator(key, std::forward<key_type>(key));
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
        size_t pos;

        if (empty()) {
            return end();
        }

        auto h_info = _get_hash_info(key);
        pos = h_info.pos;

        while (1) {
            if (is_empty_slot(_ctrls[pos])) {
                return end();
            }
            if (is_full_slot(_ctrls[pos]) && _ctrls[pos] == h_info.h2_hash) {
                if (_keq(_slots[pos]->first, key)) {
                    return iterator(&_slots[pos]);
                }
            }
            pos = mod(pos + 1, _capacity);
        }
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::const_iterator unordered_map<K, V, Hash, Eq>::find(const key_type &key) const {
        return const_cast<unordered_map<K, V, Hash, Eq>*>(this)->find(key);
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
                hash = _hasher(old_slots[i]->first);
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
    umap::hash_info unordered_map<K, V, Hash, Eq>::_get_hash_info(const key_type &key) {
        uint64_t hash;
        size_t h1_hash, pos;
        ctrl_t h2_hash;

        hash = _hasher(key);
        h1_hash = h1(hash, _ctrls);
        h2_hash = h2(hash);
        pos = mod(h1_hash, _capacity);

        return {pos, hash, h2_hash};
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::find_insert_info unordered_map<K, V, Hash, Eq>::_find_or_prepare_insert(const key_type &key, size_type pos, ctrl_t h2_hash) {
        bool found_deleted;
        size_t empty_pos, del_pos;

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
                    return {{iterator(&_slots[pos]), false}, found_deleted, del_pos, empty_pos};
                }
            }
            pos = mod(pos + 1, _capacity);
        }

        return {{iterator(), true}, found_deleted, del_pos, empty_pos};
    }

    template<typename K, typename V, typename Hash, typename Eq>
    std::pair<umap::iterator, bool> unordered_map<K, V, Hash, Eq>::_handle_elem_found(const std::pair<iterator, bool> &retval, to_ignore obj) {
        return retval;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    std::pair<umap::iterator, bool> unordered_map<K, V, Hash, Eq>::_handle_elem_found(const std::pair<iterator, bool> &retval, to_delete obj) {
        delete obj.ptr;
        return retval;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    void unordered_map<K, V, Hash, Eq>::_check_load_factor(uint64_t hash, size_type& pos) {
        double load_factor = (double) _size / (double) _capacity;
        if (load_factor >= 0.75) {
            _rehash();
            pos = mod(h1(hash, _ctrls), _capacity);
        }
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::pointer unordered_map<K, V, Hash, Eq>::_construct_new_element(const_reference val) {
        return new value_type(val);
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::pointer unordered_map<K, V, Hash, Eq>::_construct_new_element(pointer val) {
        return val;
    }

    template<typename K, typename V, typename Hash, typename Eq>
    template<class P>
    umap::pointer unordered_map<K, V, Hash, Eq>::_construct_new_element(P &&val, typename std::enable_if<std::is_constructible<P, value_type>::value, enabler>::type) {
        return new value_type(std::forward<P>(val));
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::pointer unordered_map<K, V, Hash, Eq>::_construct_new_element(const key_type &key) {
        return new value_type(key, mapped_type());
    }

    template<typename K, typename V, typename Hash, typename Eq>
    umap::pointer unordered_map<K, V, Hash, Eq>::_construct_new_element(key_type &&key) {
        return new value_type(std::forward<key_type>(key), mapped_type());
    }

    template<typename K, typename V, typename Hash, typename Eq>
    std::pair<umap::size_type, umap::size_type> unordered_map<K, V, Hash, Eq>::_erase(pointer *_ptr) {
        size_type pos, prev_pos;
        value_type *to_delete;

        if (_ptr != &_slots[_capacity]) {
            pos = _ptr - _slots;

            _ctrls[pos] = ctrl_deleted;
            to_delete = _slots[pos];
            _slots[pos] = nullptr;
            _size--;

            prev_pos = pos;
            /* Find next non-null entry.  */
            while (_slots[pos] == nullptr && pos != _capacity) {
                pos++;
            }

            /* If the deleted entry was the first element in our container, update.  */
            if (prev_pos == _first_elem_pos) {
                _first_elem_pos = pos;
            }

            delete to_delete;

            return {pos, 1};
        }

        return {_capacity, 0};
    }
}
