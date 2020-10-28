#pragma once

#include <climits>
#include <type_traits>

namespace adt {

    #define _hash_insert_unique(key, val, handler)                           \
        do {                                                                 \
            size_t pos;                                                      \
            /* Hash the value and return information.  */                    \
            auto info = _get_hash_info(key);                                 \
                                                                             \
            /* Keys are unique, try and find it first.  */                   \
            auto p = _find_or_prepare_insert(key, info.pos, info.h2_hash);   \
            /* If we found it, return early.  */                             \
            if (!p.find_info.second) {                                       \
                return _handle_elem_found(p.find_info, handler);             \
            }                                                                \
            pos = p.found_deleted ? p.del_pos : p.empty_pos;                 \
                                                                             \
            _check_load_factor(info.hash, pos);                              \
                                                                             \
            while (1) {                                                      \
                if (is_empty_or_deleted(_ctrls[pos])) {                      \
                    _ctrls[pos] = info.h2_hash;                              \
                    _slots[pos] = _construct_new_element(val);               \
                    _size++;                                                 \
                                                                             \
                    /* Update first element position.  */                    \
                    if (pos < _first_elem_pos) {                             \
                        _first_elem_pos = pos;                               \
                    }                                                        \
                                                                             \
                    return {iterator(&_slots[pos]), true};                   \
                }                                                            \
                pos = mod(pos + 1, _capacity);                               \
            }                                                                \
        } while(0)

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
    
    // Rounds up the capacity to the next power of 2.
    size_t normalize_capacity(size_t v) {
        v--;
        for (size_t i = 1; i < sizeof(v) * CHAR_BIT; i *= 2) {
            v |= v >> i;
        }
        return ++v;
    }

    // Capacity should always be a power of 2.
    bool is_valid_capacity(size_t capacity) {
        return (capacity && (!(capacity & (capacity - 1)))) && capacity > 0;
    }

    // Fast modulo knowing capacity is power of 2.
    uint64_t mod(uint64_t n, uint64_t d) {
        return (n & (d - 1));
    }
}