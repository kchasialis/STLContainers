#pragma once

namespace rbtree_internal {

    using color_t = int8_t;
    using balance_t = int8_t;

    enum color : color_t {
        RED,
        BLACK
    };

    enum balance_type : balance_t {
        DELETION,
        INSERTION
    };

    #define container typename Container

    template<typename T>
    struct rb_node {
        T data;
        rb_node *left;
        rb_node *right;
        rb_node *parent;
        color_t color;

        rb_node() : data(), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
        explicit rb_node(const T &val) : data(val), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
        explicit rb_node(T &&val) : data(std::forward<T>(val)), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
        template<typename... Args>
        rb_node(Args&&... args) : data(std::forward<Args>(args)...), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
        explicit rb_node(const rb_node &node) = default;
        explicit rb_node(rb_node &&node) = default;

        rb_node &operator=(rb_node &&node) = default;
    };

    /* Red Black tree operations.  */
    template<class Container>
    rb_node<container::node_type> *_rbtree_copy(rb_node<container::node_type> *other_root) {
        rb_node<container::node_type> *new_node;

        if (other_root == nullptr) return nullptr;

        new_node = new rb_node<container::node_type>(other_root->data);

        new_node->left = _rbtree_copy<Container>(other_root->left);
        if (new_node->left) new_node->left->parent = new_node;

        new_node->right = _rbtree_copy<Container>(other_root->right);
        if (new_node->right) new_node->right->parent = new_node;

        return new_node;
    }

    template<class Container>
    void _rbtree_destruct(rb_node<container::node_type> *tnode) {
        if (tnode) {
            _rbtree_destruct<Container>(tnode->left);
            _rbtree_destruct<Container>(tnode->right);

            delete tnode;
        }
    }

    template<class Container>
    rb_node<container::node_type> *_rbtree_left_of(rb_node<container::node_type> *tnode) {
        return tnode ? tnode->left : nullptr;
    }

    template<class Container>
    rb_node<container::node_type> *_rbtree_right_of(rb_node<container::node_type> *tnode) {
        return tnode ? tnode->right : nullptr;
    }

    template<class Container>
    rb_node<container::node_type> *_rbtree_parent_of(rb_node<container::node_type> *tnode) {
        return tnode ? tnode->parent : nullptr;
    }

    template<class Container>
    rb_node<container::node_type> *_rbtree_grandparent_of(rb_node<container::node_type> *tnode) {
        if (tnode) {
            if (tnode->parent) {
                return tnode->parent->parent;
            }
        }
        return nullptr;
    }

    template<class Container>
    void _rbtree_set_color(rb_node<container::node_type> *tnode, color_t color) {
        if (tnode) tnode->color = color;
    }

    template<class Container>
    color_t _rbtree_get_color(rb_node<container::node_type> *tnode) {
        return tnode ? tnode->color : BLACK;
    }

    template<class Container>
    void _rbtree_rotate_right(rb_node<container::node_type> **root, rb_node<container::node_type> *tnode) {
        rb_node<container::node_type> *left_child, *left_right_child, *p_node;

        if (tnode) {
            left_child = tnode->left;
            left_right_child = left_child->right;

            tnode->left = left_right_child;
            if (left_right_child) left_right_child->_parent = tnode;

            p_node = tnode->parent;
            left_child->parent = p_node;

            if (p_node == nullptr) {
                *root = left_child;
            } else if (p_node->right == tnode) {
                p_node->right = left_child;
            } else {
                p_node->left = left_child;
            }

            left_child->right = tnode;
            tnode->parent = left_child;
        }
    }

    template<class Container>
    void _rbtree_rotate_left(rb_node<container::node_type> **root, rb_node<container::node_type> *tnode) {
        rb_node<container::node_type> *right_child, *right_left_child, *p_node;

        if (tnode) {
            right_child = tnode->right;
            right_left_child = right_child->left;

            tnode->_right = right_left_child;
            if (right_left_child) right_left_child->parent = tnode;

            p_node = tnode->parent;
            right_child->parent = p_node;

            if (p_node == nullptr) {
                *root = right_child;
            } else if (p_node->left == tnode) {
                p_node->left = right_child;
            } else {
                p_node->right = right_child;
            }

            right_child->left = tnode;
            tnode->parent = right_child;
        }
    }

