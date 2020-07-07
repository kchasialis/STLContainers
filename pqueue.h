#ifndef PQUEUE_H
#define PQUEUE_H

#include <functional>
#include "vector.h"

namespace adt {

    template<typename T, class Compare = std::less<T>>
    class pqueue {
    private:
        vector<T> data;
        Compare compare;
        void _heapify_up(size_t index);
        void _heapify_down(size_t index);
    public:
        pqueue() = default;
        pqueue(const pqueue& other);
        pqueue(pqueue&& other);

        pqueue& operator=(const pqueue& other) = default;
        pqueue& operator=(pqueue&& other) = default;

        bool empty() const;
        size_t size() const;
        const T& top() const;
        void push(const T& val);
        void push(T&& val);
        void pop();
    };

    template<typename T, class Less>
    pqueue<T, Less>::pqueue(const pqueue &other) : data(other.data) {}

    template<typename T, class Less>
    pqueue<T, Less>::pqueue(pqueue &&other) : data(other.data) {}

    template<typename T, class Less>
    bool pqueue<T, Less>::empty() const {
        return data.empty();
    }

    template<typename T, class Less>
    size_t pqueue<T, Less>::size() const {
        return data.size();
    }

    template<typename T, class Less>
    const T &pqueue<T, Less>::top() const {
        return data.front();
    }

    template<typename T, class Less>
    void pqueue<T, Less>::push(const T &val) {
        data.push_back(val);
        _heapify_up(data.size() - 1);
    }

    template<typename T, class Less>
    void pqueue<T, Less>::push(T &&val) {
        data.push_back(std::forward<T>(val));
        _heapify_up(data.size() - 1);
    }

    template<typename T, class Less>
    void pqueue<T, Less>::pop() {
        if (data.size() == 0) {
            throw std::out_of_range("Attempting to pop from empty priority queue");
        }
        data.front() = data.back();
        data.pop_back();

        _heapify_down(0);
    }

    template<typename T, class Less>
    void pqueue<T, Less>::_heapify_up(size_t index) {
        if (index != 0) {
            size_t parent = (index - 1) / 2;
            if (compare(data[parent], data[index])) {
                using std::swap;
                swap(data[parent], data[index]);

                _heapify_up(parent);
            }
        }
    }

    template<typename T, class Less>
    void pqueue<T, Less>::_heapify_down(size_t index) {
        size_t left = index * 2 + 1;
        size_t right = index * 2 + 2;
        size_t highest_priority = index;

        if (left < data.size() && !compare(data[left], data[index])) {
            highest_priority = left;
        }

        if (right < data.size() && !compare(data[right], data[index])) {
            highest_priority = right;
        }

        if (highest_priority != index) {
            using std::swap;
            swap(data[index], data[highest_priority]);
            _heapify_down(highest_priority);
        }
    }
}

#endif