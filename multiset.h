#pragma once

#include <memory>

namespace adt {
    
    /*On MultiSet its possible to have multiple keys with the same value, we need them on the same node*/
    template<typename T>
    struct DataNode {
        T data;
        std::shared_ptr<DataNode<T>> next;
        std::shared_ptr<DataNode<T>> previous; 
        DataNode() : data(0), next(nullptr), previous(nullptr) {}
        explicit DataNode(T&& val) : data(std::forward<T>(val)), next(nullptr), previous(nullptr) {}
        explicit DataNode(const T& val) : data(val), next(nullptr), previous(nullptr) {}
        template<typename... Args>
        explicit DataNode(Args &&... args) noexcept : data(std::forward<Args>(args)...), next(nullptr), previous(nullptr) {}
    };
    
    template<typename T>
    struct DataList {
        std::shared_ptr<DataNode<T>> head;
        std::shared_ptr<DataNode<T>> tail;
        size_t size;
        ~DataList() {
            /*Break shared_ptr cycle*/
            std::shared_ptr<DataNode<T>> current = head;
            for (size_t i = 0 ; i < size ; i++) {
                current->next = nullptr;
            }
        }
        DataList() : head(nullptr), tail(nullptr), size(0) {}

        void __push_back(std::shared_ptr<DataNode<T>>& new_node) {
            if (this->head != nullptr) {
                new_node->previous = this->tail;
                this->tail = this->tail->next = new_node;
            } else {
                this->tail = this->head  = new_node;
            }
            ++this->size;
        }

        void _pop_front() {
            if (this->head) {
                this->head = this->head->next;
                --this->size;
            }
            if (this->size == 0) {
                this->tail = nullptr;
            }
        }

        void _push_back(const T& data) {
            std::shared_ptr<DataNode<T>> tmp(new DataNode<T>(data));
            __push_back(tmp);
        }

        void _push_back(T&& data) {
            std::shared_ptr<DataNode<T>> tmp(new DataNode<T>(std::forward<T>(data)));
            __push_back(tmp);    
        }
    };

    template<typename T>
    class multiset_iterator;

    template<typename T, class Less = std::less<T>>
    class multiset;
    
    template<typename T>
    class MultiSetNode {
        friend class multiset<T>;
    private:
        enum {
            RED,
            BLACK
        };
        
        DataList<T> data_list;
        MultiSetNode *left;
        MultiSetNode *right;
        MultiSetNode *parent;
        int8_t color;

        MultiSetNode();
        explicit MultiSetNode(const T& data);
        explicit MultiSetNode(T&& data);
        explicit MultiSetNode(const MultiSetNode& node) = default;
        explicit MultiSetNode(MultiSetNode&& node) noexcept = default;

        MultiSetNode& operator=(MultiSetNode&& node) noexcept = default;
    };

    template<typename T>
    class multiset_iterator : public std::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t , T*, T&> {
        friend class multiset<T>;
    public:
        using const_data_reference = const T&;
        using const_data_pointer = const T*;
        
