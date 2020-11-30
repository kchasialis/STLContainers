#pragma once

#include <memory>
#include <iostream>
#include <initializer_list>

#define list_t typename list<T>

namespace adt {

    template<typename T>
    class list {
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
        struct list_node {
            T data;
            list_node *next;
            list_node *previous;

            list_node() noexcept : data(), next(nullptr), previous(nullptr) {}
            explicit list_node(const T &val) noexcept : data(val), next(nullptr), previous(nullptr) {}
            explicit list_node(T &&val) noexcept : data(std::forward<T>(val)), next(nullptr), previous(nullptr)  {}
            template<typename... Args>
            list_node(Args&&... args) : data(std::forward<Args>(args)...), next(nullptr), previous(nullptr) {}
            explicit list_node(const list_node &node) = default;
            explicit list_node(list_node &&node) = default;
        };

        struct default_comp_unique {
            bool operator()(const T &lhs, const T &rhs) {
                return lhs == rhs;
            }
        };

        struct default_comp_sort {
            bool operator()(const T &lhs, const T &rhs) {
                return lhs < rhs;
            }
        };

        using internal_ptr = list_node *;

        internal_ptr _head;
        internal_ptr _sentinel;
        size_t _size{};

    public:
        class iterator {
            friend class list;
            using internal_ptr = list::internal_ptr;

        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = list::value_type;
            using reference = list::reference;
            using const_reference = list::const_reference;
            using pointer = list::pointer;
            using const_pointer = list::const_pointer;
            using difference_type = list::difference_type;

            iterator(const iterator &other) = default;
            iterator(iterator &&other) = default;
        
            iterator &operator=(const iterator &other) = default;
            iterator &operator=(internal_ptr ptr) {
                this->_ptr = ptr;
                return *this;
            }

            bool operator==(const iterator &rhs) const { return this->_ptr == rhs._ptr; }
            bool operator!=(const iterator &rhs) const { return !(*this == rhs); }

            iterator& operator++() {
                if (_ptr) {
                    _ptr = _ptr->next;
                }
                return *this;     
            }
            iterator operator++(int) {
                iterator tmp(*this);
                ++(*this);

                return tmp;
            }
            iterator &operator--() {
                if (_ptr) {
                    _ptr = _ptr->previous;
                }
                return *this;
            }
            iterator operator--(int) {
                iterator tmp(*this);
                --(*this);

                return tmp;
            }

            reference operator*() { return _ptr->data; }
            const_reference operator*() const { return _ptr->data; }
            pointer operator->() { return &(_ptr->data); }
            const_pointer operator->() const { return &(_ptr->data); }

        private:
            internal_ptr _ptr;

            iterator(internal_ptr ptr = nullptr) : _ptr(ptr) {}
        };

        class const_iterator {
            friend class list;
            using internal_ptr = list::internal_ptr;

        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = list::value_type;
            using reference = list::const_reference;
            using pointer = list::const_pointer;
            using difference_type = list::difference_type;

            /* Implicit conversion from iterator.  */
            const_iterator(iterator it) : _it(std::move(it)) {}

            const_iterator &operator=(const const_iterator &other) = default;
            const_iterator &operator=(internal_ptr ptr) {
                this->_it = ptr;
                return *this;
            }

            bool operator==(const const_iterator &other) const { return this->_it == other._it; }
            bool operator!=(const const_iterator &other) const { return !(*this == other); }

            reference operator*() const { return *_it; }
            pointer operator->() const { return _it.operator->(); }

            const_iterator &operator++() {
                ++_it;
                return *this;
            }
            const_iterator operator++(int) { return _it++; }
            const_iterator &operator--() {
                --_it;
                return *this;
            }
            const_iterator operator--(int) { return _it--; }

        private:
            iterator _it;

            const_iterator(internal_ptr ptr = nullptr) : _it(ptr) {};
        };

