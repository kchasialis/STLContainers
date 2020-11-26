#pragma once

#include <cassert>

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

        rb_node &operator=(const rb_node &node) = default;
    };

    template<class Container>
    void _rbtree_destruct(Container *cnt, rb_node<container::node_type> *tnode) {
        if (tnode) {
            _rbtree_destruct(cnt, tnode->left);
            _rbtree_destruct(cnt, tnode->right);

            cnt->_clear_node(tnode);
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
            if (left_right_child) left_right_child->parent = tnode;

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

            tnode->right = right_left_child;
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
    void _rbtree_restore_balance(rb_node<container::node_type> **root, rb_node<container::node_type> *sentinel, rb_node<container::node_type> *tnode, balance_t btype) {
        rb_node<container::node_type> *sibling, *uncle;

        if (btype == DELETION) {
            while (tnode != *root && _rbtree_get_color<Container>(tnode) == BLACK) {

                if (tnode == _rbtree_left_of<Container>(_rbtree_parent_of<Container>(tnode))) {
                    sibling = _rbtree_right_of<Container>(_rbtree_parent_of<Container>(tnode));

                    if (_rbtree_get_color<Container>(sibling) == RED) {
                        _rbtree_set_color<Container>(sibling, BLACK);
                        _rbtree_set_color<Container>(_rbtree_parent_of<Container>(tnode), RED);
                        _rbtree_rotate_left<Container>(root, _rbtree_parent_of<Container>(tnode));
                        sibling = _rbtree_right_of<Container>(_rbtree_parent_of<Container>(tnode));
                    }

                    if (_rbtree_get_color<Container>(_rbtree_left_of<Container>(sibling)) == BLACK && _rbtree_get_color<Container>(_rbtree_right_of<Container>(sibling)) == BLACK) {
                        _rbtree_set_color<Container>(sibling, RED);
                        tnode = _rbtree_parent_of<Container>(tnode);
                    } else {
                        if (_rbtree_get_color<Container>(_rbtree_right_of<Container>(sibling)) == BLACK) {
                            _rbtree_set_color<Container>(_rbtree_left_of<Container>(sibling), BLACK);
                            _rbtree_set_color<Container>(sibling, RED);
                            _rbtree_rotate_right<Container>(root, sibling);
                            sibling = _rbtree_right_of<Container>(_rbtree_parent_of<Container>(tnode));
                        }

                        _rbtree_set_color<Container>(sibling, _rbtree_get_color<Container>(_rbtree_parent_of<Container>(tnode)));
                        _rbtree_set_color<Container>(_rbtree_parent_of<Container>(tnode), BLACK);
                        _rbtree_set_color<Container>(_rbtree_right_of<Container>(sibling), BLACK);
                        _rbtree_rotate_left<Container>(root, _rbtree_parent_of<Container>(tnode));
                        tnode = *root;
                    }
                }
                else {
                    sibling = _rbtree_left_of<Container>(_rbtree_parent_of<Container>(tnode));

                    if (_rbtree_get_color<Container>(sibling) == RED) {
                        _rbtree_set_color<Container>(sibling, BLACK);
                        _rbtree_set_color<Container>(_rbtree_parent_of<Container>(tnode), RED);
                        _rbtree_rotate_right<Container>(root, _rbtree_parent_of<Container>(tnode));
                        sibling = _rbtree_left_of<Container>(_rbtree_parent_of<Container>(tnode));
                    }

                    if (_rbtree_get_color<Container>(_rbtree_right_of<Container>(sibling)) == BLACK && _rbtree_get_color<Container>(_rbtree_left_of<Container>(sibling)) == BLACK) {
                        _rbtree_set_color<Container>(sibling, RED);
                        tnode = _rbtree_parent_of<Container>(tnode);
                    } else {
                        if (_rbtree_get_color<Container>(_rbtree_left_of<Container>(sibling)) == BLACK) {
                            _rbtree_set_color<Container>(_rbtree_right_of<Container>(sibling), BLACK);
                            _rbtree_set_color<Container>(sibling, RED);
                            _rbtree_rotate_left<Container>(root, sibling);
                            sibling = _rbtree_left_of<Container>(_rbtree_parent_of<Container>(tnode));
                        }

                        _rbtree_set_color<Container>(sibling, _rbtree_get_color<Container>(_rbtree_parent_of<Container>(tnode)));
                        _rbtree_set_color<Container>(_rbtree_parent_of<Container>(tnode), BLACK);
                        _rbtree_set_color<Container>(_rbtree_left_of<Container>(sibling), BLACK);
                        _rbtree_rotate_right<Container>(root, _rbtree_parent_of<Container>(tnode));
                        tnode = *root;
                    }
                }
            }

            if (tnode != nullptr && _rbtree_get_color<Container>(tnode) != BLACK) {
                _rbtree_set_color<Container>(tnode, BLACK);
            }
        }
        else if (btype == INSERTION) {
            tnode->color = RED;
            (*root)->parent = nullptr;

            while (tnode != sentinel && tnode != *root) {
                if (tnode->parent->color != RED) {
                    break;
                }

                if (_rbtree_parent_of<Container>(tnode) == _rbtree_left_of<Container>(_rbtree_grandparent_of<Container>(tnode))) {

                    uncle = _rbtree_right_of<Container>(_rbtree_grandparent_of<Container>(tnode));
                    if (_rbtree_get_color<Container>(uncle) == RED) {
                        /* Uncle RED means color-flip.  */
                        _rbtree_set_color<Container>(_rbtree_parent_of<Container>(tnode), BLACK);
                        _rbtree_set_color<Container>(_rbtree_grandparent_of<Container>(tnode), RED);
                        _rbtree_set_color<Container>(uncle, BLACK);
                        tnode = _rbtree_grandparent_of<Container>(tnode);
                    } else {
                        /* Uncle BLACK means rotations.  */
                        if (tnode == _rbtree_right_of<Container>(_rbtree_parent_of<Container>(tnode))) {
                            tnode = _rbtree_parent_of<Container>(tnode);
                            _rbtree_rotate_left<Container>(root, tnode);
                        }
                        _rbtree_set_color<Container>(_rbtree_parent_of<Container>(tnode), BLACK);
                        _rbtree_set_color<Container>(_rbtree_grandparent_of<Container>(tnode), RED);
                        _rbtree_rotate_right<Container>(root, _rbtree_grandparent_of<Container>(tnode));
                    }
                }
                else {
                    uncle = _rbtree_left_of<Container>(_rbtree_grandparent_of<Container>(tnode));

                    if (_rbtree_get_color<Container>(uncle) == RED) {
                        /* Uncle RED means color-flip.  */
                        _rbtree_set_color<Container>(_rbtree_parent_of<Container>(tnode), BLACK);
                        _rbtree_set_color<Container>(_rbtree_grandparent_of<Container>(tnode), RED);
                        _rbtree_set_color<Container>(uncle, BLACK);
                        tnode = _rbtree_grandparent_of<Container>(tnode);
                    } else {
                        /* Uncle BLACK means rotations.  */
                        if (tnode == _rbtree_left_of<Container>(_rbtree_parent_of<Container>(tnode))) {
                            tnode = _rbtree_parent_of<Container>(tnode);
                            _rbtree_rotate_right<Container>(root, tnode);
                        }

                        _rbtree_set_color<Container>(_rbtree_parent_of<Container>(tnode), BLACK);
                        _rbtree_set_color<Container>(_rbtree_grandparent_of<Container>(tnode), RED);
                        _rbtree_rotate_left<Container>(root, _rbtree_grandparent_of<Container>(tnode));
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

    template<typename Container, typename K, typename V>
    rb_node<container::node_type> *_rbtree_bst_insert(Container *cnt, bool &added_new, const K &key, V val) {
        rb_node<container::node_type> *new_node, *current;

        if (cnt->empty()) {
            added_new = true;
            cnt->_root = cnt->_construct_new_element(std::forward<V>(val));
            cnt->_sentinel->left = cnt->_root;
            cnt->_root->parent = cnt->_sentinel;
            return cnt->_root;
        }

        current = cnt->_root;
        while (1) {

            if (cnt->_less(key, cnt->_get_key(current))) {
                if (current->left != nullptr) {
                    current = current->left;
                } else {
                    added_new = true;
                    new_node = cnt->_construct_new_element(std::forward<V>(val));
                    current->left = new_node;
                    new_node->parent = current;

                    return new_node;
                }
            }
            else if (cnt->_less(cnt->_get_key(current), key)) {
                if (current->right != nullptr) {
                    current = current->right;
                } else {
                    added_new = true;
                    new_node = cnt->_construct_new_element(std::forward<V>(val));
                    current->right = new_node;
                    new_node->parent = current;

                    return new_node;
                }
            }
            else {
                added_new = false;
                return current;
            }
        }
    }

    /* Container functions.  */
    template<class Container, typename R, typename K, typename V, typename... Args>
    R _rbtree_insert(Container *cnt, const K &key, V val, Args &&... args) {
        rb_node<container::node_type> *current;
        bool added_new;

        assert(sizeof...(Args) <= 1);

        current = _rbtree_bst_insert<Container, K, V>(cnt, added_new, key, std::forward<V>(val));

        if (!added_new) return cnt->_handle_elem_found(current, std::forward<Args>(args)...);

        _rbtree_restore_balance<Container>(&(cnt->_root), cnt->_sentinel, current, INSERTION);

        cnt->_root->color = BLACK;
        cnt->_sentinel->left = cnt->_root;
        cnt->_root->parent = cnt->_sentinel;
        cnt->_size++;

        return cnt->_handle_elem_not_found(current);
    }

    template<class Container>
    rb_node<container::node_type> * _rbtree_prepare_erase(Container *cnt, rb_node<container::node_type> *current, rb_node<container::node_type> *successor) {
        rb_node<container::node_type> *r_node, *parent_node;

        /* If this node is not a leaf and has both children.  */
        if (current->left != nullptr && current->right != nullptr) {
            /* Get the minimum value of the right subtree.  */
            current->data = std::move(successor->data);
            current = successor;
        }

        r_node = current->left != nullptr ? current->left : current->right;

        /* If node has one child.  */
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
                _rbtree_restore_balance<Container>(&(cnt->_root), cnt->_sentinel, r_node, DELETION);
            }
        }
        else if (current->parent == nullptr) {
            return cnt->_root;
        }
        else {
            /* Its a leaf.  */
            if (_rbtree_get_color<Container>(current) == BLACK) {
                /* Balance only if its a black node.  */
                _rbtree_restore_balance<Container>(&(cnt->_root), cnt->_sentinel, current, DELETION);
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
        container::internal_ptr current;
        
        current = cnt->_root;        
        while (current) {
            if (cnt->_less(key, cnt->_get_key(current))) {
                current = current->left;
            }
            else if (cnt->_less(cnt->_get_key(current), key)) {
                current = current->right;
            }
            else {
                return container::iterator(cnt->_sentinel, current);
            }
        }

        return cnt->end();
    }

    template<class Container>
    rb_node<container::node_type> *_rbtree_find_bound(Container *cnt, rb_node<container::node_type> *tnode, const container::key_type &key) {
        if (tnode == nullptr) return nullptr;

        if (cnt->_is_equal_key(cnt->_get_key(tnode), key)) return tnode;

        if (cnt->_less(cnt->_get_key(tnode), key)) return _rbtree_find_bound(cnt, tnode->right, key);

        auto rnode = _rbtree_find_bound(cnt, tnode->left, key);
        return rnode == nullptr ? tnode : rnode;
    }
}