    template<class Container>
    void _rbtree_restore_balance(rb_node<container::node_type> *root, rb_node<container::node_type> *sentinel, rb_node<container::node_type> *tnode, balance_t btype) {
        rb_node<container::node_type> *sibling, *uncle;

        if (btype == DELETION) {
            while (tnode != root && _rbtree_get_color(tnode) == BLACK) {

                if (tnode == _rbtree_left_of(_rbtree_parent_of(tnode))) {
                    sibling = _rbtree_right_of(_rbtree_parent_of(tnode));

                    if (_rbtree_get_color(sibling) == RED) {
                        _rbtree_set_color(sibling, BLACK);
                        _rbtree_set_color(_rbtree_parent_of(tnode), RED);
                        _rbtree_rotate_left(&root, _rbtree_parent_of(tnode));
                        sibling = _rbtree_right_of(_rbtree_parent_of(tnode));
                    }

                    if (_rbtree_get_color(_rbtree_left_of(sibling)) == BLACK && _rbtree_get_color(_rbtree_right_of(sibling)) == BLACK) {
                        _rbtree_set_color(sibling, RED);
                        tnode = _rbtree_parent_of(tnode);
                    } else {
                        if (_rbtree_get_color(_rbtree_right_of(sibling)) == BLACK) {
                            _rbtree_set_color(_rbtree_left_of(sibling), BLACK);
                            _rbtree_set_color(sibling, RED);
                            _rbtree_rotate_right(&root, sibling);
                            sibling = _rbtree_right_of(_rbtree_parent_of(tnode));
                        }

                        _rbtree_set_color(sibling, _rbtree_get_color(_rbtree_parent_of(tnode)));
                        _rbtree_set_color(_rbtree_parent_of(tnode), BLACK);
                        _rbtree_set_color(_rbtree_right_of(sibling), BLACK);
                        _rbtree_rotate_left(&root, _rbtree_parent_of(tnode));
                        tnode = root;
                    }
                }
                else {
                    sibling = _rbtree_left_of(_rbtree_parent_of(tnode));

                    if (_rbtree_get_color(sibling) == RED) {
                        _rbtree_set_color(sibling, BLACK);
                        _rbtree_set_color(_rbtree_parent_of(tnode), RED);
                        _rbtree_rotate_right(&root, _rbtree_parent_of(tnode));
                        sibling = _rbtree_left_of(_rbtree_parent_of(tnode));
                    }

                    if (_rbtree_get_color(_rbtree_right_of(sibling)) == BLACK && _rbtree_get_color(_rbtree_left_of(sibling)) == BLACK) {
                        _rbtree_set_color(sibling, RED);
                        tnode = _rbtree_parent_of(tnode);
                    } else {
                        if (_rbtree_get_color(_rbtree_left_of(sibling)) == BLACK) {
                            _rbtree_set_color(_rbtree_right_of(sibling), BLACK);
                            _rbtree_set_color(sibling, RED);
                            _rbtree_rotate_left(&root, sibling);
                            sibling = _rbtree_left_of(_rbtree_parent_of(tnode));
                        }

                        _rbtree_set_color(sibling, _rbtree_get_color(_rbtree_parent_of(tnode)));
                        _rbtree_set_color(_rbtree_parent_of(tnode), BLACK);
                        _rbtree_set_color(_rbtree_left_of(sibling), BLACK);
                        _rbtree_rotate_right(&root, _rbtree_parent_of(tnode));
                        tnode = root;
                    }
                }
            }

            if (tnode != nullptr && _rbtree_get_color(tnode) != BLACK) {
                _rbtree_set_color(tnode, BLACK);
            }
        }
        else if (btype == INSERTION) {
            tnode->color = RED;
            root->parent = nullptr;

            while (tnode != sentinel && tnode != root) {
                if (tnode->parent->color != RED) {
                    break;
                }

                if (_rbtree_parent_of(tnode) == _rbtree_left_of(_rbtree_grandparent_of(tnode))) {

                    uncle = _rbtree_right_of(_rbtree_grandparent_of(tnode));
                    if (_rbtree_color_of(uncle) == RED) {
                        /* Uncle RED means color-flip.  */
                        _rbtree_set_color(_rbtree_parent_of(tnode), BLACK);
                        _rbtree_set_color(_rbtree_grandparent_of(tnode), RED);
                        _rbtree_set_color(uncle, BLACK);
                        tnode = _rbtree_grandparent_of(tnode);
                    } else {
                        /* Uncle BLACK means rotations.  */
                        if (tnode == _rbtree_right_of(_rbtree_parent_of(tnode))) {
                            tnode = _rbtree_parent_of(tnode);
                            _rbtree_rotate_left(&root, tnode);
                        }
                        _rbtree_set_color(_rbtree_parent_of(tnode), BLACK);
                        _rbtree_set_color(_rbtree_grandparent_of(tnode), RED);
                        _rbtree_rotate_right(&root, _rbtree_grandparent_of(tnode));
                    }
                }
                else {
                    uncle = _rbtree_left_of(_rbtree_grandparent_of(tnode));

                    if (_rbtree_color_of(uncle) == RED) {
                        /* Uncle RED means color-flip.  */
                        _rbtree_set_color(_rbtree_parent_of(tnode), BLACK);
                        _rbtree_set_color(_rbtree_grandparent_of(tnode), RED);
                        _rbtree_set_color(uncle, BLACK);
                        tnode = _rbtree_grandparent_of(tnode);
                    } else {
                        /* Uncle BLACK means rotations.  */
                        if (tnode == _rbtree_left_of(_rbtree_parent_of(tnode))) {
                            tnode = _rbtree_parent_of(tnode);
                            _rbtree_rotate_right(&root, tnode);
                        }

                        _rbtree_set_color(_rbtree_parent_of(tnode), BLACK);
                        _rbtree_set_color(_rbtree_grandparent_of(tnode), RED);
                        _rbtree_rotate_left(&root, _rbtree_grandparent_of(tnode));
                    }
                }
            }
        }
        else {
            throw std::invalid_argument("This should not have happened, pls debug me :)");
        }
    }

