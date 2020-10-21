#pragma once

#include <iostream>
#include <cstdlib>

namespace adt {

    template<typename T>
    class const_vector_iterator;

    template<typename T>
    class vector_iterator : public std::iterator<std::random_access_iterator_tag, T, std::ptrdiff_t , T*, T&> {
    public:
        vector_iterator(T **ptr = nullptr) : _ptr(ptr) {}
        vector_iterator(const vector_iterator& other) = default;

        vector_iterator &operator=(const vector_iterator &other) {
            this->_ptr = other._ptr;
            return *this;
        }
        vector_iterator &operator=(T **ptr) {
            this->_ptr = ptr;
            return *this;
        }

        bool operator==(const vector_iterator &other) const {
            return this->_ptr == other._ptr;
        }
        bool operator!=(const vector_iterator &other) const {
            return this->_ptr != other._ptr;
        }

        virtual vector_iterator &operator+=(const std::ptrdiff_t &val) {
            _ptr += val;
            return *this;
        }
        virtual vector_iterator &operator-=(const std::ptrdiff_t &diff_val) {
            _ptr -= diff_val;
            return *this;
        }

        virtual vector_iterator &operator++() {
            _ptr++;
            return *this;
        }
        virtual vector_iterator operator++(int) & {
            auto temp(*this);
            _ptr++;
            return temp;
        }
        virtual vector_iterator &operator--() {
            _ptr--;
            return *this;
        }
        virtual vector_iterator operator--(int) & {
            auto temp(*this);
            _ptr--;
            return temp;
        }
        virtual vector_iterator operator+(const std::ptrdiff_t &val) & {
            T **tmp_ptr = this->_ptr;
            _ptr += val;
            auto temp(*this);
            _ptr = tmp_ptr;
            return temp;
        }
        virtual vector_iterator operator-(const std::ptrdiff_t &val) & {
            T **tmp_ptr = this->_ptr;
            _ptr -= val;
            auto temp(*this);
            _ptr = tmp_ptr;
            return temp;
        }
        std::ptrdiff_t operator-(const vector_iterator &other) {
            return std::distance(other._ptr, this->_ptr);
        }

        T& operator*() {
            return *(*_ptr);
        }
        const T& operator*() const {
            return *(*_ptr);
        }
        T* operator->() {
            return *(_ptr);
        }
        const T* operator->() const {
            return *(_ptr);
        }

    protected:
        friend class const_vector_iterator<T>;
        T **_ptr;
    };

    template<typename T>
    class const_vector_iterator : public std::iterator<std::random_access_iterator_tag, T, std::ptrdiff_t , T*, T&> {
    public:
        const_vector_iterator(T **ptr = nullptr) : _ptr(ptr) {}
        const_vector_iterator(const const_vector_iterator &other) = default;
        const_vector_iterator(const vector_iterator<T> &other) : _ptr(other._ptr) {}

        const_vector_iterator &operator=(const const_vector_iterator &other) {
            if (this == &other) {
                return *this;
            }
            this->_ptr = other._ptr;
            return *this;
        }
        const_vector_iterator &operator=(const vector_iterator<T> &other) {
            this->_ptr = other._ptr;
        }
        const_vector_iterator &operator=(T **ptr) {
            this->_ptr = ptr;
            return *this;
        }

        bool operator==(const const_vector_iterator &other) const {
            return this->_ptr == other._ptr;
        }
        bool operator!=(const const_vector_iterator &other) const {
            return this->_ptr != other._ptr;
        }

        virtual const_vector_iterator &operator+=(const std::ptrdiff_t &val) {
            _ptr += val;
            return *this;
        }
        virtual const_vector_iterator &operator-=(const std::ptrdiff_t &diff_val) {
            _ptr -= diff_val;
            return *this;
        }

        virtual const_vector_iterator &operator++() {
            _ptr++;
            return *this;
        }
        virtual const_vector_iterator operator++(int) & {
            auto temp(*this);
            _ptr++;
            return temp;
        }
        virtual const_vector_iterator &operator--() {
            _ptr--;
            return *this;
        }
        virtual const_vector_iterator operator--(int) & {
            auto temp(*this);
            _ptr--;
            return temp;
        }
        virtual const_vector_iterator operator+(const std::ptrdiff_t &val) & {
            T **tmp_ptr = this->_ptr;
            _ptr += val;
            auto temp(*this);
            _ptr = tmp_ptr;
            return temp;
        }
        virtual const_vector_iterator operator-(const std::ptrdiff_t &val) & {
            T **tmp_ptr = this->_ptr;
            _ptr -= val;
            auto temp(*this);
            _ptr = tmp_ptr;
            return temp;
        }
        std::ptrdiff_t operator-(const const_vector_iterator &other) {
            return std::distance(other._ptr, this->_ptr);
        }