        explicit multiset_iterator(MultiSetNode<T>* _ptr = nullptr) : ptr(_ptr) {
            if (_ptr) {
                this->dataptr = _ptr->data_list.head.get();
            }
        }
        multiset_iterator(const multiset_iterator<T>& other) : ptr(other.ptr), dataptr(other.dataptr) {}
        multiset_iterator<T>& operator=(const multiset_iterator<T>& other) {
            this->ptr = other.ptr;
            this->dataptr = other.dataptr; 
            return *this;
        }
        multiset_iterator<T>& operator=(MultiSetNode<T>* _ptr) {
            if (_ptr == nullptr) {
                return *this;
            }
            this->ptr = _ptr;
            this->dataptr = _ptr->data_list.head.get();
            return *this;
        }
        bool operator==(const multiset_iterator<T>& rhs) const {
            return this->ptr == rhs.ptr;
        }
        bool operator!=(const multiset_iterator<T>& rhs) const {
            return !(*this == rhs);
        }
        const_data_reference operator*() {
            return this->dataptr->data;
        }
        const_data_pointer operator->() {
            return &(this->dataptr->data);
        }
        /*Inorder successor algorithm*/
        multiset_iterator<T> operator++() {
            if (this->ptr == nullptr) {
                return multiset_iterator<T>();
            }
            /*First, check if the pointer to internal list has no other elements*/
            if (this->dataptr->next.get() != nullptr) {
                this->dataptr = this->dataptr->next.get();
                return *this;
            }

            MultiSetNode<T>* current;
            if (this->ptr->right) {
                current = this->ptr->right;
                while (current->left != nullptr) {
                    current = current->left;
                }
            }
            else {
                current = this->ptr->parent;
                while (current != nullptr && this->ptr == current->right) {
                    this->ptr = current;
                    current = current->parent;
                }
            }

            this->ptr = current;
            this->dataptr = this->ptr != nullptr ? this->ptr->data_list.head.get() : nullptr;
            return *this;
        }
        multiset_iterator<T> operator++(int) {
            if (this->ptr == nullptr) {
                return multiset_iterator<T>();
            }

            multiset_iterator<T> tmp(*this);
            ++(*this);
            return tmp;
        }
        /*Inorder predecessor algorithm*/
        multiset_iterator<T> operator--() {
            if (this->ptr == nullptr) {
                return multiset_iterator<T>();
            }

            if (this->dataptr->previous != nullptr) {
                this->dataptr = this->dataptr->previous.get();
                return *this;
            }

            MultiSetNode<T>* current;
            if (this->ptr->left) {
                current = this->ptr->left;
                while (current->right != nullptr) {
                    current = current->right;
                }
            }
            else {
                current = this->ptr->parent;
                while (current != nullptr && this->ptr == current->left) {
                    this->ptr = current;
                    current = current->parent;
                }
            }

            this->ptr = current;
            this->dataptr = this->ptr != nullptr ? this->ptr->data_list.head.get() : nullptr;
            return *this;
        }
        multiset_iterator<T> operator--(int) {
            if (this->ptr == nullptr) {
                return multiset_iterator<T>();
            }

            multiset_iterator<T> tmp(*this);
            --(*this);
            return tmp;
        }
        void swap(multiset_iterator<T>& lhs, multiset_iterator<T>& rhs) {
            std::swap(lhs, rhs);
        }
    private:
        MultiSetNode<T>* ptr;
        DataNode<T> *dataptr;
    };

    template<typename T, class Less>
    class multiset {
    private:
        enum {
            RED,
            BLACK
        };
        enum SetBalanceType {
            DELETION,
            INSERTION
        };

        MultiSetNode<T> *root;
        MultiSetNode<T> *endNode;
        Less less;
        size_t _size;
        void rotate_left(MultiSetNode<T> *);
        void rotate_right(MultiSetNode<T> *);
        MultiSetNode<T>* bst_insert(bool&, const T&);
        MultiSetNode<T>* bst_insert(bool&, T&&);
        MultiSetNode<T>* _copy_tree(MultiSetNode<T>*);
        MultiSetNode<T>* _remove(MultiSetNode<T> *, MultiSetNode<T>*);
        inline bool _is_equal_key(const T& nodeval, const T& val);
        MultiSetNode<T>* _find_bound(MultiSetNode<T> *_root, const T &val);
        void restore_balance(MultiSetNode<T>*, int8_t type);
        void tree_destroy(MultiSetNode<T> *);
    public:
        multiset() noexcept;
        multiset(const multiset& rhs) noexcept;
        multiset(multiset&& rhs) noexcept;
        ~multiset();

        multiset& operator=(multiset x);

        bool add(const T& data);
        bool add(T&& data);
        bool remove(const T& key);
        multiset_iterator<T> remove(multiset_iterator<T> itr);
        void clear() noexcept;
        void clear() const noexcept;
        bool empty();

        size_t size();