    template<class Container>
    rb_node<container::node_type> *_rbtree_successor(rb_node<container::node_type> *tnode) {
        rb_node<container::node_type> *current = nullptr;

        if (tnode != nullptr) {
            if (tnode->right) {
                current = tnode->right;
                while (current->left != nullptr) {
                    current = current->left;
                }
            } else {
                current = tnode->parent;
                while (current != nullptr && tnode == current->right) {
                    tnode = current;
                    current = current->parent;
                }
            }
        }

        return current;
    }

    /* Container functions.  */
    template<typename Container, typename R, typename V, typename... Args>
    R _rbtree_insert(Container *cnt, V val, Args &&... args) {
        rb_node<container::node_type> *current;
        bool added_new;

        assert(sizeof...(Args) <= 1);

        current = cnt->_bst_insert(added_new, std::forward<V>(val));

        if (!added_new) return cnt->_handle_elem_found(current, std::forward<Args>(args)...);

        _rbtree_restore_balance<Container>(cnt->_root, cnt->_sentinel, current, INSERTION);

        cnt->_root->color = BLACK;
        cnt->_sentinel->left = cnt->_root;
        cnt->_root->parent = cnt->_sentinel;
        cnt->_size++;

        return cnt->_handle_elem_not_found(current);
    }

    template<typename Container, typename V>
    rb_node<container::node_type> *_rbtree_bst_insert(Container *cnt, bool &added_new, V val) {
        rb_node<container::node_type> *new_node, *current;

        if (cnt->empty()) {
            added_new = true;
            cnt->_root = new rb_node<container::node_type>(std::forward<V>(val));
            cnt->_sentinel = new rb_node<container::node_type>();
            cnt->_sentinel->left = cnt->_root;
            cnt->_root->parent = cnt->_sentinel;
            return cnt->_root;
        }

        current = cnt->_root;
        while (1) {
            
            if (cnt->_less(val, cnt->_get_data(current))) {
                if (current->_left != nullptr) {
                    current = current->_left;
                } else {
                    added_new = true;
                    new_node = new rb_node<container::node_type>(std::forward<V>(val));
                    current->left = new_node;
                    new_node->parent = current;

                    return new_node;
                }
            }
            else if (_less(current->data, cnt->_get_data(current))) {
                if (current->_right != nullptr) {
                    current = current->_right;
                } else {
                    added_new = true;
                    new_node = new rb_node<container::node_type>(std::forward<V>(val));
                    current->_right = new_node;
                    new_node->_parent = current;

                    return new_node;
                }
            }
            else {
                added_new = false;
                return current;
            }
        }        
    }