        class reverse_iterator {
            friend class list;
            using internal_ptr = list::internal_ptr;

        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = list::value_type;
            using reference = list::reference;
            using const_reference = list::const_reference;
            using pointer = list::pointer;
            using const_pointer = list::const_pointer;
            using difference_type = list::difference_type;

            reverse_iterator(const reverse_iterator &other) = default;
            reverse_iterator(reverse_iterator &&other) = default;

            reverse_iterator &operator=(const reverse_iterator &other) = default;
            reverse_iterator &operator=(internal_ptr ptr) {
                this->_it = ptr;
                return *this;
            }

            bool operator==(const reverse_iterator &other) const { return this->_it == other._it; }
            bool operator==(internal_ptr ptr) const { return _it == ptr; }
            bool operator!=(const reverse_iterator &other) const { return !(*this == other); }
            bool operator!=(internal_ptr ptr) const { return !(*this == ptr); }

            reverse_iterator &operator++() {
                --_it;
                return *this;
            }
            reverse_iterator operator++(int) &{
                auto temp(*this);
                --_it;
                return temp;
            }
            reverse_iterator &operator--() {
                ++_it;
                return *this;
            }
            reverse_iterator operator--(int) &{
                auto temp(*this);
                ++_it;
                return temp;
            }

            reference operator*() { return *_it; }
            const_reference operator*() const { return *_it; }
            pointer operator->() { return _it.operator->(); }
            const_pointer operator->() const { return _it.operator->(); }

        private:
            iterator _it;

            reverse_iterator(internal_ptr ptr = nullptr) : _it(ptr) {};
        };

        class const_reverse_iterator {
            friend class list;

        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = list::value_type;
            using reference = list::const_reference;
            using pointer = list::const_pointer;
            using difference_type = list::difference_type;

            /* Implicit conversion from iterator.  */
            const_reverse_iterator(reverse_iterator it) : _it(std::move(it)) {}

            const_reverse_iterator &operator=(const const_reverse_iterator &other) = default;
            const_reverse_iterator &operator=(internal_ptr ptr) {
                this->_it = ptr;
                return *this;
            }

            bool operator==(const const_reverse_iterator &other) const { return this->_it == other._it; }
            bool operator!=(const const_reverse_iterator &other) const { return !(*this == other); }

            const_reverse_iterator &operator++() {
                ++_it;
                return *this;
            }
            const_reverse_iterator operator++(int) { return _it++; }
            const_reverse_iterator &operator--() {
                --_it;
                return *this;
            }
            const_reverse_iterator operator--(int) { return _it--; }

            reference operator*() const { return *_it; }
            pointer operator->() const { return _it.operator->(); }

        private:
            reverse_iterator _it;

            const_reverse_iterator(internal_ptr ptr = nullptr) : _it(ptr) {};
        };

        /* Constructors/Destructors.  */
        list();
        list(size_type n, const value_type &val);
        explicit list(const list &other);
        explicit list(list &&other) noexcept;
        explicit list(std::initializer_list<value_type> &il);
        ~list();

        list& operator=(list other);

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
        bool empty() const noexcept;
        size_type size() const noexcept;

        /* Element access. */
        const_reference front() const;
        reference front();
        const_reference back() const;
        reference back();

        /* Modifiers. */
        template<class... Args>
        void emplace_front(Args&&... args);
        void push_front(const value_type &val);
        void push_front(value_type &&val);
        void pop_front();
        template<class... Args>
        void emplace_back(Args&&... args);
        void push_back(const value_type &val);
        void push_back(value_type &&val);
        void pop_back();
        template<class... Args>
        iterator emplace(const_iterator pos, Args&&... args);
        iterator insert(const_iterator pos, const value_type &val);
        iterator insert(const_iterator pos, size_type n, const value_type &val);
        iterator insert(const_iterator pos, value_type &&val);
        iterator erase(const_iterator pos);
        iterator erase(const_iterator first, const_iterator last);
        void swap(list &other) noexcept;
        void clear();

