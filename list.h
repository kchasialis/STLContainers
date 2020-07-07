#ifndef LIST_H
#define LIST_H

#include <memory>
#include <iostream>
#include <initializer_list>

namespace adt {
    
    template<typename T>
    struct Node {
        T data;
        std::shared_ptr<Node<T>> next;
        std::shared_ptr<Node<T>> previous;

        Node() noexcept;
        explicit Node(const T& val) noexcept;
        explicit Node(T&& val) noexcept;
        explicit Node(const Node& node) = default;
        explicit Node(Node&& node) = default;

        template<typename... Args>
        Node(Args&&... args) noexcept;

        Node& operator=(Node&& node) = default;
    };

    template<typename T>
    class list_iterator : public std::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t , T*, T&> {
    public:
        using data_reference = T&;
        using data_pointer = T*;
        using const_data_reference = const T&;
        using const_data_pointer = const T*; 

        list_iterator(Node<T> *_ptr = nullptr) noexcept : ptr(_ptr) {}
        list_iterator(const list_iterator<T>& other) = default;
       
        list_iterator<T>& operator=(const list_iterator<T>& other) = default;
        list_iterator<T>& operator=(Node<T>* _ptr) {
            this->ptr = _ptr;
            return *this;
        }

        bool operator==(const list_iterator<T>& rhs) const {
            return this->ptr == rhs.ptr;
        }
        bool operator!=(const list_iterator<T>& rhs) const {
            return !(*this == rhs);
        }        
        
        /* Prefix ++ operator. */
        list_iterator<T>& operator++() {
            if (ptr) {
                ptr = ptr->next.get();
            }
            return *this;     
        }

        /* Postfix ++ operator. */
        list_iterator<T> operator++(int) {
            list_iterator<T> tmp(*this);
            ++(*this);

            return tmp;
        }

        /* Prefix -- operator. */
        list_iterator<T>& operator--() {
            if (ptr) {
                ptr = ptr->previous.get();
            }
            return *this;
        }

        /* Postfix -- operator. */
        list_iterator<T> operator--(int) {
            list_iterator<T> tmp(*this);
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

        Node<T>* get_ptr() {
            return ptr;
        }

        const Node<T>* get_ptr() const {
            return ptr;
        }
    protected:
        Node<T> *ptr;
    };

    template<typename T>
    class list_r_iterator : public list_iterator<T> {
    public:
        list_r_iterator(Node<T> *_ptr = nullptr) noexcept : list_iterator<T>(_ptr) {}
        list_r_iterator(const list_r_iterator<T>& other) {
            this->ptr = other.ptr;
        }
       
        list_r_iterator<T>& operator=(const list_r_iterator<T>& other) = default;
        list_r_iterator<T>& operator=(Node<T>* _ptr) {
            this->ptr = _ptr;
            return *this;
        }    
        
        /* Prefix ++ operator. */
        list_r_iterator<T>& operator++() {
            if (this->ptr) {
                this->ptr = this->ptr->previous.get();
            }
            return *this;     
        }

        /* Postfix ++ operator. */
        list_r_iterator<T> operator++(int) {
            list_r_iterator<T> tmp(*this);
            ++(*this);

            return tmp;
        }

        /* Prefix -- operator. */
        list_r_iterator<T>& operator--() {
            if (this->ptr) {
                this->ptr = this->ptr->next.get();
            }
            return *this;
        }

        /* Postfix -- operator. */
        list_r_iterator<T> operator--(int) {
            list_r_iterator<T> tmp(*this);
            --(*this);

            return tmp;
        }
    };

    
    template<typename T>
    class list {
    private:
        std::shared_ptr<Node<T>> _head;
        std::shared_ptr<Node<T>> _sentinel;
        size_t _size;

        template<typename Container>
        void _copy_from_container(const Container& other);
        void _push_empty(std::shared_ptr<Node<T>>& new_node);
        void _pop_empty();
        void _remove_node(Node<T>* node);
        void _push_front(std::shared_ptr<Node<T>>&& new_node);
        void _push_back(std::shared_ptr<Node<T>>&& new_node);
    public:
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
        /* These functions remove all the elements that compare equal to val. */
        void remove(const T& val);
        void remove(T&& val);
        /* This function removes the element pointed to by this iterator. */
        list_iterator<T> remove(list_iterator<T>& ite);
        void clear();
        
        /* Element access. */
        const T& back() const;
        T& back();
        const T& front() const;
        T& front();
        list_iterator<T> begin();
        list_iterator<T> end();
        list_r_iterator<T> rbegin();
        list_r_iterator<T> rend();
        list_iterator<T> search(const T& val);
        list_iterator<T> search(T&& val);

        /* Operations. 
           TODO: add me*/

        /* Capacity. */
        bool empty() const noexcept;
        size_t size() const noexcept;

        void swap(list& other) noexcept;
    };

