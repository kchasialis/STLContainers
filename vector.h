#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <cstdlib>

namespace ADT {

    template<typename T>
    class VectorIterator : public std::iterator<std::random_access_iterator_tag, T, std::ptrdiff_t , T*, T&> {
    public:
        VectorIterator(T **ptr = nullptr) : _ptr(ptr) {}
        VectorIterator(const VectorIterator<T>& other) = default;

        VectorIterator<T>& operator=(const VectorIterator<T>& other) = default;
        VectorIterator<T>& operator=(T** _ptr) {
            this->_ptr = _ptr;
            return *this;
        }

        bool operator==(const VectorIterator<T>& other) const {
            return this->_ptr == other._ptr;
        }
        bool operator!=(const VectorIterator<T>& other) const {
            return this->_ptr != other._ptr;
        }
        VectorIterator<T>& operator+=(const std::ptrdiff_t& val) {
            this->_ptr += val;
            return *this;
        }
        VectorIterator<T>& operator-=(const std::ptrdiff_t& diff_val) {
            this->_ptr -= diff_val;
            return *this;
        }
        /*Pre-(inc/dec)rement operator*/
        VectorIterator<T>& operator++() {
            this->_ptr++;
            return *this;
        }
        VectorIterator<T>& operator--() {
            this->_ptr--;
            return *this;
        }
        /*Post-(inc/dec)rement*/
        VectorIterator<T> operator++(int) {
            auto temp(*this);
            this->_ptr++;
            return temp;
        }
        VectorIterator<T> operator--(int) {
            auto temp(*this);
            this->_ptr--;
            return temp;
        }
        VectorIterator<T> operator+(const std::ptrdiff_t& val) {
            T** tmp_ptr = this->_ptr;
            this->_ptr += val;
            auto temp(*this);
            this->_ptr = tmp_ptr;
            return temp;
        }
        VectorIterator<T> operator-(const std::ptrdiff_t& val) {
            T** tmp_ptr = this->_ptr;
            this->_ptr -= val;
            auto temp(*this);
            this->_ptr = tmp_ptr;
            return temp;
        }
        std::ptrdiff_t operator-(const VectorIterator<T>& other) {
            return std::distance(other._ptr, this->_ptr);
        }
        T& operator*() {
            return *(*this->_ptr);
        }
        const T& operator*() const {
            return *(*this->_ptr);
        }
        T* operator->() {
            return *this->_ptr;
        }
    protected:
        T **_ptr;
    };

    template<typename T>
    class VectorRIterator : public VectorIterator<T> {
    
    public:
        VectorRIterator(T** ptr = nullptr) : VectorIterator<T>(ptr) {}
        VectorRIterator(const VectorIterator<T>& other) {
            this->_ptr = other._ptr;
        }
        VectorRIterator(const VectorRIterator<T>& other) {
            this->_ptr = other._ptr;
        }

        VectorRIterator<T>& operator=(const VectorIterator<T>& other) {
            this->_ptr = other._ptr;
        }
        VectorRIterator<T>& operator=(const VectorRIterator<T>& other) = default;
        VectorRIterator<T>& operator=(T** _ptr) {
            this->_ptr = _ptr;
            return *this;
        }

        VectorRIterator<T>& operator+=(const std::ptrdiff_t& val) {
            this->_ptr -= val;
            return *this;
        }
        VectorRIterator<T>& operator-=(const std::ptrdiff_t& diff_val) {
            this->_ptr += diff_val;
            return *this;
        }

        /* Pre-increment operator. */
        VectorRIterator<T>& operator++() {
            this->_ptr--;
            return *this;
        }
        
        /* Post-increment operator. */
        VectorRIterator<T> operator++(int) {
            auto temp(*this);
            this->_ptr--;
            return temp;
        }
        
        /* Pre-decrement operator. */
        VectorRIterator<T>& operator--() {
            this->_ptr++;
            return *this;
        }
        
        /* Post-decrement operator. */
        VectorRIterator<T> operator--(int) {
            auto temp(*this);
            this->_ptr++;
            return temp;
        }

        VectorRIterator<T> operator+(const std::ptrdiff_t& val) {
            T** tmp_ptr = this->_ptr;
            this->_ptr -= val;
            auto temp(*this);
            this->_ptr = tmp_ptr;
            return temp;
        }
        VectorRIterator<T> operator-(const std::ptrdiff_t& val) {
            T** tmp_ptr = this->_ptr;
            this->_ptr += val;
            auto temp(*this);
            this->_ptr = tmp_ptr;
            return temp;
        }
        std::ptrdiff_t operator-(const VectorRIterator<T>& other) {
            return std::distance(this->_ptr, other._ptr);
        }
    };

    template<typename T>
    class Vector {
    private:
        T** data;
        size_t _size;
        size_t _capacity;
    public:
        Vector();
        explicit Vector(size_t n);
        Vector(size_t n, const T& val);
        Vector(size_t n, T &&val);
        explicit Vector(const Vector& other);
        Vector(Vector&& other) noexcept;
        ~Vector();

