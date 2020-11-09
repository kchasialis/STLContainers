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

            list_node() noexcept : data(0), next(nullptr), previous(nullptr) {}
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

        list_node *_head;
        list_node *_sentinel;
        size_t _size{};

    public:
        class iterator {
            friend class list;

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
            iterator &operator=(list_node *ptr) {
                this->_ptr = ptr;
                return *this;
            }

            bool operator==(const iterator &rhs) const { return this->_ptr == rhs._ptr; }
            bool operator==(list_node *ptr) const { return this->_ptr == ptr; }
            bool operator!=(const iterator &rhs) const { return !(*this == rhs); }
            bool operator!=(list_node *ptr) const { return !(*this == ptr); }
            
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
            list_node *_ptr;

            iterator(list_node *ptr = nullptr) : _ptr(ptr) {}
        };

        class const_iterator {
            friend class list;

        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = list::value_type;
            using reference = list::const_reference;
            using pointer = list::const_pointer;
            using difference_type = list::difference_type;

            /* Implicit conversion from iterator.  */
            const_iterator(iterator it) : _it(std::move(it)) {}

            bool operator==(const const_iterator &other) const { return this->_it == other._it; }
            bool operator==(list_node *ptr) const { return _it == ptr; }
            bool operator!=(const const_iterator &other) const { return !(*this == other); }
            bool operator!=(list_node *ptr) const { return !(*this == ptr); }

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

            const_iterator(list_node *ptr) : _it(ptr) {};
        };

        class reverse_iterator {
            friend class list;

        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = list::value_type;
            using reference = list::reference;
            using pointer = list::pointer;
            using difference_type = list::difference_type;

            /* Implicit conversion from iterator.  */
            reverse_iterator(iterator it) : _it(std::move(it)) {}

            bool operator==(const reverse_iterator &other) const { return this->_it == other._it; }
            bool operator==(list_node *ptr) const { return _it == ptr; }
            bool operator!=(const reverse_iterator &other) const { return !(*this == other); }
            bool operator!=(list_node *ptr) const { return !(*this == ptr); }

            reference operator*() const { return *_it; }
            pointer operator->() const { return _it.operator->(); }

            reverse_iterator &operator++() {
                --_it;
                return *this;
            }
            reverse_iterator operator++(int) { return _it--; }
            reverse_iterator &operator--() {
                ++_it;
                return *this;
            }
            reverse_iterator operator--(int) { return _it++; }

        private:
            iterator _it;

            reverse_iterator(list_node *ptr) : _it(ptr) {};
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
            const_reverse_iterator(iterator it) : _it(std::move(it)) {}

            bool operator==(const const_reverse_iterator &other) const { return this->_it == other._it; }
            bool operator==(list_node *ptr) const { return _it == ptr; }
            bool operator!=(const const_reverse_iterator &other) const { return !(*this == other); }
            bool operator!=(list_node *ptr) const { return !(*this == ptr); }

            reference operator*() const { return *_it; }
            pointer operator->() const { return _it.operator->(); }

            const_reverse_iterator &operator++() {
                --_it;
                return *this;
            }
            const_reverse_iterator operator++(int) { return _it--; }
            const_reverse_iterator &operator--() {
                ++_it;
                return *this;
            }
            const_reverse_iterator operator--(int) { return _it++; }

        private:
            iterator _it;

            const_reverse_iterator(list_node *ptr) : _it(ptr) {};
        };

        /* Constructors/Destructors.  */
        list();
        list(size_t n, const value_type &val);
        list(size_t n, value_type &&val);
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
        template<typename V>
        void _fill_list(size_type n, V val);
        template<typename Container>
        void _copy_from_container(const Container& other);
        void _push_empty(list_node *new_node);
        void _push_front(list_node *new_node);
        void _push_middle(list_node *where, list_node *new_node);
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
        _fill_list<const value_type&>(val);
    }

    template<typename T>
    list<T>::list(size_type n, value_type &&val) : _head(nullptr), _size(n) {
        _fill_list<value_type&&>(std::forward<value_type>(val));
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
        return const_iterator(_sentinel);
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
        return const_reverse_iterator(_sentinel);
    }

    template<typename T>
    list_t::const_iterator list<T>::cbegin() const noexcept {
        return const_iterator(_head);
    }

    template<typename T>
    list_t::const_iterator list<T>::cend() const noexcept {
        return const_iterator(_sentinel);
    }

    template<typename T>
    list_t::const_reverse_iterator list<T>::crbegin() const noexcept {
        return const_reverse_iterator(_sentinel->previous != nullptr ? _sentinel->previous : _sentinel);
    }

    template<typename T>
    list_t::const_reverse_iterator list<T>::crend() const noexcept {
        return const_reverse_iterator(_sentinel);
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
                if (_head) _head->previous = nullptr;
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
            it = _push_middle(new list_node(val));
            for (size_type i = 1 ; i < n ; i++) _push_middle(new list_node(val));
        }

        return it;
    }

    template<typename T>
    list_t::iterator list<T>::insert(const_iterator pos, value_type &&val) {
        return _push_middle(pos._it._ptr, new list_node(std::forward<value_type>(val)));
    }

    template<typename T>
    list_t::iterator list<T>::erase(const_iterator pos) {
        auto it = ++pos._it;

        _remove_node(pos._it_.ptr);
        return it;
    }

    template<typename T>
    list_t::iterator list<T>::erase(const_iterator first, const_iterator last) {
        auto it = first;

        while (it != last) it = erase(it);

        return it;
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
        _size = 0;
    }

    template<typename T>
    void list<T>::remove(const value_type &val) {
        list_node *current = _head, save, end = _sentinel;

        while (current != end) {
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
            _head->previous = _sentinel->previous;
            _sentinel->previous->next = nullptr;
            _merge_sort(default_comp_sort());
            _head->previous->next = _sentinel;
            _head->previous = _sentinel;
        }
    }

    template<typename T>
    template<class Compare>
    void list<T>::sort(Compare comp) {
        if (!empty()) {
            _head->previous = _sentinel->previous;
            _sentinel->previous->next = nullptr;
            _merge_sort(default_comp_sort());
            _head->previous->next = _sentinel;
            _head->previous = _sentinel;
        }
    }

    template<typename T>
    void list<T>::reverse() noexcept {
        list_node *current = _head, *prev = _sentinel, *next;

        while (current != _sentinel) {
            next = current->next;
            current->next = prev;

            prev = current;
            current = next;
        }
        _head = prev;
    }

    /* Private member functions.  */
    template<typename T>
    template<typename V>
    void list<T>::_fill_list(list::size_type n, V val) {
        list_node *new_node;

        _sentinel = new list_node();

        if (n == 0) {
            return;
        }

        _head = new list_node(std::forward<V>(val));
        _sentinel->next = _head;
        _sentinel->previous = _head;

        for (size_t i = 1 ; i < n ; i++) {
            new_node = new list_node(std::forward<V>(val));
            _push_back(new_node);
        }
    }

    template<typename T>
    template<typename Container>
    void list<T>::_copy_from_container(const Container &other) {

        _head = *(other.begin());
        _sentinel = new list_node();
        _sentinel->previous = _head;
        _head->previous = _sentinel;

        list_iterator it = ++(other.begin());
        list_iterator end = other.end();

        for (; it != end ; it++) {
            std::shared_ptr<Node> new_node(new Node(*it));
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
    void list<T>::_push_middle(list_node *where, list_node *new_node) {
        if (where == _head) {
            _push_front(new_node);
        }
        else if (where == _sentinel) {
            _push_back(new_node);
        }
        else {
            new_node->next = where;
            new_node->previous = where->previous;
            where->previous->next = new_node;
            where->previous = new_node;
            ++_size;
        }
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

        if (*head != nullptr && (*head)->next != nullptr) {
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

        while (fast_ptr != nullptr) {
            fast_ptr = fast_ptr->next;
            if (fast_ptr != nullptr) {
                slow_ptr = slow_ptr->next;
                fast_ptr = fast_ptr->next;
            }
        }

        *left_half_head = current;
        *right_half_head = slow_ptr->next;
        slow_ptr->next = nullptr;
    }

    /* Merges 2 lists and returns the new head.  */
    template<typename T>
    template<class Compare>
    list_t::list_node *list<T>::_merge_sorted(list_node *left_list_head, list_node *right_list_head, Compare comp) {
        list_node *sorted_list_head, *sorted_current, *left_current, *right_current, *remaining;

        left_current = left_list_head;
        right_current = right_list_head;
        if (right_list_head == nullptr) {
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
        while (left_current != nullptr && right_current != nullptr) {
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
        remaining = left_current ? left_current : right_current;
        sorted_current->next = remaining;
        /* Connect new head to the last node of the list.  */
        sorted_list_head->previous = remaining->previous;
        remaining->previous = sorted_current;

        return sorted_list_head;
    }
}
