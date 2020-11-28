#pragma once

#include <iostream>
#include <cassert>
#include <cstdlib>

#define vector_t typename vector<T>

namespace adt {

    template<typename T>
    class vector {
    public:
        using value_type = T;
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
        using internal_ptr = pointer;

        pointer _data;
        size_t _size;
        size_t _capacity;

    public:
        class iterator {
            friend class vector;
            friend class const_iterator;
            friend class reverse_iterator;
            friend class const_reverse_iterator;
            using internal_ptr = vector::internal_ptr;

        public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type = vector::value_type;
            using reference = vector::reference;
            using const_reference = vector::const_reference;
            using pointer = vector::pointer;
            using const_pointer = vector::const_pointer;
            using difference_type = vector::difference_type;

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
            bool operator<(const iterator &rhs) const { return this->_ptr < rhs._ptr; }
            bool operator<=(const iterator &rhs) const { return this->_ptr <= rhs._ptr; }
            bool operator>(const iterator &rhs) const { return this->_ptr > rhs._ptr;}
            bool operator>=(const iterator &rhs) const { return this->_ptr >= rhs._ptr; }

            iterator &operator+=(difference_type val) {
                _ptr += val;
                return *this;
            }
            iterator &operator-=(difference_type val) {
                _ptr -= val;
                return *this;
            }

            iterator &operator++() {
                ++_ptr;
                return *this;
            }
            iterator operator++(int) & {
                auto temp(*this);
                ++(*this);
                return temp;
            }
            iterator &operator--() {
                --_ptr;
                return *this;
            }
            iterator operator--(int) & {
                auto temp(*this);
                --(*this);
                return temp;
            }
            iterator operator+(const std::ptrdiff_t &val) & {
                internal_ptr tmp_ptr = _ptr;
                _ptr += val;
                auto temp(*this);
                _ptr = tmp_ptr;
                return temp;
            }
            iterator operator-(const std::ptrdiff_t &val) & {
                internal_ptr tmp_ptr = _ptr;
                _ptr -= val;
                auto temp(*this);
                _ptr = tmp_ptr;
                return temp;
            }
            difference_type operator-(const iterator &other) { return std::distance(other._ptr, this->_ptr); }


            reference operator[](size_type n) { return _ptr[n]; }
            const_reference operator[](size_type n) const { return _ptr[n]; }
            reference operator*() { return *_ptr; }
            const_reference operator*() const { return *_ptr; }
            pointer operator->() { return _ptr; }
            const_pointer operator->() const { return _ptr; }

        private:
            internal_ptr _ptr;

            iterator(internal_ptr ptr = nullptr) : _ptr(ptr) {}
        };

        class const_iterator {
            friend class vector;
            using internal_ptr = vector::internal_ptr;

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = vector::value_type;
            using reference = vector::const_reference;
            using pointer = vector::const_pointer;
            using difference_type = vector::difference_type;

            /* Implicit conversion from iterator.  */
            const_iterator(iterator it) : _it(std::move(it)) {}

            const_iterator &operator=(const const_iterator &rhs) = default;
            const_iterator &operator=(internal_ptr ptr) {
                this->_it = ptr;
                return *this;
            }

            bool operator==(const const_iterator &other) const { return this->_it == other._it; }
            bool operator==(internal_ptr ptr) const { return _it == ptr; }
            bool operator!=(const const_iterator &other) const { return !(*this == other); }
            bool operator!=(internal_ptr ptr) const { return !(*this == ptr); }
            bool operator<(const const_iterator &rhs) const { return this->_it < rhs._it; }
            bool operator<=(const const_iterator &rhs) const { return this->_it <= rhs._it; }
            bool operator>(const const_iterator &rhs) const { return this->_it > rhs._it;}
            bool operator>=(const const_iterator &rhs) const { return this->_it >= rhs._it; }