        Vector& operator=(Vector other);

        VectorIterator<T> begin();
        VectorIterator<T> end();
        VectorRIterator<T> rbegin();
        VectorRIterator<T> rend();
        const VectorIterator<T> cbegin();
        const VectorIterator<T> cend();

        /*Modifiers*/
        void push_back(const T& val);
        void push_back(T&& val);
        void pop_back();
        void clear();
        void erase(size_t pos);
        VectorIterator<T> erase(VectorIterator<T>& ite);

        /*Capacity*/
        size_t size() const;
        size_t capacity() const;
        void resize(size_t new_size);
        void resize(size_t new_size, const T& val);
        void resize(size_t new_size, T &&val);
        void reserve(size_t new_cap);
        bool empty() const;

        /*Element access*/
        T& operator[](size_t index);
        const T& operator[](size_t index) const;
        T& at(size_t index);
        const T& at(size_t index) const;
        T& back();
        const T& back() const;
        T& front();
        const T& front() const;

        friend void swap(Vector& lhs, Vector& rhs) {
            using std::swap;

            swap(lhs.data, rhs.data);
            swap(lhs._size, rhs._size);
            swap(lhs._capacity, rhs._capacity);
        }
    };

    template<typename T>
    Vector<T>::Vector() : data(nullptr), _size(0), _capacity(0) {}

    template<typename T>
    Vector<T>::Vector(size_t n) : _size(n), _capacity(n) {
        this->data = (T **) calloc(n, sizeof(T*));
    }

    template<typename T>
    Vector<T>::Vector(size_t n, const T &val) : _size(n), _capacity(n) {
        this->data = (T **) calloc(n, sizeof(T*));
        if (this->data != nullptr) {
            for (size_t i = 0 ; i < n ; i++) {
                this->data[i] = new T(val);
            }
        }
    }

    template<typename T>
    Vector<T>::Vector(size_t n, T &&val) : _size(n), _capacity(n) {
        this->data = (T **) calloc(n, sizeof(T*));
        if (this->data != nullptr) {
            for (size_t i = 0 ; i < n ; i++) {
                this->data[i] = new T(std::forward<T>(val));
            }
        }
    }

    template<typename T>
    Vector<T>::Vector(const Vector &other) : _size(other._size), _capacity(other._capacity) {
        this->data = (T **) malloc(_capacity * sizeof(T*));
        for (size_t i = 0 ; i < this->_size ; i++) {
            this->data[i] = new T(other[i]);
        }
    }

    template<typename T>
    Vector<T>::Vector(Vector &&other) noexcept : Vector() {
        swap(*this, other);
    }

    template<typename T>
    Vector<T>::~Vector() {
        for (size_t i = 0 ; i < this->_size ; i++) {
            if (data[i]) {
                delete data[i];
            }
        }
        if (data) {
            free(data);
        }
    }

    template<typename T>
    Vector<T> &Vector<T>::operator=(Vector other) {
        /*Copy and swap idiom, let the compiler handle the copy of the argument*/
        swap(*this, other);

        return *this;
    }

    template<typename T>
    VectorIterator<T> Vector<T>::begin() {
        return VectorIterator<T>(&data[0]);
    }

    template<typename T>
    VectorIterator<T> Vector<T>::end() {
        return VectorIterator<T>(&data[_size]);
    }

    template<typename T>
    VectorRIterator<T> Vector<T>::rbegin() {
        return VectorRIterator<T>(&data[_size - 1]);
    }  

    template<typename T>
    VectorRIterator<T> Vector<T>::rend() {
        return VectorRIterator<T>(&data[-1]);
    }

    template<typename T>
    const VectorIterator<T> Vector<T>::cbegin() {
        return VectorIterator<const T>(&data[0]);
    }

    template<typename T>
    const VectorIterator<T> Vector<T>::cend() {
        return VectorIterator<const T>(&data[_size]);
    }

    template<typename T>
    void Vector<T>::push_back(const T &val) {
        if (this->_size < this->_capacity) {
            data[this->_size++] = new T(val);
        } else {
            size_t new_cap = 1 + this->_capacity * 2;
            T** new_data = (T**) realloc(data, new_cap * sizeof(T*));
            if (new_data != nullptr) {
                this->data = new_data;
                this->_capacity = new_cap;
                this->data[this->_size++] = new T(val);
            }
        }
    }

    template<typename T>
    void Vector<T>::push_back(T &&val) {
        if (this->_size < this->_capacity) {
            data[this->_size++] = new T(std::forward<T>(val));
        } else {
            size_t new_cap = 1 + this->_capacity * 2;
            T** new_data = (T**) realloc(data, new_cap * sizeof(T*));
            if (new_data != nullptr) {
                this->data = new_data;
                this->_capacity = new_cap;
                this->data[this->_size++] = new T(std::forward<T>(val));
            }
        }
    }

    template<typename T>
    void Vector<T>::pop_back() {
        delete data[this->_size - 1];
        this->_size--;
    }