        multiset_iterator<T> begin();
        multiset_iterator<T> end();
        multiset_iterator<T> search(const T& key);
        multiset_iterator<T> lower_bound(const T& val);
        multiset_iterator<T> upper_bound(const T& val);

        friend void swap(multiset& lhs, multiset& rhs) {
            using std::swap;

            swap(lhs.root, rhs.root);
            swap(lhs.endNode, rhs.endNode);
            swap(lhs.less, rhs.less);
            swap(lhs._size, rhs._size);
        }

        MultiSetNode<T>* _successor(MultiSetNode<T>*);
    };

    #define multiset_parent_of(node) _multiset_parent_of(node)

        template<typename T>
        static inline MultiSetNode<T>* _multiset_parent_of(MultiSetNode<T>* node) {
            return node ? node->parent : nullptr;
        }

    #define multiset_grandparent_of(node) _multiset_grandparent_of(node)

        template<typename T>
        static inline MultiSetNode<T>* _multiset_grandparent_of(MultiSetNode<T>* node) {
            if (node) {
                if (node->parent) {
                    return node->parent->parent;
                }
            }
            return nullptr;
        }

        template<typename T>
        static inline MultiSetNode<T>* multiset_left_of(MultiSetNode<T> *node) {
            return node ? node->left : nullptr;
        }

        template<typename T>
        static inline MultiSetNode<T>* multiset_right_of(MultiSetNode<T> *node) {
            return node ? node->right : nullptr;
        }

    #define multiset_assign_color(node, color) _multiset_multiset_color(node, color)

        template<typename T>
        static inline void _multiset_multiset_color(MultiSetNode<T> *node, char color) {
            if (node) {
                node->color = color;
            }
        }

    #define multiset_color_of(node) _multiset_color_of(node)

        template<typename T>
        static inline char _multiset_color_of(MultiSetNode<T> *node) {
            return node ? node->color : (int8_t) MultiSetNode<T>::BLACK;
        }

    #define multiset_balance_insertion(side, rotate_1, rotate_2)             \
        do  {                                                           \
                {                                                       \
                    MultiSetNode<T> *uncle = side(multiset_grandparent_of(node)); \
                                                                        \
                    if (multiset_color_of(uncle) == RED) {                   \
                        /*Uncle RED means color-flip*/                  \
                                                                        \
                        multiset_assign_color(multiset_parent_of(node), BLACK);   \
                        multiset_assign_color(multiset_grandparent_of(node), RED);\
                        multiset_assign_color(uncle, BLACK);                 \
                        node = multiset_grandparent_of(node);                \
                    }                                                   \
                    else {                                              \
                        /*Uncle BLACK means rotations*/                 \
                        if (node == side(multiset_parent_of(node))) {        \
                            node = multiset_parent_of(node);                 \
                            rotate_2(node);                             \
                        }                                               \
                        multiset_assign_color(multiset_parent_of(node), BLACK);   \
                        multiset_assign_color(multiset_grandparent_of(node), RED);\
                        rotate_1(multiset_grandparent_of(node));             \
                    }                                                   \
                }                                                       \
        } while(0)

    #define multiset_balance_deletion(side1, side2, rotate_side1, rotate_side2)   \
        do {                                                                 \
                {                                                            \
                    MultiSetNode<T>* sibling = side1(multiset_parent_of(node));        \
                    if (multiset_color_of(sibling) == RED) {                      \
                        multiset_assign_color(sibling, BLACK);                    \
                        multiset_assign_color(multiset_parent_of(node), RED);          \
                        rotate_side1(multiset_parent_of(node));                   \
                        sibling = side1(multiset_parent_of(node));                \
                    }                                                        \
                    if (multiset_color_of(side2(sibling)) == BLACK && multiset_color_of(side1(sibling)) == BLACK) { \
                        multiset_assign_color(sibling, RED);                      \
                        node = multiset_parent_of(node);                          \
                    }                                                        \
                    else {                                                   \
                        if (multiset_color_of(side1(sibling)) == BLACK) {         \
                            multiset_assign_color(side2(sibling), BLACK);         \
                            multiset_assign_color(sibling, RED);                  \
                            rotate_side2(sibling);                           \
                            sibling = side1(multiset_parent_of(node));            \
                        }                                                    \
                        multiset_assign_color(sibling, multiset_color_of(multiset_parent_of(node)));\
                        multiset_assign_color(multiset_parent_of(node), BLACK);        \
                        multiset_assign_color(side1(sibling), BLACK);             \
                        rotate_side1(multiset_parent_of(node));                   \
                        node = this->root;                                   \
                    }                                                        \
                }                                                            \
        } while(0)

