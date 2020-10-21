#pragma once

#include <memory>
#include <iostream>
#include <initializer_list>

namespace adt {

    template<typename T>
    class list {
    private:

        class Node {
        public:

            friend class list<T>;
            friend class list_iterator;

            T data;
            std::shared_ptr<Node> next;
            std::shared_ptr<Node> previous;

            Node() noexcept : data(0), next(nullptr), previous(nullptr) {}
            explicit Node(const T &val) noexcept : data(val), next(nullptr), previous(nullptr) {}
            explicit Node(T&& val) noexcept : data(std::forward<T>(val)), next(nullptr), previous(nullptr)  {}
            explicit Node(const Node& node) = default;
            explicit Node(Node&& node) = default;

            template<typename... Args>
            Node(Args&&... args) noexcept : data(std::forward<Args>(args)...), next(nullptr), previous(nullptr) {}

            Node& operator=(Node&& node) = default;
        };

        std::shared_ptr<Node> _head;
        std::shared_ptr<Node> _sentinel;
        size_t _size;

        template<typename Container>
        void _copy_from_container(const Container& other);
        void _push_empty(std::shared_ptr<Node>& new_node);
        void _pop_empty();
        void _remove_node(Node* node);
        void _push_front(std::shared_ptr<Node>&& new_node);
        void _push_back(std::shared_ptr<Node>&& new_node);
    public:
        class list_iterator : public std::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t , T*, T&> {
            friend class list<T>;
            friend class list_r_iterator;
        public:
            using data_reference = T&;
            using data_pointer = T*;
            using const_data_reference = const T&;
            using const_data_pointer = const T*; 

            list_iterator(Node *_ptr = nullptr) noexcept : ptr(_ptr) {}
            list_iterator(const list_iterator& other) = default;
        
            list_iterator& operator=(const list_iterator& other) = default;
            list_iterator& operator=(Node* _ptr) {
                this->ptr = _ptr;
                return *this;
            }

            bool operator==(const list_iterator& rhs) const {
                return this->ptr == rhs.ptr;
            }
            bool operator!=(const list_iterator& rhs) const {
                return !(*this == rhs);
            }        
            
            /* Prefix ++ operator. */
            list_iterator& operator++() {
                if (ptr) {
                    ptr = ptr->next.get();
                }
                return *this;     
            }

            /* Postfix ++ operator. */
            list_iterator operator++(int) {
                list_iterator tmp(*this);
                ++(*this);

                return tmp;
            }

            /* Prefix -- operator. */
            list_iterator& operator--() {
                if (ptr) {
                    ptr = ptr->previous.get();
                }
                return *this;
            }

            /* Postfix -- operator. */
            list_iterator operator--(int) {
                list_iterator tmp(*this);
                --(*this);

                return tmp;
            }

            const_data_reference operator*() {
                return ptr->data;
            }

            const_data_pointer operator->() {
                return &(ptr->data);
            }

            const_data_reference operator*() const {
                return ptr->data;
            }

            const_data_pointer operator->() const {
                return &(ptr->data);
            }
        protected:
            Node *ptr;
        };        

        class list_r_iterator : public list_iterator {
            friend class list<T>;
        public:
            list_r_iterator(Node *_ptr = nullptr) noexcept : list_iterator(_ptr) {}
            list_r_iterator(const list_r_iterator& other) {
                this->ptr = other.ptr;
            }
        
            list_r_iterator& operator=(const list_r_iterator& other) = default;
            list_r_iterator& operator=(Node* _ptr) {
                this->ptr = _ptr;
                return *this;
            }    
            
            /* Prefix ++ operator. */
            list_r_iterator& operator++() {
                if (this->ptr) {
                    this->ptr = this->ptr->previous.get();
                }
                return *this;     
            }

            /* Postfix ++ operator. */
            list_r_iterator operator++(int) {
                list_r_iterator tmp(*this);
                ++(*this);

                return tmp;
            }

            /* Prefix -- operator. */
            list_r_iterator& operator--() {
                if (this->ptr) {
                    this->ptr = this->ptr->next.get();
                }
                return *this;
            }

            /* Postfix -- operator. */
            list_r_iterator operator--(int) {
                list_r_iterator tmp(*this);
                --(*this);

                return tmp;
            }
        };


        list();
        list(size_t n, const T& val);
        list(size_t n, T&& val);
        explicit list(const list& other);
        explicit list(list&& other) noexcept;
        explicit list(std::initializer_list<T>& il);
        ~list();

