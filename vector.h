#ifndef vector_H
#define vector_H

#include <iostream>
#include <cstdlib>

namespace adt {

    template<typename T>
    class vector_iterator : public std::iterator<std::random_access_iterator_tag, T, std::ptrdiff_t , T*, T&> {
    public:
        vector_iterator(T **ptr = nullptr) : _ptr(ptr) {}
        vector_iterator(const vector_iterator<T>& other) = default;

        vector_iterator<T>& operator=(const vector_iterator<T>& other) = default;
        vector_iterator<T>& operator=(T** _ptr) {
            this->_ptr = _ptr;
            return *this;
        }

        bool operator==(const vector_iterator<T>& other) const {
            return this->_ptr == other._ptr;
        }
        bool operator!=(const vector_iterator<T>& other) const {
            return this->_ptr != other._ptr;
        }
        vector_iterator<T>& operator+=(const std::ptrdiff_t& val) {
            this->_ptr += val;
            return *this;
        }
        vector_iterator<T>& operator-=(const std::ptrdiff_t& diff_val) {
            this->_ptr -= diff_val;
            return *this;
        }
        /*Pre-(inc/dec)rement operator*/
        vector_iterator<T>& operator++() {
            this->_ptr++;
            return *this;
        }
        vector_iterator<T>& operator--() {
            this->_ptr--;
            return *this;
        }
        /*Post-(inc/dec)rement*/
        vector_iterator<T> operator++(int) {
            auto temp(*this);
            this->_ptr++;
            return temp;
        }
        vector_iterator<T> operator--(int) {
            auto temp(*this);
            this->_ptr--;
            return temp;
        }
        vector_iterator<T> operator+(const std::ptrdiff_t& val) {
            T** tmp_ptr = this->_ptr;
            this->_ptr += val;
            auto temp(*this);
            this->_ptr = tmp_ptr;
            return temp;
        }
        vector_iterator<T> operator-(const std::ptrdiff_t& val) {
            T** tmp_ptr = this->_ptr;
            this->_ptr -= val;
            auto temp(*this);
            this->_ptr = tmp_ptr;
            return temp;
        }
        std::ptrdiff_t operator-(const vector_iterator<T>& other) {
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
    class vector_r_iterator : public vector_iterator<T> {
    
    public:
        vector_r_iterator(T** ptr = nullptr) : vector_iterator<T>(ptr) {}
        vector_r_iterator(const vector_iterator<T>& other) {
            this->_ptr = other._ptr;
        }
        vector_r_iterator(const vector_r_iterator<T>& other) {
            this->_ptr = other._ptr;
        }

        vector_r_iterator<T>& operator=(const vector_iterator<T>& other) {
            this->_ptr = other._ptr;
        }
        vector_r_iterator<T>& operator=(const vector_r_iterator<T>& other) = default;
        vector_r_iterator<T>& operator=(T** _ptr) {
            this->_ptr = _ptr;
            return *this;
        }

        vector_r_iterator<T>& operator+=(const std::ptrdiff_t& val) {
            this->_ptr -= val;
            return *this;
        }
        vector_r_iterator<T>& operator-=(const std::ptrdiff_t& diff_val) {
            this->_ptr += diff_val;
            return *this;
        }

        /* Pre-increment operator. */
        vector_r_iterator<T>& operator++() {
            this->_ptr--;
            return *this;
        }
        
        /* Post-increment operator. */
        vector_r_iterator<T> operator++(int) {
            auto temp(*this);
            this->_ptr--;
            return temp;
        }
        
        /* Pre-decrement operator. */
        vector_r_iterator<T>& operator--() {
            this->_ptr++;
            return *this;
        }
        
        /* Post-decrement operator. */
        vector_r_iterator<T> operator--(int) {
            auto temp(*this);
            this->_ptr++;
            return temp;
        }

        vector_r_iterator<T> operator+(const std::ptrdiff_t& val) {
            T** tmp_ptr = this->_ptr;
            this->_ptr -= val;
            auto temp(*this);
            this->_ptr = tmp_ptr;
            return temp;
        }
        vector_r_iterator<T> operator-(const std::ptrdiff_t& val) {
            T** tmp_ptr = this->_ptr;
            this->_ptr += val;
            auto temp(*this);
            this->_ptr = tmp_ptr;
            return temp;
        }
        std::ptrdiff_t operator-(const vector_r_iterator<T>& other) {
            return std::distance(this->_ptr, other._ptr);
        }
    };

    template<typename T>
    class vector {
    private:
        T** data;
        size_t _size;
        size_t _capacity;
    public:
        vector();
        explicit vector(size_t n);
        vector(size_t n, const T& val);
        vector(size_t n, T &&val);
        explicit vector(const vector& other);
        vector(vector&& other) noexcept;
        ~vector();

        vector& operator=(vector other);

        vector_iterator<T> begin();
        vector_iterator<T> end();
        vector_r_iterator<T> rbegin();
        vector_r_iterator<T> rend();
        const vector_iterator<T> cbegin();
        const vector_iterator<T> cend();

        /*Modifiers*/
        void push_back(const T& val);
        void push_back(T&& val);
        void pop_back();
        void clear();
        void erase(size_t pos);
        vector_iterator<T> erase(vector_iterator<T>& ite);

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

        friend void swap(vector& lhs, vector& rhs) {
            using std::swap;

            swap(lhs.data, rhs.data);
            swap(lhs._size, rhs._size);
            swap(lhs._capacity, rhs._capacity);
        }
    };

    template<typename T>
    vector<T>::vector() : data(nullptr), _size(0), _capacity(0) {}

    template<typename T>
    vector<T>::vector(size_t n) : _size(n), _capacity(n) {
        this->data = (T **) calloc(n, sizeof(T*));
    }

    template<typename T>
    vector<T>::vector(size_t n, const T &val) : _size(n), _capacity(n) {
        this->data = (T **) calloc(n, sizeof(T*));
        if (this->data != nullptr) {
            for (size_t i = 0 ; i < n ; i++) {
                this->data[i] = new T(val);
            }
        }
    }

    template<typename T>
    vector<T>::vector(size_t n, T &&val) : _size(n), _capacity(n) {
        this->data = (T **) calloc(n, sizeof(T*));
        if (this->data != nullptr) {
            for (size_t i = 0 ; i < n ; i++) {
                this->data[i] = new T(std::forward<T>(val));
            }
        }
    }

    template<typename T>
    vector<T>::vector(const vector &other) : _size(other._size), _capacity(other._capacity) {
        this->data = (T **) malloc(_capacity * sizeof(T*));
        for (size_t i = 0 ; i < this->_size ; i++) {
            this->data[i] = new T(other[i]);
        }
    }

    template<typename T>
    vector<T>::vector(vector &&other) noexcept : vector() {
        swap(*this, other);
    }

    template<typename T>
    vector<T>::~vector() {
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
    vector<T> &vector<T>::operator=(vector other) {
        /*Copy and swap idiom, let the compiler handle the copy of the argument*/
        swap(*this, other);

        return *this;
    }

    template<typename T>
    vector_iterator<T> vector<T>::begin() {
        return vector_iterator<T>(&data[0]);
    }

    template<typename T>
    vector_iterator<T> vector<T>::end() {
        return vector_iterator<T>(&data[_size]);
    }

    template<typename T>
    vector_r_iterator<T> vector<T>::rbegin() {
        return vector_r_iterator<T>(&data[_size - 1]);
    }  

    template<typename T>
    vector_r_iterator<T> vector<T>::rend() {
        return vector_r_iterator<T>(&data[-1]);
    }

    template<typename T>
    const vector_iterator<T> vector<T>::cbegin() {
        return vector_iterator<const T>(&data[0]);
    }

    template<typename T>
    const vector_iterator<T> vector<T>::cend() {
        return vector_iterator<const T>(&data[_size]);
    }

    template<typename T>
    void vector<T>::push_back(const T &val) {
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
    void vector<T>::push_back(T &&val) {
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
    void vector<T>::pop_back() {
        delete data[this->_size - 1];
        this->_size--;
    }

    template<typename T>
    void vector<T>::erase(size_t pos) {
        if (pos < this->_size) {
            delete data[pos];
            for (size_t i = pos ; i < _size - 1 ; i++) {
                data[i] = data[i + 1];
            }
            _size--;
        }
    }

    template<typename T>
    vector_iterator<T> vector<T>::erase(vector_iterator<T> &ite) {
        if (ite != end()) {
            std::ptrdiff_t shift_idx = ite - begin();
            delete data[shift_idx];
            for (std::ptrdiff_t i = shift_idx ; i < _size - 1; i++) {
                data[i] = data[i + 1];
            }
            _size--;
            return vector_iterator<T>(&data[shift_idx]);
        }
        else {
            return end();
        }
    }

    template<typename T>
    T &vector<T>::operator[](size_t index) {
        return *(data[index]);
    }

    template<typename T>
    const T &vector<T>::operator[](size_t index) const {
        return *(data[index]);
    }

    template<typename T>
    T &vector<T>::at(size_t index) {
        if (index >= this->_size) {
            throw std::out_of_range("Index is out of vector range");
        }
        return *(data[index]);
    }

    template<typename T>
    const T &vector<T>::at(size_t index) const {
        if (index >= this->_size) {
            throw std::out_of_range("Index is out of vector range");
        }
        return *(data[index]);
    }

    template<typename T>
    T &vector<T>::back() {
        return *(this->data[this->_size - 1]);
    }

    template<typename T>
    const T &vector<T>::back() const {
        return *(this->data[this->_size - 1]);
    }

    template<typename T>
    T &vector<T>::front() {
        return *(this->data[0]);
    }

    template<typename T>
    const T &vector<T>::front() const {
        return *(this->data[0]);
    }

    template<typename T>
    size_t vector<T>::size() const {
        return this->_size;
    }

    template<typename T>
    size_t vector<T>::capacity() const {
        return this->_capacity;
    }

    template<typename T>
    void vector<T>::resize(size_t new_size) {
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
    void vector<T>::resize(size_t new_size, const T &val) {
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
    void vector<T>::resize(size_t new_size, T &&val) {
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
    void vector<T>::reserve(size_t new_cap) {
        if (new_cap > this->_capacity) {
            T** new_data = (T **) realloc(data, new_cap * sizeof(T*));
            if (new_data != nullptr) {
                this->data = new_data;
                this->_capacity = new_cap;
            }
        }
    }

    template<typename T>
    bool vector<T>::empty() const {
        return this->_size == 0;
    }

    template<typename T>
    void vector<T>::clear() {
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