    template<typename T, class Less>
    multiset<T, Less>::multiset() noexcept : root(nullptr), endNode(nullptr) , _size(0) {}

    template<typename T, class Less>
    MultiSetNode<T>* multiset<T, Less>::_copy_tree(MultiSetNode<T>* other_root) {
        if (other_root == nullptr) {
            return nullptr;
        }

        MultiSetNode<T>* new_node = new MultiSetNode<T>();
        for (DataNode<T> *current = other_root->data_list.head.get() ; current != nullptr ; current = current->next.get()) {
            new_node->data_list._push_back(current->data);
        }

        new_node->left = _copy_tree(other_root->left);
        if (new_node->left) {
            new_node->left->parent = new_node;
        }

        new_node->right = _copy_tree(other_root->right);
        if (new_node->right) {
            new_node->right->parent = new_node;
        }

        return new_node;
    }

    template<typename T, class Less>
    multiset<T, Less>::multiset(const MultiSet& other) noexcept {
        /*Create an exact copy of this MultiSet, O(n)*/
        this->root = _copy_tree(other.root);
        this->endNode = new MultiSetNode<T>();
        this->endNode->left = this->root;
        this->root->parent = this->endNode;
    }

    template<typename T, class Less>
    multiset<T, Less>::multiset(MultiSet&& other) noexcept : multiset() {
        swap(*this, other);
    }

    template<typename T, class Less>
    multiset<T, Less>& multiset<T, Less>::operator=(multiset other) {
        /*Copy and swap idiom, let the compiler handle the copy of the argument*/
        swap(*this, other);

        return *this;
    }

    template<typename T, class Less>
    void multiset<T, Less>::tree_destroy(MultiSetNode<T> *current) {

        if (!current) {
            return;
        }

        tree_destroy(current->left);
        tree_destroy(current->right);

        delete current;
    }

    template<typename T, class Less>
    multiset<T, Less>::~multiset() {
        if (this->root) {
            tree_destroy(this->root);
            delete this->endNode;
        }
    }

    template<typename T, class Less>
    MultiSetNode<T> *multiset<T, Less>::bst_insert(bool &added_new, const T &data) {

        if (empty()) {
            added_new = true;
            this->root = new MultiSetNode<T>(data);
            this->endNode = new MultiSetNode<T>();
            this->endNode->left = this->root;
            this->root->parent = this->endNode;
            return this->root;
        }

        MultiSetNode<T> *current = this->root;
        while (1) {

            if (less(data, current->data_list.head->data)) {
                if (current->left != nullptr) {
                    current = current->left;
                }
                else {
                    added_new = true;
                    MultiSetNode<T> *new_node = new MultiSetNode<T>(data);
                    current->left = new_node;
                    new_node->parent = current;

                    return new_node;
                }
            }
            else if (less(current->data_list.head->data, data)) {
                if (current->right != nullptr) {
                    current = current->right;
                }
                else {
                    added_new = true;
                    MultiSetNode<T> *new_node = new MultiSetNode<T>(data);
                    current->right = new_node;
                    new_node->parent = current;

                    return new_node;
                }
            }
            else {
                added_new = false;
                current->data_list._push_back(data);
                return current;
            }
        }
    }