    template<typename T>
    template<typename... Args>
    Node<T>::Node(Args &&... args) noexcept : data(std::forward<Args>(args)...), next(nullptr), previous(nullptr) {}

    template<typename T>
    Node<T>::Node() noexcept : data(0), next(nullptr), previous(nullptr) {}

    template<typename T>
    Node<T>::Node(T&& val) noexcept : data(std::forward<T>(val)), next(nullptr), previous(nullptr)  {}

    template<typename T>
    Node<T>::Node(const T &val) noexcept : data(val), next(nullptr), previous(nullptr) {}

    template<typename T>
    list<T>::list() : _head(nullptr),  _sentinel(nullptr), _size(0) {}

    template<typename T>
    list<T>::list(size_t n, const T& val) : _head(nullptr), _sentinel(nullptr), _size(0) {

        if (n == 0) {
            return;
        }

        _head = std::make_shared<Node<T>>(val);
        _sentinel = std::make_shared<Node<T>>();
        _sentinel->previous = _head;

        for (size_t i = 1 ; i < n ; i++) {
            std::shared_ptr<Node<T>> new_node(new Node<T>(val));
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

        _head = std::make_shared<Node<T>>(std::forward<T>(val));
        _sentinel = std::make_shared<Node<T>>();
        _sentinel->previous = _head;

        for (size_t i = 1 ; i < n ; i++) {
            std::shared_ptr<Node<T>> new_node(new Node<T>(std::forward<T>(val)));
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
        
        _head = std::make_shared<Node<T>>(*(other.begin()));    
        _sentinel = std::make_shared<Node<T>>();                        
        _sentinel->previous = _head;                                    
                                                                        
        list_iterator<T> it = ++(other.begin());                 
        list_iterator<T> end = other.end();                      
                                                                        
        for (; it != end ; it++) {                                      
            std::shared_ptr<Node<T>> new_node(new Node<T>(*it));        
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
    void list<T>::_push_empty(std::shared_ptr<Node<T>>& new_node) {
        
        if (_sentinel.get() == nullptr)
            /*Allocate a sentinel only if this is the very first call of push_* */
            _sentinel = std::make_shared<Node<T>>();    
        
        _head = new_node;               
        _sentinel->previous = _head;                
        _head->next = _sentinel;                    
        _size = 1; 
    }

    template<typename T>
    void list<T>::_push_back(std::shared_ptr<Node<T>>&& new_node) {

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
        _push_back(std::make_shared<Node<T>>(val));
    }

    template<typename T>
    void list<T>::push_back(T&& val) {
        _push_back(std::make_shared<Node<T>>(std::forward<T>(val)));
    }

    template<typename T>
    void list<T>::_push_front(std::shared_ptr<Node<T>>&& new_node) {

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
        _push_front(std::make_shared<Node<T>>(val));
    }

    template<typename T>
    void list<T>::push_front(T&& val) {
        _push_front(std::make_shared<Node<T>>(std::forward<T>(val)));
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
    void list<T>::_remove_node(Node<T>* node) {

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
                node = nullptr;
                _size--;
            }
        }
    }

    template<typename T>
    void list<T>::remove(const T& val) {
        _remove_node(search(val).get_ptr());
    }

    template<typename T>
    void list<T>::remove(T&& val) {
        _remove_node(search(val).get_ptr());
    }

    template<typename T>
    list_iterator<T> list<T>::remove(list_iterator<T>& ite) {
        _remove_node(ite.get_ptr());

        if (_size == 0) {
            return end();
        }
        else {
            return list_iterator<T>(_head->next.get());
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
    list_iterator<T> list<T>::begin() {
        if (_head.get()) 
            return list_iterator<T>(_head.get());
        else 
            return list_iterator<T>(_sentinel.get());
    }

    template<typename T>
    list_iterator<T> list<T>::end() {
        return list_iterator<T>(_sentinel.get());
    }

    /* Really not sure about these 2. */
    template<typename T>
    list_r_iterator<T> list<T>::rbegin() {
        if (_sentinel != nullptr) {
            return list_r_iterator<T>(_sentinel->previous.get());
        }
        else {
            return list_r_iterator<T>(nullptr);
        }
    }

    template<typename T>
    list_r_iterator<T> list<T>::rend() {
        return list_r_iterator<T>(nullptr);
    }

    template<typename T>
    list_iterator<T> list<T>::search(const T& val) {
        Node<T>* current = _head.get();
        Node<T>* end = _sentinel.get();

        while (current != end) {
            if (current->data == val) {
                break;   
            } 
            current = current->next.get();
        }
        return list_iterator<T>(current);
    }

    template<typename T>
    list_iterator<T> list<T>::search(T&& val) {
        Node<T>* current = _head.get();
        Node<T>* end = _sentinel.get();

        while (current != end) {
            if (current->data == val) {
                break;   
            } 
            current = current->next;
        }
        return list_iterator<T>(current);
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

#endif