        /* Operations.  */
        void remove(const value_type &val);
        void unique();
        template<class BinaryPredicate>
        void unique(BinaryPredicate binary_pred);
        void sort();
        template<class Compare>
        void sort(Compare comp);
        void reverse() noexcept;

    private:
        template<typename Container>
        void _copy_from_container(const Container& other);
        void _push_empty(list_node *new_node);
        void _push_front(list_node *new_node);
        iterator _push_middle(list_node *where, list_node *new_node);
        void _push_back(list_node *new_node);
        void _pop_empty();
        void _remove_node(list_node *to_delete);
        template<class BinaryPredicate>
        void _unique(BinaryPredicate binary_pred);
        template<class Compare>
        void _merge_sort(list_node **head, Compare comp);
        void _split_to_halves(list_node *current, list_node **left_half_head, list_node **right_half_head);
        template<class Compare>
        list_node *_merge_sorted(list_node *left_list_head, list_node *right_list_head, Compare comp);
    };

    /* Implementation.  */

    /* Public member functions.  */
    template<typename T>
    list<T>::list() : _head(nullptr), _size(0) {
        _sentinel = new list_node();
    }

    template<typename T>
    list<T>::list(size_type n, const value_type &val) : _head(nullptr), _size(n) {
        list_node *new_node;

        _sentinel = new list_node();

        if (n != 0) {
            _head = new list_node(val);
            _sentinel->previous = _head;

            for (size_t i = 1; i < n; i++) {
                new_node = new list_node(val);
                _push_back(new_node);
            }

            _size = n;
        }
    }

    template<typename T>
    list<T>::list(const list<T> &other) : _head(nullptr), _sentinel(nullptr), _size(0) {
        if (!other.empty()) _copy_from_container(other);
    }

    template<typename T>
    list<T>::list(std::initializer_list<T> &il) : _head(nullptr), _sentinel(nullptr), _size(0) {
        if (il.size() != 0) _copy_from_container(il);
    }

    template<typename T>
    list<T>::list(list &&other) noexcept : list() {
        delete _sentinel;
        swap(*this, other);
    }

    template<typename T>
    list<T>::~list() {
        if (_head != nullptr) {
            clear();
            delete _sentinel;
        } else {
            if (_sentinel) delete _sentinel;
        }
    }

    template<typename T>
    list<T> &list<T>::operator=(list other) {
        /*Copy and swap idiom, let the compiler handle the copy of the argument*/
        swap(*this, other);

        return *this;
    }

    template<typename T>
    list_t::iterator list<T>::begin() noexcept {
        return iterator(_head != nullptr ? _head : _sentinel);
    }

    template<typename T>
    list_t::const_iterator list<T>::begin() const noexcept {
        return iterator(_head != nullptr ? _head : _sentinel);
    }

    template<typename T>
    list_t::iterator list<T>::end() noexcept {
        return iterator(_sentinel);
    }

    template<typename T>
    list_t::const_iterator list<T>::end() const noexcept {
        return const_cast<list<T>*>(this)->end();
    }

    template<typename T>
    list_t::reverse_iterator list<T>::rbegin() noexcept {
        return reverse_iterator(_sentinel->previous != nullptr ? _sentinel->previous : _sentinel);
    }

    template<typename T>
    list_t::const_reverse_iterator list<T>::rbegin() const noexcept {
        return const_reverse_iterator(_sentinel->previous != nullptr ? _sentinel->previous : _sentinel);
    }

    template<typename T>
    list_t::reverse_iterator list<T>::rend() noexcept {
        return reverse_iterator(_sentinel);
    }

    template<typename T>
    list_t::const_reverse_iterator list<T>::rend() const noexcept {
        return const_cast<list<T>*>(this)->rend();
    }

    template<typename T>
    list_t::const_iterator list<T>::cbegin() const noexcept {
        return const_cast<list<T>*>(this)->begin();
    }

