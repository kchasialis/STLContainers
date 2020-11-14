#pragma once

#include <climits>
#include <cstring>
#include <type_traits>

namespace hash_internal {

    using ctrl_t = int8_t;

    enum ctrl_val : ctrl_t {
        ctrl_empty = -1,
        ctrl_deleted = -2
    };

    size_t hash_seed(const ctrl_t* ctrl) {
        return reinterpret_cast<uintptr_t>(ctrl) >> 12;
    }

    size_t h1(size_t hash, const ctrl_t* ctrl) {
        return (hash) ^ hash_seed(ctrl);
    }

    ctrl_t h2(size_t hash) {
        return hash & 0x7F;
    }

    bool is_empty_slot(ctrl_t ctrl) {
        return ctrl == ctrl_empty;
    }

    bool is_deleted_slot(ctrl_t ctrl) {
        return ctrl == ctrl_deleted;
    }

    bool is_full_slot(ctrl_t ctrl) {
        return ctrl >= 0;
    }

    bool is_empty_or_deleted(ctrl_t ctrl) {
        return ctrl == ctrl_empty || ctrl == ctrl_deleted;
    }
    
    /* Rounds up the capacity to the next power of 2.  */
    size_t normalize_capacity(size_t v) {
        v--;
        for (size_t i = 1; i < sizeof(v) * CHAR_BIT; i *= 2) {
            v |= v >> i;
        }
        return ++v;
    }

    /* Capacity should always be a power of 2.  */
    bool is_valid_capacity(size_t capacity) {
        return (capacity && (!(capacity & (capacity - 1)))) && capacity > 0;
    }

    /* Fast modulo knowing capacity is power of 2.  */
    uint64_t mod(uint64_t n, uint64_t d) {
        return (n & (d - 1));
    }

    #define container typename Container

    template<class Container>
    void _hash_construct(Container *cnt) {

        cnt->_capacity = normalize_capacity(cnt->_capacity);
        cnt->_size = 0;

        assert(is_valid_capacity(cnt->_capacity) &&  "capacity should always be a power of 2");
        cnt->_first_elem_pos = cnt->_capacity;
        cnt->_ctrls = new ctrl_t[cnt->_capacity];

        /* Add one extra slot so we can determine when our hash table ends.  */
        cnt->_slots = (container::internal_ptr *) calloc (sizeof(container::internal_ptr), cnt->_capacity + 1);
        cnt->_slots[cnt->_capacity] = (container::internal_ptr) 0x1;

        memset(cnt->_ctrls, ctrl_empty, cnt->_capacity * sizeof(ctrl_t));
    }

    template<class Container>
    void _hash_destruct(Container *cnt) {
        
        if (cnt) {
            cnt->clear();
            if (cnt->_ctrls && cnt->_slots) {
                delete[] cnt->_ctrls;
                free(cnt->_slots);
            }
        }
    }

    template<class Container>
    container::find_insert_info _hash_find_or_prepare_insert(Container *cnt, const container::key_type &key, container::size_type pos, ctrl_t h2_hash) {
        bool found_deleted;
        size_t empty_pos, del_pos;

        found_deleted = false;
        while (1) {
            if (is_empty_slot(cnt->_ctrls[pos])) {
                empty_pos = pos;
                break;
            }
            else if (is_deleted_slot(cnt->_ctrls[pos]) && !found_deleted) {
                found_deleted = true;
                del_pos = pos;
            }
            else if (cnt->_ctrls[pos] == h2_hash) {
                if (cnt->_keq(cnt->_get_slot_key(cnt->_slots[pos]), key)) {
                    return {container::iterator(&cnt->_slots[pos]), found_deleted, del_pos, empty_pos};
                }
            }
            pos = mod(pos + 1, cnt->_capacity);
        }

        return {container::iterator(nullptr), found_deleted, del_pos, empty_pos};
    }

    template<class Container, typename R, typename K, typename V, typename... Args>
    R _hash_insert(Container *cnt, const K &key, V val, Args &&... args) {
        size_t pos;
        auto info = cnt->_get_hash_info(key);

        assert(sizeof...(Args) <= 1);

        /* Try to find the key first.  */
        auto p = _hash_find_or_prepare_insert<Container>(cnt, key, info.pos, info.h2_hash);
        /* If we found it, return early.  */
        if (p.it != nullptr) return cnt->_handle_elem_found(p.it, std::forward<Args>(args)...);

        pos = p.found_deleted ? p.del_pos : p.empty_pos;
        cnt->_check_load_factor(info.hash, pos);

        while (1) {
            if (is_empty_or_deleted(cnt->_ctrls[pos])) {
                cnt->_ctrls[pos] = info.h2_hash;
                cnt->_slots[pos] = cnt->_construct_new_element(std::forward<V>(val));
                cnt->_size++;

                /* Update first element position.  */
                if (pos < cnt->_first_elem_pos) cnt->_first_elem_pos = pos;

                return cnt->_handle_elem_not_found(container::iterator(&(cnt->_slots[pos])));
            }
            pos = mod(pos + 1, cnt->_capacity);
        }
    }
    