            const_iterator &operator+=(difference_type val) {
                _it += val;
                return *this;
            }
            const_iterator &operator-=(difference_type val) {
                _it -= val;
                return *this;
            }
            const_iterator &operator++() {
                ++_it;
                return *this;
            }
            const_iterator operator++(int) & { return _it++; }
            const_iterator &operator--() {
                --_it;
                return *this;
            }
            const_iterator operator--(int) & { return _it--; }

            const_iterator operator+(const std::ptrdiff_t &val) & { return _it + val; }
            const_iterator operator-(const std::ptrdiff_t &val) & { return _it - val; }
            difference_type operator-(const const_iterator &other) { return this->_it - other._it; }

            reference operator[](size_type n) { return _it[n]; }
            reference operator*() const { return *_it; }
            pointer operator->() const { return _it.operator->(); }

        private:
            iterator _it;

            const_iterator(internal_ptr ptr = nullptr) : _it(ptr) {}
        };

        class reverse_iterator {
            friend class vector;
            using internal_ptr = vector::internal_ptr;

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = vector::value_type;
            using reference = vector::reference;
            using const_reference = vector::const_reference;
            using pointer = vector::pointer;
            using const_pointer = vector::const_pointer;
            using difference_type = vector::difference_type;

            /* Implicit conversion from iterator.  */
            reverse_iterator(iterator it) : _it(std::move(it)) {}

            reverse_iterator &operator=(const reverse_iterator &rhs) = default;
            reverse_iterator &operator=(internal_ptr ptr) {
                this->_it = ptr;
                return *this;
            }

            bool operator==(const reverse_iterator &other) const { return this->_it == other._it; }
            bool operator!=(const reverse_iterator &other) const { return !(*this == other); }
            bool operator<(const reverse_iterator &rhs) const { return this->_it < rhs._it; }
            bool operator<=(const reverse_iterator &rhs) const { return this->_it <= rhs._it; }
            bool operator>(const reverse_iterator &rhs) const { return this->_it > rhs._it;}
            bool operator>=(const reverse_iterator &rhs) const { return this->_it >= rhs._it; }

            reverse_iterator &operator+=(difference_type val) {
                _it -= val;
                return *this;
            }
            reverse_iterator &operator-=(difference_type val) {
                _it += val;
                return *this;
            }
            reverse_iterator &operator++() {
                --_it;
                return *this;
            }
            reverse_iterator operator++(int) & { return _it--; }
            reverse_iterator &operator--() {
                ++_it;
                return *this;
            }
            reverse_iterator operator--(int) & { return _it++; }
            reverse_iterator operator+(const std::ptrdiff_t &val) & { return _it - val; }
            reverse_iterator operator-(const std::ptrdiff_t &val) & { return _it + val; }
            difference_type operator-(const reverse_iterator &other) { return _it + other._it; }

            reference operator[](size_type n) { return _it[n]; }
            const_reference operator[](size_type n) const { return _it[n]; }
            reference operator*() { return *_it; }
            const_reference operator*() const { return *_it; }
            pointer operator->() { return _it.operator->(); }
            const_pointer operator->() const { return _it.operator->(); }

        private:
            iterator _it;

            reverse_iterator(internal_ptr ptr = nullptr) : _it(ptr) {}
        };

        class const_reverse_iterator {
            friend class vector;
            using internal_ptr = vector::internal_ptr;

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = vector::value_type;
            using reference = vector::const_reference;
            using pointer = vector::const_pointer;
            using difference_type = vector::difference_type;

            /* Implicit conversion from iterator.  */
            const_reverse_iterator(reverse_iterator it) : _it(std::move(it)) {}

            const_reverse_iterator &operator=(const const_reverse_iterator &rhs) = default;
            const_reverse_iterator &operator=(internal_ptr ptr) {
                this->_it = ptr;
                return *this;
            }