    template<typename T>
    list_t::const_iterator list<T>::cend() const noexcept {
        return const_cast<list<T>*>(this)->end();
    }

    template<typename T>
    list_t::const_reverse_iterator list<T>::crbegin() const noexcept {
        return const_cast<list<T>*>(this)->rbegin();
    }

    template<typename T>
    list_t::const_reverse_iterator list<T>::crend() const noexcept {
        return const_cast<list<T>*>(this)->rend();
    }

    template<typename T>
    bool list<T>::empty() const noexcept {
        return _size == 0;
    }

    template<typename T>
    size_t list<T>::size() const noexcept {
        return _size;
    }

    template<typename T>
    list_t::reference list<T>::front() {
        return _head->data;
    }

    template<typename T>
    list_t::const_reference &list<T>::front() const {
        return _head->data;
    }

    template<typename T>
    list_t::reference list<T>::back() {
        return _sentinel->previous->data;
    }

    template<typename T>
    list_t::const_reference list<T>::back() const {
        return _sentinel->previous->data;
    }

    template<typename T>
    template<class... Args>
    void list<T>::emplace_front(Args &&... args) {
        _push_front(new list_node(std::forward<Args>(args)...));
    }

    template<typename T>
    void list<T>::push_front(const value_type &val) {
        _push_front(new list_node(val));
    }

    template<typename T>
    void list<T>::push_front(value_type &&val) {
        _push_front(new list_node(std::forward<value_type>(val)));
    }

    template<typename T>
    void list<T>::pop_front() {
        if (!empty()) {
            --_size;
            if (_size == 0) {
                _pop_empty();
            } else {
                _head = _head->next;
                if (_head) {
                    delete _head->previous;
                    _head->previous = _sentinel;
                }
            }
        }
    }

    template<typename T>
    template<class... Args>
    void list<T>::emplace_back(Args &&... args) {
        _push_back(new list_node(std::forward<Args>(args)...));
    }

    template<typename T>
    void list<T>::push_back(const T &val) {
        _push_back(new list_node(val));
    }

    template<typename T>
    void list<T>::push_back(value_type &&val) {
        _push_back(new list_node(std::forward<value_type>(val)));
    }

    template<typename T>
    void list<T>::pop_back() {
        if (!empty()) {
            --_size;
            if (_size == 0) {
                _pop_empty();
            }
            else {
                _sentinel->previous = _sentinel->previous->previous;
                delete _sentinel->previous->next;
                _sentinel->previous->next = _sentinel;
            }
        }
    }

    template<typename T>
    template<class... Args>
    list_t::iterator list<T>::emplace(const_iterator pos, Args &&... args) {
        return _push_middle(pos._it._ptr,new list_node(std::forward<Args>(args)...));
    }

    template<typename T>
    list_t::iterator list<T>::insert(const_iterator pos, const value_type &val) {
        return _push_middle(pos._it._ptr, new list_node(val));
    }

    template<typename T>
    list_t::iterator list<T>::insert(const_iterator pos, size_type n, const value_type &val) {
        iterator it;

        if (n > 0) {
            it = _push_middle(pos._it._ptr, new list_node(val));
            for (size_type i = 1 ; i < n ; i++) _push_middle(pos._it._ptr, new list_node(val));
        }

        return it;
    }

    template<typename T>
    list_t::iterator list<T>::insert(const_iterator pos, value_type &&val) {
        return _push_middle(pos._it._ptr, new list_node(std::forward<value_type>(val)));
    }

    template<typename T>
    list_t::iterator list<T>::erase(const_iterator pos) {
        auto it = pos._it;
        ++it;

        _remove_node(pos._it._ptr);
        return it;
    }

    template<typename T>
    list_t::iterator list<T>::erase(const_iterator first, const_iterator last) {
        while (first != last) {
            erase(first++);
        }

        /* TODO: check this.  */
        return last._it;
    }