        list& operator=(list other);

        /* Modifiers. */
        void push_back(const T& val);
        void push_back(T&& val);
        void push_front(const T& val);
        void push_front(T&& val);
        void pop_back();
        void pop_front();
        /* Remove all the elements that compare equal to val. */
        void remove(const T& val);
        void remove(T&& val);
        /* Remove the element pointed to by this iterator. */
        list_iterator remove(list_iterator& ite);
        void clear();
        
        /* Element access. */
        const T& back() const;
        T& back();
        const T& front() const;
        T& front();
        list_iterator begin();
        list_iterator end();
        list_r_iterator rbegin();
        list_r_iterator rend();

        /* Operations. 
           TODO: add me. */

        /* Capacity. */
        bool empty() const noexcept;
        size_t size() const noexcept;

        void swap(list& other) noexcept;
    };

    template<typename T>
    list<T>::list() : _head(nullptr),  _sentinel(nullptr), _size(0) {}

    template<typename T>
    list<T>::list(size_t n, const T& val) : _head(nullptr), _sentinel(nullptr), _size(0) {

        if (n == 0) {
            return;
        }

        _head = std::make_shared<Node>(val);
        _sentinel = std::make_shared<Node>();
        _sentinel->previous = _head;

        for (size_t i = 1 ; i < n ; i++) {
            std::shared_ptr<Node> new_node(new Node(val));
            _sentinel->previous->next = new_node;
            new_node->previous = _sentinel->previous;
            _sentinel->previous = new_node;
            new_node->next = _sentinel;
        }
        _size = n;
    }

    template<typename T>
    list<T>::list(size_t n, T&& val) : _head(nullptr), _sentinel(nullptr), _size(0) {
        
        if (n == 0) {
            return;
        }

        _head = std::make_shared<Node>(std::forward<T>(val));
        _sentinel = std::make_shared<Node>();
        _sentinel->previous = _head;

        for (size_t i = 1 ; i < n ; i++) {
            std::shared_ptr<Node> new_node(new Node(std::forward<T>(val)));
            _sentinel->previous->next = new_node;
            new_node->previous = _sentinel->previous;
            _sentinel->previous = new_node;
            new_node->next = _sentinel;
        }
        _size = n;
    }