            bool operator==(const const_reverse_iterator &other) const { return this->_it == other._it; }
            bool operator!=(const const_reverse_iterator &other) const { return !(*this == other); }
            bool operator<(const const_reverse_iterator &rhs) const { return this->_it < rhs._it; }
            bool operator<=(const const_reverse_iterator &rhs) const { return this->_it <= rhs._it; }
            bool operator>(const const_reverse_iterator &rhs) const { return this->_it > rhs._it;}
            bool operator>=(const const_reverse_iterator &rhs) const { return this->_it >= rhs._it; }

            const_reverse_iterator &operator+=(difference_type val) {
                _it += val;
                return *this;
            }
            const_reverse_iterator &operator-=(difference_type val) {
                _it -= val;
                return *this;
            }
            const_reverse_iterator &operator++() {
                ++_it;
                return *this;
            }
            const_reverse_iterator operator++(int) & { return _it++; }
            const_reverse_iterator &operator--() {
                --_it;
                return *this;
            }
            const_reverse_iterator operator--(int) & { return _it--; }
            const_reverse_iterator operator+(const std::ptrdiff_t &val) & { return _it + val; }
            const_reverse_iterator operator-(const std::ptrdiff_t &val) & { return _it - val; }
            difference_type operator-(const const_reverse_iterator &other) { return _it._ptr - other._ptr; }

            reference operator[](size_type n) { return _it[n]; }
            reference operator*() const { return *_it; }
            pointer operator->() const { return _it.operator->(); }

        private:
            reverse_iterator _it;

            const_reverse_iterator(internal_ptr ptr = nullptr) : _it(ptr) {}
        };

        /* Constructors/Destructors.  */
        vector();
        explicit vector(size_t n);
        vector(size_type n, const value_type &val);
        vector(size_type n, value_type &&val);
        vector(const vector& other);
        vector(vector&& other) noexcept;
        ~vector();
        vector& operator=(vector other);

        /* Iterators.  */
        iterator begin() noexcept;
        const_iterator begin() const noexcept;
        iterator end() noexcept;
        const_iterator end() const noexcept;
        reverse_iterator rbegin() noexcept;
        const_reverse_iterator rbegin() const noexcept;
        reverse_iterator rend() noexcept;
        const_reverse_iterator rend() const noexcept;
        const_iterator cbegin() const noexcept;
        const_iterator cend() const noexcept;
        const_reverse_iterator crbegin() const noexcept;
        const_reverse_iterator crend() const noexcept;

        /* Capacity.  */
        size_type size() const noexcept;
        size_type capacity() const noexcept;
        void resize(size_type n);
        void resize(size_type n, const value_type &val);
        void reserve(size_type n);
        bool empty() const noexcept;

        /* Element access.  */
        reference operator[](size_type n);
        const_reference operator[](size_type n) const;
        reference at(size_type n) noexcept(false);
        const_reference at(size_type n) const noexcept(false);
        reference front();
        const_reference front() const;
        reference back();
        const_reference back() const;
        pointer data() noexcept;
        const_pointer data() const noexcept;

        /* Modifiers.  */
        template<class InputIterator>
        void assign(InputIterator first, InputIterator last);
        void assign(size_type n, const value_type &val);
        void assign(std::initializer_list<value_type> il);
        void push_back(const value_type &val);
        void push_back(value_type &&val);
        iterator insert(const_iterator pos, const value_type &val);
        iterator insert(const_iterator pos, size_type n, const value_type &val);
        iterator insert(const_iterator pos, value_type &&val);
        iterator insert(const_iterator pos, std::initializer_list<value_type> il);
        template<typename... Args>
        iterator emplace(const_iterator pos, Args&&... args);
        template<typename... Args>
        void emplace_back(Args&&... args);
        iterator erase(const_iterator pos);
        iterator erase(const_iterator first, const_iterator last);
        void pop_back();
        void swap(vector &x);
        void clear() noexcept;

        friend void swap(vector &lhs, vector &rhs) {
            using std::swap;

            swap(lhs._data, rhs._data);
            swap(lhs._size, rhs._size);
            swap(lhs._capacity, rhs._capacity);
        }