    template<typename T>
    void list<T>::swap(list &other) noexcept {
        using std::swap;
        swap(_head, other._head);
        swap(_sentinel, other._sentinel);
        swap(_size, other._size);
    }

    template<typename T>
    void list<T>::clear() {
        list_node *current, *to_delete;

        current = _head;
        while (current != _sentinel) {
            to_delete = current;
            current = current->next;
            to_delete->previous = nullptr;
            to_delete->next = nullptr;
            delete to_delete;
        }
        _head = _sentinel;
        _head->previous = nullptr;
        _size = 0;
    }

    template<typename T>
    void list<T>::remove(const value_type &val) {
        list_node *current = _head, *save;

        while (current != _sentinel) {
            if (current->data == val) {
                break;
            }
            current = current->next;
        }
        while (current && current->data == val) {
            save = current;
            current = current->next;
            _remove_node(save);
        }
    }

    template<typename T>
    void list<T>::unique() {
        _unique(default_comp_unique());
    }

    template<typename T>
    template <class BinaryPredicate>
    void list<T>::unique(BinaryPredicate binary_pred) {
        _unique(binary_pred);
    }

    template<typename T>
    void list<T>::sort() {
        if (!empty()) {
            _merge_sort<default_comp_sort>(&_head, default_comp_sort());
        }
    }

    template<typename T>
    template<class Compare>
    void list<T>::sort(Compare comp) {
        if (!empty()) {
            _merge_sort<Compare>(&_head, comp);
        }
    }

    template<typename T>
    void list<T>::reverse() noexcept {
        list_node *prev, *current = _head, *cached_head = _head;
        /* Temporarily pretend we dont have a sentinel node.  */
        _sentinel->previous->next = nullptr;
        _head->previous = nullptr;

        while (current != nullptr) {
            prev = current->previous;
            current->previous = current->next;
            current->next = prev;
            current = current->previous;
        }

        if (prev != nullptr) {
            _head = prev->previous;
        }
        _head->previous = _sentinel;
        cached_head->next = _sentinel;
        _sentinel->previous = cached_head;
    }

    /* Private member functions.  */
    template<typename T>
    template<typename Container>
    void list<T>::_copy_from_container(const Container &other) {

        _head = *(other.begin());
        _sentinel = new list_node();
        _sentinel->previous = _head;
        _head->previous = _sentinel;

        iterator it = ++(other.begin());
        iterator end = other.end();

        for (; it != end ; it++) {
            list_node *new_node(new list_node(*it));
            _sentinel->previous->next = new_node;
            new_node->previous = _sentinel->previous;
            _sentinel->previous = new_node;
            new_node->next = _sentinel;
        }
        _size = other._size;
    }

    template<typename T>
    void list<T>::_push_empty(list_node *new_node) {
        _head = new_node;
        _sentinel->previous = _head;
        _head->previous = _sentinel;
        _head->next = _sentinel;
        _size = 1;
    }

    template<typename T>
    void list<T>::_push_front(list_node *new_node) {
        if (empty()) {
            _push_empty(new_node);
        }
        else {
            auto previous_head = _head;
            _head = new_node;
            _head->next = previous_head;
            previous_head->previous = _head;
            ++_size;
        }
    }

    template<typename T>
    list_t::iterator list<T>::_push_middle(list_node *where, list_node *new_node) {
        list_node *pos;

        if (where == _head) {
            _push_front(new_node);
            pos = _head;
        }
        else if (where == _sentinel) {
            _push_back(new_node);
            pos = _sentinel->previous;
        }
        else {
            new_node->next = where;
            new_node->previous = where->previous;
            where->previous->next = new_node;
            where->previous = new_node;
            ++_size;
            pos = new_node;
        }

        return iterator(pos);
    }

    template<typename T>
    void list<T>::_push_back(list_node *new_node) {
        if (empty()) {
            _push_empty(new_node);
        }
        else {
            _sentinel->previous->next = new_node;
            new_node->previous = _sentinel->previous;
            _sentinel->previous = new_node;
            new_node->next = _sentinel;
            ++_size;
        }
    }