        const T& operator*() const {
            return *(*_ptr);
        }
        const T* operator->() const {
            return *(_ptr);
        }

    protected:
        T **_ptr;
    };

    template<typename T>
    class vector_r_iterator : public vector_iterator<T> {
    public:
        vector_r_iterator(T **ptr = nullptr) : vector_iterator<T>(ptr) {}
        vector_r_iterator(const vector_r_iterator &other) {
            this->_ptr = other._ptr;
        }

        vector_r_iterator<T> &operator+=(const std::ptrdiff_t &val) {
            this->_ptr -= val;
            return *this;
        }
        vector_r_iterator<T> &operator-=(const std::ptrdiff_t &diff_val) {
            this->_ptr += diff_val;
            return *this;
        }

        vector_r_iterator<T> &operator++() {
            this->_ptr--;
            return *this;
        }
        vector_r_iterator<T> operator++(int) {
            auto temp(*this);
            this->_ptr--;
            return temp;
        }
        vector_r_iterator<T> &operator--() {
            this->_ptr++;
            return *this;
        }
        vector_r_iterator<T> operator--(int) {
            auto temp(*this);
            this->_ptr++;
            return temp;
        }

        vector_r_iterator<T> operator+(const std::ptrdiff_t &val) {
            T **tmp_ptr = this->_ptr;
            this->_ptr -= val;
            auto temp(*this);
            this->_ptr = tmp_ptr;
            return temp;
        }
        vector_r_iterator<T> operator-(const std::ptrdiff_t &val) {
            T **tmp_ptr = this->_ptr;
            this->_ptr += val;
            auto temp(*this);
            this->_ptr = tmp_ptr;
            return temp;
        }
    };

    template<typename T>
    class vector {
    private:
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        using pointer = T*;
        using const_pointer = const T*;
        using iterator = adt::vector_iterator<T>;
        using const_iterator = adt::const_vector_iterator<T>;
        using reverse_iterator = adt::vector_r_iterator<T>;
        /* TODO: add const reverse iterator and methods.  */

        pointer *_data;
        size_t _size;
        size_t _capacity;
    public:
        vector();
        explicit vector(size_t n);
        vector(size_t n, const value_type &val);
        vector(size_t n, value_type &&val);
        vector(const vector& other);
        vector(vector&& other) noexcept;
        ~vector();

        vector& operator=(vector other);

        iterator begin();
        const_iterator begin() const noexcept;
        iterator end();
        const_iterator end() const noexcept;
        reverse_iterator rbegin();
        reverse_iterator rend();
        const_iterator cbegin() const noexcept;
        const_iterator cend() const noexcept;

        /* Modifiers.  */
        void push_back(const_reference val);
        void push_back(value_type &&val);
        /* TODO: add emplace, emplace_back and wrap push_back's into this.  */
        void pop_back();
        void clear();
        void erase(size_t pos);
        iterator erase(const_iterator ite);

        /* Capacity.  */
        size_t size() const;
        size_t capacity() const;
        void resize(size_t new_size);
        void resize(size_t new_size, const_reference val);
        void resize(size_t new_size, value_type &&val);
        void reserve(size_t new_cap);
        bool empty() const;

        /* Element access.  */
        reference operator[](size_t index);
        const_reference operator[](size_t index) const;
        reference at(size_t index);
        const_reference at(size_t index) const;
        reference back();
        const_reference back() const;
        reference front();
        const_reference front() const;

        friend void swap(vector& lhs, vector& rhs) {
            using std::swap;

            swap(lhs._data, rhs._data);
            swap(lhs._size, rhs._size);
            swap(lhs._capacity, rhs._capacity);
        }
    };

    template<typename T>
    vector<T>::vector() : _data(nullptr), _size(0), _capacity(0) {}