    private:
        template<typename... Args>
        void _construct_vector(size_type n, Args&&... args);
        template<typename... Args>
        void _resize(size_type n, Args&&... args);
        template<typename... Args>
        void _push_back(Args&&... args);
        template<typename... Args>
        iterator _insert(const const_iterator &pos, Args&&... args);
    };

    template<typename T>
    vector<T>::vector() : _data(nullptr), _size(0), _capacity(0) {}

    template<typename T>
    vector<T>::vector(size_type n) {
        _construct_vector(n);
    }

    template<typename T>
    vector<T>::vector(size_type n, const value_type &val) {
        _construct_vector(n);
    }

    template<typename T>
    vector<T>::vector(size_type n, value_type &&val) {
        _construct_vector(n);
    }

    template<typename T>
    vector<T>::vector(const vector &other) : _size(other._size), _capacity(other._capacity) {
        _data = (pointer) malloc(_capacity * sizeof(value_type));
        for (size_t i = 0 ; i < _size ; i++) {
            new (&(_data[i])) value_type(other[i]);
        }
    }

    template<typename T>
    vector<T>::vector(vector &&other) noexcept : vector() {
        swap(*this, other);
    }

    template<typename T>
    vector<T>::~vector() {
        for (size_t i = 0 ; i < _size ; i++) {
            _data[i].~value_type();
        }
        if (_data) {
            free(_data);
        }
    }

    template<typename T>
    vector<T> &vector<T>::operator=(vector other) {
        /*Copy and swap idiom, let the compiler handle the copy of the argument*/
        swap(*this, other);

        return *this;
    }

    template<typename T>
    vector_t::iterator vector<T>::begin() noexcept {
        return iterator(&(_data[0]));
    }

    template<typename T>
    vector_t::const_iterator vector<T>::begin() const noexcept {
        return const_cast<vector<T>*>(this)->begin();
    }

    template<typename T>
    vector_t::iterator vector<T>::end() noexcept {
        return iterator(&(_data[_size]));
    }

    template<typename T>
    vector_t::const_iterator vector<T>::end() const noexcept {
        return const_cast<vector<T>*>(this)->end();
    }

    template<typename T>
    vector_t::reverse_iterator vector<T>::rbegin() noexcept {
        return reverse_iterator(&(_data[_size - 1]));
    }

    template<typename T>
    vector_t::const_reverse_iterator vector<T>::rbegin() const noexcept {
        return const_cast<vector<T>*>(this)->rbegin();
    }

    template<typename T>
    vector_t::reverse_iterator vector<T>::rend() noexcept {
        return reverse_iterator(&(_data[-1]));
    }

    template<typename T>
    vector_t::const_reverse_iterator vector<T>::rend() const noexcept {
        return const_cast<vector<T>*>(this)->rend();
    }

    template<typename T>
    vector_t::const_iterator vector<T>::cbegin() const noexcept {
        return const_cast<vector<T>*>(this)->begin();
    }

    template<typename T>
    vector_t::const_iterator vector<T>::cend() const noexcept {
        return const_cast<vector<T>*>(this)->end();
    }

    template<typename T>
    vector_t::const_reverse_iterator vector<T>::crbegin() const noexcept {
        return const_cast<vector<T>*>(this)->rbegin();
    }

    template<typename T>
    vector_t::const_reverse_iterator vector<T>::crend() const noexcept {
        return const_cast<vector<T>*>(this)->rend();
    }

    template<typename T>
    vector_t::size_type vector<T>::size() const noexcept {
        return _size;
    }

    template<typename T>
    vector_t::size_type vector<T>::capacity() const noexcept {
        return _capacity;
    }

    template<typename T>
    void vector<T>::resize(size_type n) noexcept(false) {
        _resize(n);
    }

    template<typename T>
    void vector<T>::resize(size_type n, const value_type &val) noexcept(false) {
        _resize(n, val);
    }