    template<typename T, class Less>
    MultiSetNode<T> *multiset<T, Less>::bst_insert(bool &added_new, T &&data) {

        if (empty()) {
            added_new = true;
            this->root = new MultiSetNode<T>(std::forward<T>(data));
            this->endNode = new MultiSetNode<T>();
            this->endNode->left = this->root;
            this->root->parent = this->endNode;
            return this->root;
        }

        MultiSetNode<T> *current = this->root;
        while (1) {

            if (less(data, current->data_list.head->data)) {
                if (current->left != nullptr) {
                    current = current->left;
                }
                else {
                    added_new = true;
                    MultiSetNode<T> *new_node = new MultiSetNode<T>(std::forward<T>(data));
                    current->left = new_node;
                    new_node->parent = current;

                    return new_node;
                }
            }
            else if (less(current->data_list.head->data, data)) {
                if (current->right != nullptr) {
                    current = current->right;
                }
                else {
                    added_new = true;
                    MultiSetNode<T> *new_node = new MultiSetNode<T>(std::forward<T>(data));
                    current->right = new_node;
                    new_node->parent = current;

                    return new_node;
                }
            }
            else {
                added_new = false;
                current->data_list._push_back(data);
                return current;
            }
        }
    }

    template<typename T, class Less>
    MultiSetNode<T>* multiset<T, Less>::_successor(MultiSetNode<T> *node) {
        if (node == nullptr) {
            return nullptr;
        }
        MultiSetNode<T>* current;
        if (node->right) {
            current = node->right;
            while (current->left != nullptr) {
                current = current->left;
            }
        }
        else {
            current = node->parent;
            while (current != nullptr && node == current->right) {
                node = current;
                current = current->parent;
            }
        }

        return current;
    }


    template<typename T, class Less>
    multiset_iterator<T> multiset<T, Less>::begin() {
        MultiSetNode<T> *current = this->root;
        if (current == nullptr) {
            return multiset_iterator<T>();
        }
        while (current->left != nullptr) {
            current = current->left;
        }

        return multiset_iterator<T>(current);
    }

    template<typename T, class Less>
    multiset_iterator<T> multiset<T, Less>::end() {
        return multiset_iterator<T>(endNode);
    }

    template<typename T, class Less>
    inline bool multiset<T, Less>::_is_equal_key(const T& nodeval, const T& val) {
        if (less(val, nodeval)) {
            return false;
        }
        else if (less(nodeval, val)) {
            return false;
        }
        return true;
    }

    template<typename T, class Less>
    MultiSetNode<T>* multiset<T, Less>::_find_bound(MultiSetNode<T> *_root, const T &val) {
        if (_root == nullptr) {
            return nullptr;
        }

        if (_is_equal_key(_root->data_list.head->data, val)) {
            return _root;
        }

        if (less(_root->data_list.head->data, val)) {
            return _find_bound(_root->right, val);
        }

        MultiSetNode<T>* retnode = _find_bound(_root->left, val);
        return retnode == nullptr ? _root : retnode;
    }

    template<typename T, class Less>
    multiset_iterator<T> multiset<T, Less>::upper_bound(const T& val) {
        MultiSetNode<T>* retnode = _find_bound(this->root, val);
        if (retnode == nullptr) {
            return this->end();
        }
        else {
            if (_is_equal_key(retnode->data_list.head->data, val)) {
                return multiset_iterator<T>(_successor(retnode));
            }
            else {
                return multiset_iterator<T>(retnode);
            }
        }
    }

    template<typename T, class Less>
    multiset_iterator<T> multiset<T, Less>::lower_bound(const T& val) {
        MultiSetNode<T> *retnode = _find_bound(this->root, val);
        if (retnode == nullptr) {
            return this->end();
        }
        else {
            return multiset_iterator<T>(retnode);
        }
    }

    template<typename T, class Less>
    multiset_iterator<T> multiset<T, Less>::search(const T &val) {
        if (this->root == nullptr) {
            return this->end();
        }
        MultiSetNode<T> *current = this->root;
        while (current) {
            if (less(val, current->data_list.head->data)) {
                current = current->left;
            }
            else if (less(current->data_list.head->data, val)) {
                current = current->right;
            }
            else {
                return multiset_iterator<T>(current);
            }
        }

        return this->end();
    }