    template<typename T>
    void Vector<T>::erase(size_t pos) {
        if (pos < this->_size) {
            delete data[pos];
            for (size_t i = pos ; i < _size - 1 ; i++) {
                data[i] = data[i + 1];
            }
            _size--;
        }
    }

    template<typename T>
    VectorIterator<T> Vector<T>::erase(VectorIterator<T> &ite) {
        if (ite != end()) {
            std::ptrdiff_t shift_idx = ite - begin();
            delete data[shift_idx];
            for (std::ptrdiff_t i = shift_idx ; i < _size - 1; i++) {
                data[i] = data[i + 1];
            }
            _size--;
            return VectorIterator<T>(&data[shift_idx]);
        }
        else {
            return end();
        }
    }

    template<typename T>
    T &Vector<T>::operator[](size_t index) {
        return *(data[index]);
    }

    template<typename T>
    const T &Vector<T>::operator[](size_t index) const {
        return *(data[index]);
    }

    template<typename T>
    T &Vector<T>::at(size_t index) {
        if (index >= this->_size) {
            throw std::out_of_range("Index is out of vector range");
        }
        return *(data[index]);
    }

    template<typename T>
    const T &Vector<T>::at(size_t index) const {
        if (index >= this->_size) {
            throw std::out_of_range("Index is out of vector range");
        }
        return *(data[index]);
    }

    template<typename T>
    T &Vector<T>::back() {
        return *(this->data[this->_size - 1]);
    }

    template<typename T>
    const T &Vector<T>::back() const {
        return *(this->data[this->_size - 1]);
    }

    template<typename T>
    T &Vector<T>::front() {
        return *(this->data[0]);
    }

    template<typename T>
    const T &Vector<T>::front() const {
        return *(this->data[0]);
    }

    template<typename T>
    size_t Vector<T>::size() const {
        return this->_size;
    }

    template<typename T>
    size_t Vector<T>::capacity() const {
        return this->_capacity;
    }

    template<typename T>
    void Vector<T>::resize(size_t new_size) {
        if (new_size < this->_size) {
            for (size_t i = new_size ; i < this->_size ; i++) {
                /*Destroy these elements*/
                delete data[i];
            }
        }
        else if (new_size > this->_size && new_size < this->_capacity) {
            for (size_t i = this->_size ; i < new_size ; i++) {
                data[i] = new T();
            }
        }
        else if (new_size > this->_size && new_size > this->_capacity) {
            T** new_data = (T **) realloc(data, new_size * sizeof(T*));
            if (new_data != nullptr) {
                this->data = new_data;
                this->_capacity = new_size;
            }
        }
        this->_size = new_size;
    }

    template<typename T>
    void Vector<T>::resize(size_t new_size, const T &val) {
        if (new_size < this->_size) {
            for (size_t i = new_size ; i < this->_size ; i++) {
                /*Destroy these elements*/
                delete data[i];
            }
            this->_size = new_size;
        }
        else if (new_size > this->_size && new_size < this->_capacity) {
            for (size_t i = this->_size ; i < new_size ; i++) {
                data[i] = new T(val);
            }
        }
        else if (new_size > this->_size && new_size > this->_capacity) {
            T** new_data = (T **) realloc(data, new_size * sizeof(T*));
            if (new_data != nullptr) {
                this->data = new_data;
                for (size_t i = this->_size ; i < new_size ; i++) {
                    data[i] = new T(val);
                }
                this->_size = new_size;
                this->_capacity = new_size;
            }
        }
    }

    template<typename T>
    void Vector<T>::resize(size_t new_size, T &&val) {
        if (new_size < this->_size) {
            for (size_t i = new_size ; i < this->_size ; i++) {
                /*Destroy these elements*/
                delete data[i];
            }
            this->_size = new_size;
        }
        else if (new_size > this->_size && new_size < this->_capacity) {
            for (size_t i = this->_size ; i < new_size ; i++) {
                data[i] = new T(std::forward<T>(val));
            }
        }
        else if (new_size > this->_size && new_size > this->_capacity) {
            T** new_data = (T **) realloc(data, new_size * sizeof(T*));
            if (new_data != nullptr) {
                this->data = new_data;
                for (size_t i = this->_size ; i < new_size ; i++) {
                    data[i] = new T(std::forward<T>(val));
                }
                this->_size = new_size;
                this->_capacity = new_size;
            }
        }
    }


    template<typename T>
    void Vector<T>::reserve(size_t new_cap) {
        if (new_cap > this->_capacity) {
            T** new_data = (T **) realloc(data, new_cap * sizeof(T*));
            if (new_data != nullptr) {
                this->data = new_data;
                this->_capacity = new_cap;
            }
        }
    }

    template<typename T>
    bool Vector<T>::empty() const {
        return this->_size == 0;
    }

    template<typename T>
    void Vector<T>::clear() {
        for (size_t i = 0 ; i < this->_size ; i++) {
            delete data[i];
            data[i] = nullptr;
        }
        free(data);
        data = nullptr;
        this->_size = 0;
        this->_capacity = 0;
    }
}

#endif