    template<typename T>
    void list<T>::_pop_empty() {
        delete _head;
        _head = _sentinel;
        _sentinel->next = nullptr;
        _sentinel->previous = nullptr;
    }

    template<typename T>
    void list<T>::_remove_node(list_node *node) {
        if (node != _sentinel) {
            if (node == _sentinel->previous) {
                pop_back();
            }
            else if (node == _head) {
                pop_front();
            }
            else {
                node->previous->next = node->next;
                node->next->previous = node->previous;
                delete node;
                --_size;
            }
        }

        if (_size == 0) {
            _head = _sentinel;
            _sentinel->previous = nullptr;
        }
    }

    template<typename T>
    template<class BinaryPredicate>
    void list<T>::_unique(BinaryPredicate binary_pred) {
        list_node *candidate, *current = _head;

        while (current->next != _sentinel) {
            candidate = current;
            current = current->next;
            if (binary_pred(candidate->data, current->data)) {
                _remove_node(candidate);
            }
        }
    }

    template<typename T>
    template<class Compare>
    void list<T>::_merge_sort(list_node **head, Compare comp) {
        list_node *left_half, *right_half;

        if (*head != _sentinel && (*head)->next != _sentinel) {
            _split_to_halves(*head, &left_half, &right_half);

            _merge_sort(&left_half, comp);
            _merge_sort(&right_half, comp);

            *head = _merge_sorted(left_half, right_half, comp);
        }
    }

    /* Splits the list into 2 sub lists.
     * Left sublist is always >= right sublist.  */
    template<typename T>
    void list<T>::_split_to_halves(list_node *current, list_node **left_half_head, list_node **right_half_head) {
        list_node *fast_ptr, *slow_ptr;

        slow_ptr = current;
        fast_ptr = current->next;

        while (fast_ptr != _sentinel) {
            fast_ptr = fast_ptr->next;
            if (fast_ptr != _sentinel) {
                slow_ptr = slow_ptr->next;
                fast_ptr = fast_ptr->next;
            }
        }

        *left_half_head = current;
        *right_half_head = slow_ptr->next;
        slow_ptr->next = _sentinel;
    }

    /* Merges 2 lists and returns the new head.  */
    template<typename T>
    template<class Compare>
    list_t::list_node *list<T>::_merge_sorted(list_node *left_list_head, list_node *right_list_head, Compare comp) {
        list_node *sorted_list_head, *sorted_current, *left_current, *right_current, *remaining;

        left_current = left_list_head;
        right_current = right_list_head;
        if (right_list_head == _sentinel) {
            sorted_list_head = left_list_head;
        } else {
            if (comp(left_current->data, right_current->data)) {
                sorted_list_head = left_current;
                left_current = left_current->next;
            } else {
                sorted_list_head = right_current;
                right_current = right_current->next;
            }
        }

        /* Connect nodes in a sorted way.  */
        sorted_current = sorted_list_head;
        while (left_current != _sentinel && right_current != _sentinel) {
            if (comp(left_current->data, right_current->data)) {
                sorted_current->next = left_current;
                left_current->previous = sorted_current;
                left_current = left_current->next;
            } else {
                sorted_current->next = right_current;
                right_current->previous = sorted_current;
                right_current = right_current->next;
            }
            sorted_current = sorted_current->next;
        }

        /* Connect remaining nodes.  */
        remaining = left_current != _sentinel ? left_current : right_current;

        if (right_current == _sentinel) {
            list_node *current = remaining;
            while (current->next != _sentinel) current = current->next;
            current->next = _sentinel;
            _sentinel->previous = current;
        }

        sorted_current->next = remaining;
        /* Connect new head to the last node of the list.  */
        sorted_list_head->previous = _sentinel;
        remaining->previous = sorted_current;

        return sorted_list_head;
    }
}