    template<typename T, class Less>
    bool multiset<T, Less>::add(const T &data) {

        bool added_new;
        MultiSetNode<T> *current = bst_insert(added_new, data);

        if (!added_new) {
            return false;
        }

        restore_balance(current,INSERTION);

        this->root->color = BLACK;
        this->_size++;
        this->endNode->left = this->root;
        this->root->parent = this->endNode;

        return true;
    }

    template<typename T, class Less>
    bool multiset<T, Less>::add(T &&data) {

        bool added_new;
        MultiSetNode<T> *current = bst_insert(added_new, std::forward<T>(data));

        if (!added_new) {
            return false;
        }

        restore_balance(current,INSERTION);

        this->root->color = BLACK;
        this->_size++;
        this->root->parent = this->endNode;
        this->endNode->left = this->root;

        return true;
    }

    template<typename T, class Less>
    MultiSetNode<T>* multiset<T, Less>::_remove(MultiSetNode<T>* current, MultiSetNode<T>* successor) {

        /*If this node is not a leaf and has both children*/
        if (current->left != nullptr && current->right != nullptr) {
            /*Get the minimum value of the right subtree*/
            current->data_list = successor->data_list;
            current = successor;
        }

        MultiSetNode<T>* r_node = current->left != nullptr ? current->left : current->right;

        /*If node has one children*/
        if (r_node != nullptr) {
            r_node->parent = current->parent;
            MultiSetNode<T>* parent_node = current->parent;
            if (parent_node == nullptr) {
                this->root = r_node;
            }
            else if (current == parent_node->left) {
                parent_node->left = r_node;
            }
            else {
                parent_node->right = r_node;
            }

            current->right = nullptr;
            current->left = nullptr;
            current->parent = nullptr;

            if (multiset_color_of(current) == BLACK) {
                /*Balance only if its a black node*/
                restore_balance(r_node, DELETION);
            }
        }
        else if (current->parent == nullptr) {
            this->_size = 0;
            delete this->root;
            delete this->endNode;
            this->root = nullptr;
            this->endNode = nullptr;
            return this->root;
        }
        else {
            /*Its a leaf*/
            if (multiset_color_of(current) == BLACK) {
                /*Balance only if its a black node*/
                restore_balance(current, DELETION);
            }

            MultiSetNode<T>* parent_node = current->parent;
            if (parent_node != nullptr) {
                if (current == parent_node->left) {
                    parent_node->left = nullptr;
                }
                else if (current == parent_node->right) {
                    parent_node->right = nullptr;
                }
                current->parent = nullptr;
            }
        }

        return current;
    }

    template<typename T, class Less>
    bool multiset<T, Less>::remove(const T &key) {

        MultiSetNode<T>* current = nullptr;
        auto result = search(key);

        if (result == end()) {
            return false;
        }

        MultiSetNode<T>* save = this->endNode;
        this->root->parent = nullptr;

        MultiSetNode<T>* successor =  (++multiset_iterator<T>(result)).ptr;
        current = _remove(result.ptr, successor);

        if (current != nullptr) {
            delete current;
            current = nullptr;

            /*Update the secret node*/
            this->endNode = save;
            this->root->parent = this->endNode;
            this->endNode->left = this->root;
            this->_size--;
        }

        return true;
    }

    template<typename T, class Less>
    multiset_iterator<T> multiset<T, Less>::remove(multiset_iterator<T> itr) {
        if (itr == end()) {
            return itr;
        }

        if (itr.dataptr->next != nullptr) {
            return ++itr;
        }

        MultiSetNode<T>* save = this->endNode;
        this->root->parent = nullptr;

        MultiSetNode<T>* successor =  (++multiset_iterator<T>(itr)).ptr;
        MultiSetNode<T>* current = _remove(itr.ptr, successor);

        if (current != nullptr) {
            multiset_iterator<T> retrnValue(current == successor ? itr.ptr : successor);
            delete current;
            current = nullptr;

            /*Update the secret node*/
            this->endNode = save;
            this->root->parent = this->endNode;
            this->endNode->left = this->root;
            this->_size--;

            if (successor != nullptr) {
                return retrnValue;
            }
            else {
                return this->end();
            }
        }

        return this->end();
    }

