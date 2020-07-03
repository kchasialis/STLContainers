#ifndef PQUEUE_H
#define PQUEUE_H

#include <functional>
#include "Vector.h"

namespace ADT {

    template<typename T, class Compare = std::less<T>>
    class PQueue {
    private:
        Vector<T> data;
        Compare compare;
        void _heapify_up(size_t index);
        void _heapify_down(size_t index);
    public:
        PQueue() = default;
        PQueue(const PQueue& other);
        PQueue(PQueue&& other);

        PQueue& operator=(const PQueue& other) = default;
        PQueue& operator=(PQueue&& other) = default;

        bool empty() const;
        size_t size() const;
        const T& top() const;
        void push(const T& val);
        void push(T&& val);
        void pop();
    };

    template<typename T, class Less>
    PQueue<T, Less>::PQueue(const PQueue &other) : data(other.data) {}

    template<typename T, class Less>
    PQueue<T, Less>::PQueue(PQueue &&other) : data(other.data) {}

    template<typename T, class Less>
    bool PQueue<T, Less>::empty() const {
        return data.empty();
    }

    template<typename T, class Less>
    size_t PQueue<T, Less>::size() const {
        return data.size();
    }

    template<typename T, class Less>
    const T &PQueue<T, Less>::top() const {
        return data.front();
    }

    template<typename T, class Less>
    void PQueue<T, Less>::push(const T &val) {
        data.push_back(val);
        _heapify_up(data.size() - 1);
    }

    template<typename T, class Less>
    void PQueue<T, Less>::push(T &&val) {
        data.push_back(std::forward<T>(val));
        _heapify_up(data.size() - 1);
    }

    template<typename T, class Less>
    void PQueue<T, Less>::pop() {
        if (data.size() == 0) {
            throw std::out_of_range("Attempting to pop from empty priority queue");
        }
        data.front() = data.back();
        data.pop_back();

        _heapify_down(0);
    }

    template<typename T, class Less>
    void PQueue<T, Less>::_heapify_up(size_t index) {
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
    void PQueue<T, Less>::_heapify_down(size_t index) {
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