    template<typename T>
    template<typename Container>
    void list<T>::_copy_from_container(const Container& other) {
        
        _head = std::make_shared<Node>(*(other.begin()));    
        _sentinel = std::make_shared<Node>();                        
        _sentinel->previous = _head;                                    
                                                                        
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
    list<T>::list(const list<T>& other) : _head(nullptr), _sentinel(nullptr), _size(0) {

        if (!other.empty()) {
            _copy_from_container(other);
        }
    }

    template<typename T>
    list<T>::list(std::initializer_list<T>& il) : _head(nullptr), _sentinel(nullptr), _size(0) {

        if (il.size() != 0) {
            _copy_from_container(il);
        }
    }

    template<typename T>
    list<T>::list(list&& other) noexcept : list() {
        swap(*this, other);
    }

    template<typename T>
    list<T>::~list() {
        if (_head.get()) {
            clear();
        }
        _sentinel = nullptr;
    }

    template<typename T>
    list<T>& list<T>::operator=(list other) {
        /*Copy and swap idiom, let the compiler handle the copy of the argument*/
        swap(*this, other);

        return *this;
    }

    template<typename T>
    void list<T>::_push_empty(std::shared_ptr<Node>& new_node) {
        
        if (_sentinel.get() == nullptr)
            /*Allocate a sentinel only if this is the very first call of push_* */
            _sentinel = std::make_shared<Node>();    
        
        _head = new_node;               
        _sentinel->previous = _head;                
        _head->next = _sentinel;                    
        _size = 1; 
    }

    template<typename T>
    void list<T>::_push_back(std::shared_ptr<Node>&& new_node) {

        if (empty()) {
            _push_empty(new_node);                                 
        }
        else {
            _sentinel->previous->next = new_node;
            new_node->previous = _sentinel->previous;
            _sentinel->previous = new_node;
            new_node->next = _sentinel;
            _size++;
        }
    }

    template<typename T>
    void list<T>::push_back(const T &val) {
        _push_back(std::move(std::make_shared<Node>(val)));
    }

    template<typename T>
    void list<T>::push_back(T&& val) {
        _push_back(std::move(std::make_shared<Node>(std::forward<T>(val))));
    }

    template<typename T>
    void list<T>::_push_front(std::shared_ptr<Node>&& new_node) {

        if (empty()) {
            _push_empty(new_node);
        }
        else {
            auto previous_head = _head;
            _head = new_node;
            _head->next = previous_head;
            previous_head->previous = _head;
            _size++;
        }
    }

    template<typename T>
    void list<T>::push_front(const T& val) {
        _push_front(std::move(std::make_shared<Node>(val)));
    }

    template<typename T>
    void list<T>::push_front(T&& val) {
        _push_front(std::move(std::make_shared<Node>(std::forward<T>(val))));
    }

    template<typename T>
    void list<T>::_pop_empty() {
        _head->next = nullptr;        
        _head = nullptr;              
        _sentinel->previous = nullptr;
    }

    template<typename T>
    void list<T>::pop_back() {

        if (!empty()) {
            --_size;
            if (_size == 0)
                _pop_empty();
            else {
                _sentinel->previous = _sentinel->previous->previous;
                _sentinel->previous->next = _sentinel;
            }
        }
    }

    template<typename T>
    void list<T>::pop_front() {
        
        if (!empty()) {
            --_size;
            if (_size == 0)
                _pop_empty();
            else {
                _head = _head->next;
                if (_head.get()) {
                    _head->previous = nullptr;
                }
            }
        }
    }

    template<typename T>
    void list<T>::_remove_node(Node* node) {

        if (node != _sentinel.get()) {
            /*This seems ugly, prolly need to fix it*/
            if (node == _sentinel->previous.get()) {
                pop_back();
            }
            else if (node == _head.get()) {
                pop_front();
            }
            else {
                node->previous->next = node->next;
                node->next->previous = node->previous;
                node->previous = nullptr;
                node->next = nullptr;
                _size--;
            }
        }
    }

    template<typename T>
    void list<T>::remove(const T& val) {
        Node* current = _head.get();
        Node* save;
        Node* end = _sentinel.get();

        while (current != end) {
            if (current->data == val) {
                break;   
            } 
            current = current->next.get();
        }
        while (current && current->data == val) {
            save = current;
            current = current->next.get();
            _remove_node(save);
        }
    }

    template<typename T>
    void list<T>::remove(T&& val) {
        Node* current = _head.get();
        Node* save;
        Node* end = _sentinel.get();

        while (current != end) {
            if (current->data == val) {
                break;   
            } 
            current = current->next.get();
        }
        while (current->data == val) {
            save = current;
            current = current->next.get();
            _remove_node(save);
        }
    }

    template<typename T>
    typename list<T>::list_iterator list<T>::remove(list_iterator& ite) {
        _remove_node(ite.ptr);

        if (_size == 0) {
            return end();
        }
        else {
            return list_iterator(_head->next.get());
        }
    }

    template<typename T>
    void list<T>::clear() {
        auto current = _head;
        auto end = _sentinel;
        
        while (current->next != end) {
            auto temp = current->next->next;
            current->next->next = nullptr;
            current->next->previous = nullptr;
            current->next = temp;
        }
        _pop_empty();

        _size = 0;
    }

    template<typename T>
    const T& list<T>::back() const {
        return _sentinel->previous->data;
    }

    template<typename T>
    T& list<T>::back() {
        return _sentinel->previous->data;
    }

    template<typename T>
    const T& list<T>::front() const {
        return _head->data;
    }

    template<typename T>
    T& list<T>::front() {
        return _head->data;
    }

    template<typename T>
    typename list<T>::list_iterator list<T>::begin() {
        if (_head.get()) 
            return list_iterator(_head.get());
        else 
            return list_iterator(_sentinel.get());
    }

    template<typename T>
    typename list<T>::list_iterator list<T>::end() {
        return list_iterator(_sentinel.get());
    }

    /* Really not sure about these 2. */
    template<typename T>
    typename list<T>::list_r_iterator list<T>::rbegin() {
        if (_sentinel != nullptr) {
            return list_r_iterator(_sentinel->previous.get());
        }
        else {
            return list_r_iterator(nullptr);
        }
    }

    template<typename T>
    typename list<T>::list_r_iterator list<T>::rend() {
        return list_r_iterator(nullptr);
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
    void list<T>::swap(list& other) noexcept {
        using std::swap;
        swap(_head, other._head);
        swap(_sentinel, other._sentinel);
        swap(_size, other._size);
    }
}