    template<class Container>
    std::pair<container::size_type, container::size_type> _hash_erase(Container *cnt, container::internal_ptr *ptr, bool erase_all) {
        container::size_type pos, prev_pos, count, to_delete;

        count = 0;
        if (ptr != &(cnt->_slots[cnt->_capacity])) {
            pos = ptr - cnt->_slots;
            cnt->_ctrls[pos] = ctrl_deleted;
            to_delete = pos;

            prev_pos = pos;
            /* Find next non-null entry.  */
            pos++;
            while (cnt->_slots[pos] == nullptr && pos != cnt->_capacity) pos++;

            /* If the deleted entry was the first element in our container, update.  */
            if (prev_pos == cnt->_first_elem_pos) cnt->_first_elem_pos = pos;

            if (erase_all) {
                count = cnt->_delete_all_slots(to_delete);
            } else {
                count = cnt->_delete_slot(to_delete);
            }

            return {pos, count};
        }

        return {cnt->_capacity, count};
    }

    template<class Container>
    void _hash_clear(Container *cnt) {

        if (cnt && cnt->_ctrls && cnt->_slots) {
            for (size_t i = 0 ; i < cnt->_capacity ; i++) {
                if (cnt->_slots[i] != nullptr) {
                    cnt->_delete_all_slots(i);
                    cnt->_slots[i] = nullptr;
                }
            }
            memset(cnt->_ctrls, ctrl_empty, cnt->_capacity * sizeof(ctrl_t));

            cnt->_size = 0;
            cnt->_capacity = 16;
            cnt->_first_elem_pos = cnt->_capacity;
        }
    }

    template<class Container>
    container::iterator _hash_find(Container *cnt, const container::key_type &key) {
        size_t pos;
        
        if (cnt->empty()) return cnt->end();

        auto info = cnt->_get_hash_info(key);
        pos = info.pos;

        while (1) {
            if (is_empty_slot(cnt->_ctrls[pos])) return cnt->end();

            if (is_full_slot(cnt->_ctrls[pos]) && cnt->_ctrls[pos] == info.h2_hash) {
                if (cnt->_keq(cnt->_get_slot_key(cnt->_slots[pos]), key)) {
                    return container::iterator(&cnt->_slots[pos]);
                }
            }
            pos = mod(pos + 1, cnt->_capacity);
        }
    }

    template<class Container>
    void _hash_rehash(Container *cnt) {
        uint64_t hash, h1_hash;
        size_t pos;
        auto old_slots = cnt->_slots;
        auto old_ctrls = cnt->_ctrls;
        auto old_cap = cnt->_capacity;
        auto new_cap = old_cap * 2;

        assert (is_valid_capacity (cnt->_capacity) && "capacity should always be a power of 2");

        cnt->_ctrls = new ctrl_t[new_cap];
        cnt->_slots = (container::internal_ptr *) calloc (sizeof (container::internal_ptr), new_cap + 1);

        if (cnt->_ctrls == nullptr || cnt->_slots == nullptr) throw std::bad_alloc();

        cnt->_slots[new_cap] = (container::internal_ptr) 0x1;
        cnt->_capacity = new_cap;
        cnt->_first_elem_pos = new_cap;

        memset(cnt->_ctrls, ctrl_empty, new_cap * sizeof(ctrl_t));

        for (size_t i = 0 ; i < old_cap ; i++) {
            if (is_full_slot(old_ctrls[i])) {
                hash = cnt->_hasher(cnt->_get_slot_key(old_slots[i]));
                h1_hash = h1(hash, cnt->_ctrls);

                pos = mod(h1_hash, cnt->_capacity);
                while (1) {
                    if (is_empty_or_deleted(cnt->_ctrls[pos])) {
                        cnt->_ctrls[pos] = h2(hash);
                        cnt->_slots[pos] = old_slots[i];

                        if (pos < cnt->_first_elem_pos) cnt->_first_elem_pos = pos;

                        break;
                    }
                    pos = mod(pos + 1, cnt->_capacity);
                }
            }
        }

        delete[] old_ctrls;
        free(old_slots);
    }

    template<class Container>
    container::hash_info _hash_get_hash_info(Container *cnt, const container::key_type &key) {
        uint64_t hash;
        size_t h1_hash, pos;
        ctrl_t h2_hash;

        hash = cnt->_hasher(key);
        h1_hash = h1(hash, cnt->_ctrls);
        h2_hash = h2(hash);
        pos = mod(h1_hash, cnt->_capacity);

        return {pos, hash, h2_hash};
    }

    template<class Container>
    void _hash_check_load_factor(Container *cnt, container::size_type n_slots, uint64_t hash, container::size_type &pos) {
        double load_factor = (double) n_slots / (double) cnt->_capacity;

        if (load_factor >= 0.75) {
            cnt->_rehash();
            pos = mod(h1(hash, cnt->_ctrls), cnt->_capacity);
        }
    }
}