    template<typename T>
    void vector<T>::reserve(size_type n) noexcept(false) {
        if (n > _capacity) {
            pointer new_data = (pointer) realloc(_data, n * sizeof(value_type));
            if (new_data != nullptr) {
                _data = new_data;
                _capacity = n;
            } else {
                throw std::bad_alloc();
            }
        }
    }

    template<typename T>
    bool vector<T>::empty() const noexcept {
        return _size == 0;
    }

    template<typename T>
    vector_t::reference vector<T>::operator[](size_type n) {
        return _data[n];
    }

    template<typename T>
    vector_t::const_reference vector<T>::operator[](size_type n) const {
        return const_cast<vector<T>*>(this)->operator[](n);
    }

    template<typename T>
    vector_t::reference vector<T>::at(size_type n) noexcept(false) {
        if (n < _size) return _data[n];

        throw std::out_of_range("Index is out of vector range");
    }

    template<typename T>
    vector_t::const_reference vector<T>::at(size_type n) const noexcept(false) {
        return const_cast<vector<T>*>(this)->at(n);
    }

    template<typename T>
    vector_t::reference vector<T>::front() {
        return _data[0];
    }

    template<typename T>
    vector_t::const_reference vector<T>::front() const {
        return const_cast<vector<T>*>(this)->front();
    }

    template<typename T>
    vector_t::reference vector<T>::back() {
        return _data[_size - 1];
    }

    template<typename T>
    vector_t::const_reference vector<T>::back() const {
        return const_cast<vector<T>*>(this)->back();
    }

    template<typename T>
    vector_t::pointer vector<T>::data() noexcept {
        return _data;
    }

    template<typename T>
    vector_t::const_pointer vector<T>::data() const noexcept {
        return _data;
    }

    template<typename T>
    template<class InputIterator>
    void vector<T>::assign(InputIterator first, InputIterator last) {
        auto vec_it = begin();

        for (auto it = first ; it != last ; it++) _insert(vec_it++, *it);
    }

    template<typename T>
    void vector<T>::assign(size_type n, const value_type &val) {
        auto it = begin();

        while (n-- > 0) _insert(it++, val);
    }

    template<typename T>
    void vector<T>::assign(std::initializer_list<value_type> il) {
        auto vec_it = begin();

        for (auto it = il.begin() ; it != il.end() ; it++) _insert(vec_it++, *it);
    }

    template<typename T>
    void vector<T>::push_back(const value_type &val) {
        _push_back(val);
    }

    template<typename T>
    void vector<T>::push_back(value_type &&val) {
        _push_back(std::forward<value_type>(val));
    }

    template<typename T>
    vector_t::iterator vector<T>::insert(const_iterator pos, const value_type &val) {
        return _insert(pos, val);
    }

    template<typename T>
    vector_t::iterator vector<T>::insert(const_iterator pos, size_type n, const value_type &val) {
        iterator ret_it;

        if (n > 0) {
            ret_it = _insert(pos, val);
            --n;

            while (n-- > 0) _insert(pos, val);
        } else {
            ret_it = pos;
        }

        return ret_it;
    }

    template<typename T>
    vector_t::iterator vector<T>::insert(const_iterator pos, value_type &&val) {
        return _insert(pos, std::forward<value_type>(val));
    }

    template<typename T>
    vector_t::iterator vector<T>::insert(const_iterator pos, std::initializer_list<value_type> il) {
        iterator ret_it;

        if (il.begin() != il.end()) {
            auto it = il.begin();
            ret_it = _insert(pos, *it);

            ++it;
            for (; it != il.end(); ++it) _insert(pos, *it);
        } else {
            ret_it = pos;
        }

        return ret_it;
    }

    template<typename T>
    template<typename... Args>
    vector_t::iterator vector<T>::emplace(const_iterator pos, Args &&... args) {
        return _insert(pos, std::forward<Args>(args)...);
    }