    template<typename T, class Less>
    void multiset<T, Less>::clear() noexcept {
        if (this->root) {
            tree_destroy(this->root);
            delete this->endNode;
            this->root = nullptr;
            this->endNode = nullptr;
            this->_size = 0;
        }
    }

    template<typename T, class Less>
    void multiset<T, Less>::clear() const noexcept {
        if (this->root) {
            tree_destroy(this->root);
            delete this->endNode;
            this->root = nullptr;
            this->endNode = nullptr;
            this->_size = 0;
        }
    }

    template<typename T, class Less>
    bool multiset<T, Less>::empty() {
        return this->root == nullptr;
    }


    template<typename T, class Less>
    void multiset<T, Less>::restore_balance(MultiSetNode<T>* node, int8_t type) {

        if (type == DELETION) {
            while (node != this->root && multiset_color_of(node) == BLACK) {
                if (node == multiset_left_of(multiset_parent_of(node))) {
                    multiset_balance_deletion(multiset_right_of, multiset_left_of, rotate_left, rotate_right);
                }
                else {
                    multiset_balance_deletion(multiset_left_of, multiset_right_of, rotate_right, rotate_left);
                }
            }

            if (node != nullptr && multiset_color_of(node) != BLACK) {
                multiset_assign_color(node, BLACK);
            }
        }
        else if (type == INSERTION) {
            node->color = RED;
            this->root->parent = nullptr;

            while (node != endNode && node != this->root) {
                if (node->parent->color != RED) {
                    break;
                }

                if (multiset_parent_of(node) == multiset_left_of(multiset_grandparent_of(node))) {
                    multiset_balance_insertion(multiset_right_of, rotate_right, rotate_left);
                }
                else {
                    multiset_balance_insertion(multiset_left_of, rotate_left, rotate_right);
                }
            }
        }
        else {
            throw std::invalid_argument("This should not have happened, pls debug me :)");
        }
    }

    template<typename T, class Less>
    void multiset<T, Less>::rotate_right(MultiSetNode<T> *node) {

        if (!node) {
            return;
        }

        MultiSetNode<T> *left_child = node->left;
        MultiSetNode<T> *left_right_child = left_child->right;

        node->left = left_right_child;
        if (left_right_child) {
            left_right_child->parent = node;
        }

        MultiSetNode<T> *node_p = node->parent;
        left_child->parent = node_p;

        if (node_p == nullptr) {
            this->root = left_child;
        }
        else if (node_p->right == node) {
            node_p->right = left_child;
        }
        else {
            node_p->left = left_child;
        }

        left_child->right = node;
        node->parent = left_child;
    }

    template<typename T, class Less>
    void multiset<T, Less>::rotate_left(MultiSetNode<T> *node) {

        if (!node) {
            return;
        }

        MultiSetNode<T> *right_child = node->right;
        MultiSetNode<T> *right_left_child = right_child->left;

        node->right = right_left_child;
        if (right_left_child) {
            right_left_child->parent = node;
        }

        MultiSetNode<T>  *node_p = node->parent;
        right_child->parent = node_p;

        if (node_p == nullptr) {
            this->root = right_child;
        }
        else if (node_p->left == node) {
            node_p->left = right_child;
        }
        else {
            node_p->right = right_child;
        }

        right_child->left = node;
        node->parent = right_child;
    }

    template<typename T, class Less>
    size_t multiset<T, Less>::size() {
        return this->_size;
    }

    template<typename T>
    MultiSetNode<T>::MultiSetNode() : data_list(), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}

    template<typename T>
    MultiSetNode<T>::MultiSetNode(const T &val) : data_list(), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
        data_list._push_back(val);
    }

    template<typename T>
    MultiSetNode<T>::MultiSetNode(T &&val) : data_list(), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
        data_list._push_back(val);
    }
}