    template<typename T>
    vector<T>::vector(size_t n) : _size(n), _capacity(n) {
        _data = (pointer *) calloc(n, sizeof(pointer));
    }

    template<typename T>
    vector<T>::vector(size_t n, const_reference val) : _size(n), _capacity(n) {
        _data = (pointer *) calloc(n, sizeof(pointer));
        if (_data != nullptr) {
            for (size_t i = 0 ; i < n ; i++) {
                _data[i] = new value_type(val);
            }
        }
    }

    template<typename T>
    vector<T>::vector(size_t n, value_type &&val) : _size(n), _capacity(n) {
        _data = (pointer *) calloc(n, sizeof(pointer));
        if (_data != nullptr) {
            for (size_t i = 0 ; i < n ; i++) {
                _data[i] = new value_type(std::forward<value_type>(val));
            }
        }
    }

    template<typename T>
    vector<T>::vector(const vector &other) : _size(other._size), _capacity(other._capacity) {
        _data = (pointer *) malloc(_capacity * sizeof(pointer));
        for (size_t i = 0 ; i < _size ; i++) {
            _data[i] = new value_type(other[i]);
        }
    }

    template<typename T>
    vector<T>::vector(vector &&other) noexcept : vector() {
        swap(*this, other);
    }

    template<typename T>
    vector<T>::~vector() {
        for (size_t i = 0 ; i < _size ; i++) {
            if (_data[i]) {
                delete _data[i];
            }
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
    typename vector<T>::iterator vector<T>::begin() {
        return iterator(&_data[0]);
    }

    template<typename T>
    typename vector<T>::const_iterator vector<T>::begin() const noexcept {
        return const_iterator(&_data[0]);
    }

    template<typename T>
    typename vector<T>::iterator vector<T>::end() {
        return iterator(&_data[_size]);
    }

    template<typename T>
    typename vector<T>::const_iterator vector<T>::end() const noexcept {
        return const_iterator(&_data[_size]);
    }

    template<typename T>
    typename vector<T>::reverse_iterator vector<T>::rbegin() {
        return reverse_iterator(&_data[_size - 1]);
    }  

    template<typename T>
    typename vector<T>::reverse_iterator vector<T>::rend() {
        return reverse_iterator(&_data[-1]);
    }

    template<typename T>
    typename vector<T>::const_iterator vector<T>::cbegin() const noexcept {
        return const_iterator(&_data[0]);
    }

    template<typename T>
    typename vector<T>::const_iterator vector<T>::cend() const noexcept {
        return const_iterator(&_data[_size]);
    }

    template<typename T>
    void vector<T>::push_back(const_reference val) {
        if (_size < _capacity) {
            _data[_size++] = new value_type(val);
        } else {
            size_t new_cap = 1 + _capacity * 2;
            pointer *new_data = (pointer *) realloc(_data, new_cap * sizeof(pointer));
            if (new_data != nullptr) {
                _data = new_data;
                _data[_size++] = new value_type(val);
                _capacity = new_cap;
            }
        }
    }

    template<typename T>
    void vector<T>::push_back(value_type &&val) {
        if (_size < _capacity) {
            _data[_size++] = new value_type(std::forward<value_type>(val));
        } else {
            size_t new_cap = 1 + _capacity * 2;
            pointer *new_data = (pointer *) realloc(_data, new_cap * sizeof(pointer));
            if (new_data != nullptr) {
                _data = new_data;
                _capacity = new_cap;
                _data[_size++] = new value_type(std::forward<value_type>(val));
            }
        }
    }

    template<typename T>
    void vector<T>::pop_back() {
        delete _data[_size - 1];
        _size--;
    }

    template<typename T>
    void vector<T>::erase(size_t pos) {
        if (pos < _size) {
            delete _data[pos];
            for (size_t i = pos ; i < _size - 1 ; i++) {
                _data[i] = _data[i + 1];
            }
            _size--;
        }
    }

    template<typename T>
    typename vector<T>::iterator vector<T>::erase(const_iterator ite) {
        if (ite != end()) {
            std::ptrdiff_t shift_idx = ite - begin();
            delete _data[shift_idx];
            for (std::ptrdiff_t i = shift_idx ; i < _size - 1; i++) {
                _data[i] = _data[i + 1];
            }
            _size--;
            return iterator(&_data[shift_idx]);
        }
        else {
            return end();
        }
    }

    template<typename T>
    typename vector<T>::reference vector<T>::operator[](size_t index) {
        return *(_data[index]);
    }

    template<typename T>
    typename vector<T>::const_reference vector<T>::operator[](size_t index) const {
        return *(_data[index]);
    }

    template<typename T>
    typename vector<T>::reference vector<T>::at(size_t index) {
        if (index >= _size) {
            throw std::out_of_range("Index is out of vector range");
        }
        return *(_data[index]);
    }

    template<typename T>
    typename vector<T>::const_reference vector<T>::at(size_t index) const {
        if (index >= _size) {
            throw std::out_of_range("Index is out of vector range");
        }
        return *(_data[index]);
    }

    template<typename T>
    typename vector<T>::reference vector<T>::back() {
        return *(_data[_size - 1]);
    }

    template<typename T>
    typename vector<T>::const_reference vector<T>::back() const {
        return *(_data[_size - 1]);
    }

    template<typename T>
    typename vector<T>::reference vector<T>::front() {
        return *(_data[0]);
    }

    template<typename T>
    typename vector<T>::const_reference vector<T>::front() const {
        return *(_data[0]);
    }

    template<typename T>
    size_t vector<T>::size() const {
        return _size;
    }

    template<typename T>
    size_t vector<T>::capacity() const {
        return _capacity;
    }

    template<typename T>
    void vector<T>::resize(size_t new_size) {
        if (new_size < _size) {
            for (size_t i = new_size ; i < _size ; i++) {
                /*Destroy these elements*/
                delete _data[i];
            }
        }
        else if (new_size > _size && new_size < _capacity) {
            for (size_t i = _size ; i < new_size ; i++) {
                _data[i] = new value_type();
            }
        }
        else if (new_size > _size && new_size > _capacity) {
            pointer *new_data = (pointer *) realloc(_data, new_size * sizeof(pointer));
            if (new_data != nullptr) {
                _data = new_data;
                _capacity = new_size;
            }
        }
        _size = new_size;
    }

    template<typename T>
    void vector<T>::resize(size_t new_size, const value_type &val) {
        if (new_size < _size) {
            for (size_t i = new_size ; i < _size ; i++) {
                /*Destroy these elements*/
                delete _data[i];
            }
            _size = new_size;
        }
        else if (new_size > _size && new_size < _capacity) {
            for (size_t i = _size ; i < new_size ; i++) {
                _data[i] = new value_type(val);
            }
        }
        else if (new_size > _size && new_size > _capacity) {
            pointer *new_data = (pointer *) realloc(_data, new_size * sizeof(pointer));
            if (new_data != nullptr) {
                _data = new_data;
                for (size_t i = _size ; i < new_size ; i++) {
                    _data[i] = new value_type(val);
                }
                _size = new_size;
                _capacity = new_size;
            }
        }
    }

    template<typename T>
    void vector<T>::resize(size_t new_size, value_type &&val) {
        if (new_size < _size) {
            for (size_t i = new_size ; i < _size ; i++) {
                /*Destroy these elements*/
                _data[i];
            }
            _size = new_size;
        }
        else if (new_size > _size && new_size < _capacity) {
            for (size_t i = _size ; i < new_size ; i++) {
                _data[i] = new value_type(std::forward<value_type>(val));
            }
        }
        else if (new_size > _size && new_size > _capacity) {
            pointer *new_data = (pointer *) realloc(_data, new_size * sizeof(pointer));
            if (new_data != nullptr) {
                _data = new_data;
                for (size_t i = _size ; i < new_size ; i++) {
                    _data[i] = new value_type(std::forward<value_type>(val));
                }
                _size = new_size;
                _capacity = new_size;
            }
        }
    }


    template<typename T>
    void vector<T>::reserve(size_t new_cap) {
        if (new_cap > _capacity) {
            pointer *new_data = (pointer *) realloc(_data, new_cap * sizeof(pointer));
            if (new_data != nullptr) {
                _data = new_data;
                _capacity = new_cap;
            }
        }
    }

    template<typename T>
    bool vector<T>::empty() const {
        return _size == 0;
    }

    template<typename T>
    void vector<T>::clear() {
        for (size_t i = 0 ; i < _size ; i++) {
            delete _data[i];
            _data[i] = nullptr;
        }
        _size = 0;
        _capacity = 0;
    }
}