    template<typename T>
    template<typename... Args>
    void vector<T>::emplace_back(Args &&... args) {
        _push_back(std::forward<Args>(args)...);
    }

    template<typename T>
    vector_t::iterator vector<T>::erase(const_iterator pos) {
        if (pos != end()) {
            std::ptrdiff_t shift_idx = pos - begin();
            _data[shift_idx].~value_type();
            for (size_type i = shift_idx ; i < _size - 1; i++) _data[i] = std::move(_data[i + 1]);
            _size--;

            return iterator(&(_data[shift_idx]));
        }
        else {
            return end();
        }
    }

    template<typename T>
    vector_t::iterator vector<T>::erase(const_iterator first, const_iterator last) {
        while (first != last) {
            erase(first);
            last--;
        }

        /* TODO: check this.  */
        return last._it;
    }

    template<typename T>
    void vector<T>::pop_back() {
        _data[_size - 1].~value_type();
        _size--;
    }

    template<typename T>
    void vector<T>::swap(vector &x) {
        swap(*this, x);
    }

    template<typename T>
    void vector<T>::clear() noexcept {
        for (size_t i = 0 ; i < _size ; i++) _data[i].~value_type();
        _size = 0;
        _capacity = 0;
    }

    /* Private member functions.  */
    template<typename T>
    template<typename... Args>
    void vector<T>::_construct_vector(size_type n, Args &&... args) {
        assert(sizeof...(Args) <= 1);

        _data = (pointer) calloc(n, sizeof(value_type));
        if (_data != nullptr) {
            for (size_t i = 0 ; i < n ; i++) new (&(_data[i])) value_type(std::forward<Args>(args)...);
        } else {
            throw std::bad_alloc();
        }
        _size = n;
        _capacity = n;
    }

    template<typename T>
    template<typename... Args>
    void vector<T>::_resize(size_type n, Args &&... args) noexcept(false) {

        assert(sizeof...(Args) <= 1);

        if (n < _size) {
            for (size_t i = n ; i < _size ; i++) _data[i].~value_type();
            _size = n;
        }
        else if (n > _size && n < _capacity) {
            for (size_t i = _size ; i < n ; i++) new (&(_data[i])) value_type(std::forward<Args>(args)...);
            _size = n;
        }
        else if (n > _size && n > _capacity) {
            pointer new_data = (pointer) realloc(_data, n * sizeof(value_type));
            if (new_data != nullptr) {
                _data = new_data;
                for (size_t i = _size ; i < n ; i++) new (&(_data[i])) value_type(std::forward<Args>(args)...);
                _size = n;
                _capacity = n;
            } else {
                throw std::bad_alloc();
            }
        }
    }

    template<typename T>
    template<typename... Args>
    void vector<T>::_push_back(Args&&... args) noexcept(false) {
        if (_size < _capacity) {
            new (&(_data[_size++])) value_type(std::forward<Args>(args)...);
        } else {
            size_t new_cap = 1 + _capacity * 2;
            pointer new_data = (pointer) realloc(_data, new_cap * sizeof(value_type));
            if (new_data != nullptr) {
                _data = new_data;
                new (&(_data[_size++])) value_type(std::forward<Args>(args)...);
                _capacity = new_cap;
            } else {
                throw std::bad_alloc();
            }
        }
    }

    template<typename T>
    template<typename... Args>
    vector_t::iterator vector<T>::_insert(const const_iterator &pos, Args&&... args) {
        size_type idx = pos._it._ptr - _data;

        if (idx >= _size) {
            /* Means we just need to push_back.  */
            _push_back(std::forward<Args>(args)...);
            return iterator(&(_data[_size - 1]));
        } else {
            /* Means we need to assign new value to existing slot.  */
            _data[idx].~value_type();
            new (&(_data[idx])) value_type(std::forward<Args>(args)...);

            return iterator(&(_data[idx]));
        }
    }
}