    template<class Container>
    std::pair<rb_node<container::node_type> *, container::size_type> _rbtree_erase(Container *cnt, rb_node<container::node_type> *current, rb_node<container::node_type> *successor) {
        rb_node<container::node_type> *r_node, *parent_node;

        /*If this node is not a leaf and has both children*/
        if (current->left != nullptr && current->right != nullptr) {
            /*Get the minimum value of the right subtree*/
            current->data = std::move(successor->data);
            current = successor;
        }

        r_node = current->left != nullptr ? current->left : current->right;

        /*If node has one children*/
        if (r_node != nullptr) {
            r_node->parent = current->parent;
            parent_node = current->parent;
            
            if (parent_node == nullptr) {
                cnt->_root =  r_node;
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

            if (_rbtree_get_color<Container>(current) == BLACK) {
                /* Balance only if its a black node.  */
                _rbtree_restore_balance<Container>(cnt->_root, cnt->_sentinel, r_node, DELETION);
            }
        }
        else if (current->parent == nullptr) {
            cnt->_size = 0;
            delete cnt->_root;
            delete cnt->_sentinel;
            cnt->_root = nullptr;
            cnt->_sentinel = nullptr;
            
            return cnt->_root;
        }
        else {
            /* Its a leaf.  */
            if (_rbtree_get_color<Container>(current) == BLACK) {
                /* Balance only if its a black node.  */
                _rbtree_restore_balance<Container>(cnt->_root, cnt->_sentinel, current, DELETION);
            }

            parent_node = current->parent;
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

    template<class Container>
    container::iterator _rbtree_find(Container *cnt, const container::key_type &key) {
        rb_node<container::node_type> *current;
        
        current = cnt->_root;        
        while (current) {
            if (cnt->_less(key, cnt->_get_data(current))) {
                current = current->left;
            }
            else if (cnt->_less(cnt->_get_data(current), key)) {
                current = current->right;
            }
            else {
                return container::iterator(current);
            }
        }

        return cnt->end();
    }

    template<class Container>
    rb_node<container::node_type> *_rbtree_find_bound(Container *cnt, rb_node<container::node_type> *tnode, const container::key_type &key) const {
        rb_node<container::node_type> *rnode;

        if (tnode == nullptr) {
            return nullptr;
        }

        if (cnt->_is_equal_key(cnt->_get_data(tnode), key)) {
            return tnode;
        }

        if (cnt->_less(cnt->_get_data(tnode), key)) {
            return _rbtree_find_bound<Container>(tnode->right, key);
        }

        rnode = _rbtree_find_bound<Container>(tnode->left, key);
        return rnode == nullptr ? tnode : rnode;
    }

    template<class Container>
    bool _rbtree_is_equal_key(Container *cnt, const container::key_type &lhs, const container::key_type &rhs) {
        return cnt->_less(lhs, rhs) || cnt->_less(rhs, lhs); 
    }
}