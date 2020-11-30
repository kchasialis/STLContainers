# STLContainers
This repository contains my implementation of some of the most used C++ Standard Template Library 
containers using custom iterators.

# Reference

## adt::vector

vector is implemented as a dynamically allocated array.    
When its size reaches capacity, the capacity is then doubled.  
Internally, it uses T *data to store the elements.  
When data is initialized, no constructor is called because
I use malloc to get contiguous memory of capacity * sizeof(T) size.  
When a new element is inserted, its constructor is called and when erased, its destructor is called.

### adt::vector iterators
vector's iterators are random-access iterators.

#### Properties
    * default-constructible, copy-constructible, copy-assignable and destructible  
    * Can be dereferenced as an rvalue (if in a dereferenceable state).  
    * Can be compared for equivalence using the equality/inequality operators
    (meaningful when both iterator values iterate over the same underlying sequence)
    * Can be incremented (if in a dereferenceable state).
    * Supports the arithmetic operators + and - between an iterator and an integer value, or subtracting an iterator from another.
    * Supports compound assignment operations += and -=.
    * The result is either also dereferenceable or a past-the-end iterator.
    * Two iterators that compare equal, keep comparing equal after being both increased
    * Can be decremented (if a dereferenceable iterator value precedes it).
    * Supports the offset dereference operator ([]).
    * Lvalues are swappable.
    
### adt::vector public API:      
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

    /* Constructors/Destructors.  */
     vector();
     explicit vector(size_t n);
     vector(size_type n, const value_type &val);
     vector(size_type n, value_type &&val);
     vector(const vector& other);
     vector(vector&& other) noexcept;
     ~vector();
     vector& operator=(vector other);
    
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
     size_type size() const noexcept;
     size_type capacity() const noexcept;
     void resize(size_type n);
     void resize(size_type n, const value_type &val);
     void reserve(size_type n);
     bool empty() const noexcept;
    
     /* Element access.  */
     reference operator[](size_type n);
     const_reference operator[](size_type n) const;
     reference at(size_type n) noexcept(false);
     const_reference at(size_type n) const noexcept(false);
     reference front();
     const_reference front() const;
     reference back();
     const_reference back() const;
     pointer data() noexcept;
     const_pointer data() const noexcept;
    
     /* Modifiers.  */
     template<class InputIterator>
     void assign(InputIterator first, InputIterator last);
     void assign(size_type n, const value_type &val);
     void assign(std::initializer_list<value_type> il);
     void push_back(const value_type &val);
     void push_back(value_type &&val);
     iterator insert(const_iterator pos, const value_type &val);
     iterator insert(const_iterator pos, size_type n, const value_type &val);
     iterator insert(const_iterator pos, value_type &&val);
     iterator insert(const_iterator pos, std::initializer_list<value_type> il);
     template<typename... Args>
     iterator emplace(const_iterator pos, Args&&... args);
     template<typename... Args>
     void emplace_back(Args&&... args);
     iterator erase(const_iterator pos);
     iterator erase(const_iterator first, const_iterator last);
     void pop_back();
     void swap(vector &x);
     void clear() noexcept;


### Benchmarks vs STL vector
   
   ![vector benchmarks](https://github.com/kchasialis/STLContainers/blob/master/benchmarks/vector_benchmarks.png)
    
## adt::list

list is implemented using doubly-linked list with a sentinel node, allowing for fast insertion on both ends
(front, back) and generally anywhere in the container, because inserting an element is as easy as
changing the value of some pointers (O(1) operation).  

### adt::list iterators
list's iterators are bidirectional iterators.

#### Properties
    * default-constructible, copy-constructible, copy-assignable and destructible  
    * Can be dereferenced as an rvalue (if in a dereferenceable state).  
    * Can be compared for equivalence using the equality/inequality operators
    (meaningful when both iterator values iterate over the same underlying sequence)
    * Can be incremented (if in a dereferenceable state).
    * The result is either also dereferenceable or a past-the-end iterator.
    * Two iterators that compare equal, keep comparing equal after being both increased
    * Can be decremented (if a dereferenceable iterator value precedes it).
    * Lvalues are swappable.

### adt::list public API:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

    /* Constructors/Destructors.  */
    list();
    list(size_type n, const value_type &val);
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

### Benchmarks vs STL list
   ![list benchmarks](https://github.com/kchasialis/STLContainers/blob/master/benchmarks/list_benchmarks.png)

## adt::set

sets are unique-element, sorted containers and are implemented using self-balancing binary search trees 
(red black trees).

### adt::set iterators
set's iterators are bidirectional iterators.

#### Properties
    * default-constructible, copy-constructible, copy-assignable and destructible  
    * Can be dereferenced as an rvalue (if in a dereferenceable state).  
    * Can be compared for equivalence using the equality/inequality operators
    (meaningful when both iterator values iterate over the same underlying sequence)
    * Can be incremented (if in a dereferenceable state).
    * The result is either also dereferenceable or a past-the-end iterator.
    * Two iterators that compare equal, keep comparing equal after being both increased
    * Can be decremented (if a dereferenceable iterator value precedes it).
    * Lvalues are swappable.

### adt::set public API:
    using key_type = Key;
    using value_type = Key;
    using key_compare = Less;
    using value_compare = Less;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

    /* Constructors/Destructors.  */
    set(const key_compare &keq = key_compare()) noexcept;
    explicit set(const set &other) noexcept;
    set(set&& other) noexcept;
    ~set();
    set &operator=(set rhs);

    /* Iterators.  */
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;

    /* Capacity.  */
    bool empty() const noexcept;
    size_type size() const noexcept;

    /* Observers.  */
    key_compare key_comp() const;
    value_compare value_comp() const;

    /* Modifiers.  */
    std::pair<iterator, bool> insert(const value_type &val);
    std::pair<iterator, bool> insert(value_type &&val);
    template <class... Args>
    std::pair<iterator, bool> emplace(Args &&... args);
    iterator erase(const_iterator pos);
    size_type erase(const value_type &val);
    iterator erase(const_iterator first, const_iterator last);
    void clear() noexcept;
    void swap(set &other);

    /* Operations.  */
    iterator find(const key_type &key);
    const_iterator find(const key_type &key) const;
    size_type count(const key_type &key) const;
    iterator lower_bound(const key_type &key);
    const_iterator lower_bound(const key_type &key) const;
    iterator upper_bound(const key_type &key);
    const_iterator upper_bound(const key_type &key) const;
    std::pair<iterator, iterator> equal_range(const key_type &key);
    std::pair<const_iterator, const_iterator> equal_range(const key_type &key) const;

### Benchmarks vs STL set
   ![set benchmarks](https://github.com/kchasialis/STLContainers/blob/master/benchmarks/set_benchmarks.png)

## adt::multiset

multisets are sorted containers that allow existence of same key elements and are implemented using self-balancing binary search trees 
(red black trees).  
In order to allow store multiple values with same key, each red black node
has a member that points to the head of a list that contains the keys.

### adt::multiset iterators
multiset's iterators are bidirectional iterators.

#### Properties
    * default-constructible, copy-constructible, copy-assignable and destructible  
    * Can be dereferenced as an rvalue (if in a dereferenceable state).  
    * Can be compared for equivalence using the equality/inequality operators
    (meaningful when both iterator values iterate over the same underlying sequence)
    * Can be incremented (if in a dereferenceable state).
    * The result is either also dereferenceable or a past-the-end iterator.
    * Two iterators that compare equal, keep comparing equal after being both increased
    * Can be decremented (if a dereferenceable iterator value precedes it).
    * Lvalues are swappable.

### adt::multiset public API:
    using key_type = Key;
    using value_type = Key;
    using key_compare = Less;
    using value_compare = Less;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

    /* Constructors/Destructors.  */
    multiset(const key_compare &keq = key_compare()) noexcept;
    explicit multiset(const multiset &other) noexcept;
    multiset(multiset &&other) noexcept;
    ~multiset();
    multiset &operator=(multiset rhs);

    /* Iterators.  */
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;

    /* Capacity.  */
    bool empty() const noexcept;
    size_type size() const noexcept;

    /* Observers.  */
    key_compare key_comp() const;
    value_compare value_comp() const;

    /* Modifiers.  */
    iterator insert(const value_type &val);
    iterator insert(value_type &&val);
    template <class... Args>
    iterator emplace(Args &&... args);
    iterator erase(const_iterator pos);
    size_type erase(const value_type &val);
    iterator erase(const_iterator first, const_iterator last);
    void clear() noexcept;
    void swap(multiset &other);

    /* Operations.  */
    iterator find(const key_type &key);
    const_iterator find(const key_type &key) const;
    size_type count(const key_type &key) const;
    iterator lower_bound(const key_type &key);
    const_iterator lower_bound(const key_type &key) const;
    iterator upper_bound(const key_type &key);
    const_iterator upper_bound(const key_type &key) const;
    std::pair<iterator, iterator> equal_range(const key_type &key);
    std::pair<const_iterator, const_iterator> equal_range(const key_type &key) const;

### Benchmarks vs STL multiset
   ![multiset benchmarks](https://github.com/kchasialis/STLContainers/blob/master/benchmarks/multiset_benchmarks.png)

## adt::map

maps are associative (key-value), unique-element, sorted containers 
and are implemented using self-balancing binary search trees 
(red black trees).
  
### adt::map iterators

maps's iterators are bidirectional iterators.

#### Properties
    * default-constructible, copy-constructible, copy-assignable and destructible  
    * Can be dereferenced as an rvalue (if in a dereferenceable state).  
    * Can be compared for equivalence using the equality/inequality operators
    (meaningful when both iterator values iterate over the same underlying sequence)
    * Can be incremented (if in a dereferenceable state).
    * The result is either also dereferenceable or a past-the-end iterator.
    * Two iterators that compare equal, keep comparing equal after being both increased
    * Can be decremented (if a dereferenceable iterator value precedes it).
    * Lvalues are swappable.

### adt::map public API:
    using key_type = K;
    using mapped_type = V;
    using value_type = std::pair<const K, V>;
    using key_compare = Less;
    using value_compare = Less;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

    /* Constructors/Destructors.  */
    map(const key_compare &keq = key_compare()) noexcept;
    explicit map(const map &other) noexcept;
    map(map &&other) noexcept;
    ~map();
    map &operator=(map rhs);

    /* Iterators.  */
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;

    /* Capacity.  */
    bool empty() const noexcept;
    size_type size() const noexcept;

    /* Observers.  */
    key_compare key_comp() const;
    value_compare value_comp() const;

    /* Element access.  */
    mapped_type &operator[](const key_type &key);
    mapped_type &operator[](key_type &&key);
    mapped_type &at(const key_type &key) noexcept(false);
    const mapped_type &at(const key_type &key) const noexcept(false);

    /* Modifiers.  */
    std::pair<iterator, bool> insert(const value_type &val);
    template<class P>
    std::pair<iterator, bool> insert(P &&val);
    template<class... Args>
    std::pair<iterator, bool> emplace(Args&&... args);
    iterator erase(const_iterator pos);
    size_type erase(const key_type &key);
    iterator erase(const_iterator first, const_iterator last);
    void clear() noexcept;
    void swap(map &x);

    /* Operations.  */
    iterator find(const key_type &key);
    const_iterator find(const key_type &key) const;
    size_type count(const key_type &key) const;
    iterator lower_bound(const key_type &key);
    const_iterator lower_bound(const key_type &key) const;
    iterator upper_bound(const key_type &key);
    const_iterator upper_bound(const key_type &key) const;
    std::pair<iterator, iterator> equal_range(const key_type &key);
    std::pair<const_iterator, const_iterator> equal_range(const key_type &key) const;

### Benchmarks vs STL map
   ![map benchmarks](https://github.com/kchasialis/STLContainers/blob/master/benchmarks/map_benchmarks.png)

## adt::multimap

multimaps are associative (key-value), sorted containers 
that allow existence of same key-value elements
and are implemented using self-balancing binary search trees 
(red black trees).  
In order to allow store multiple values with same key, each red black node
has a member that points to the head of a list that contains the keys.
  
### adt::multimap iterators

multimap's iterators are bidirectional iterators.

#### Properties
    * default-constructible, copy-constructible, copy-assignable and destructible  
    * Can be dereferenced as an rvalue (if in a dereferenceable state).  
    * Can be compared for equivalence using the equality/inequality operators
    (meaningful when both iterator values iterate over the same underlying sequence)
    * Can be incremented (if in a dereferenceable state).
    * The result is either also dereferenceable or a past-the-end iterator.
    * Two iterators that compare equal, keep comparing equal after being both increased
    * Can be decremented (if a dereferenceable iterator value precedes it).
    * Lvalues are swappable.

### adt::multimap public API:
    using key_type = K;
    using mapped_type = V;
    using value_type = std::pair<const K, V>;
    using key_compare = Less;
    using value_compare = Less;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

    /* Constructors/Destructors.  */
    multimap(const key_compare &keq = key_compare()) noexcept;
    multimap(const multimap &other) noexcept;
    multimap(multimap &&other) noexcept;
    ~multimap();
    multimap &operator=(multimap rhs);

    /* Iterators.  */
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;

    /* Capacity.  */
    bool empty() const noexcept;
    size_type size() const noexcept;

    /* Observers.  */
    key_compare key_comp() const;
    value_compare value_comp() const;

    /* Modifiers.  */
    iterator insert(const value_type &val);
    template<class P>
    iterator insert(P &&val, typename std::enable_if<std::is_constructible<P&&, value_type>::value, enabler>::type = enabler());
    template <class... Args>
    iterator emplace(Args &&... args);
    iterator erase(const_iterator pos);
    size_type erase(const value_type &val);
    iterator erase(const_iterator first, const_iterator last);
    void clear() noexcept;
    void swap(multimap &other);

    /* Operations.  */
    iterator find(const key_type &key);
    const_iterator find(const key_type &key) const;
    size_type count(const key_type &key) const;
    iterator lower_bound(const key_type &key);
    const_iterator lower_bound(const key_type &key) const;
    iterator upper_bound(const key_type &key);
    const_iterator upper_bound(const key_type &key) const;
    std::pair<iterator, iterator> equal_range(const key_type &key);
    std::pair<const_iterator, const_iterator> equal_range(const key_type &key) const;

### Benchmarks vs STL multimap
   ![multimap benchmarks](https://github.com/kchasialis/STLContainers/blob/master/benchmarks/multimap_benchmarks.png)

## adt::unordered_set

unordered_sets are unique-element containers
but they do not follow any particular order (unordered).  
They are implemented using hash tables.  
These hash tables do not use separate chaining to 
resolve collisions and thats the main difference
with STL's unordered_set.  
In order to resolve collisions, this hash table
uses the open-addressing with linear probe approach 
where if 2 elements collide in the same slot they are
linearly probed to the next slot.  

An important feature of this hash table is that it uses
2 arrays to store the elements.  
The first array contains the "control bytes" (_ctrls)
that is basically an array that has metadata about the
array that contains tha actual data.  
The metadata include information such is if a slot
is filled, empty or deleted.  
The reason behind this is that when we search for an
element in the hash table, most of the probing
happens in the control bytes, and this is actual
an array of bytes (int8_t), and a big part of it
fits directly in cache, which means that most of the probing
happens in cache and is very efficient because
we avoid memory accesses on the elements of the actual array
that may be big such as a whole vector.  
Another important feature to mention is the way the hashing
works.  
When we have an element, (h1_hash) we get a somewhat random
64-bit number. The way the "randomness" works is that
we initialize a seeder with the address of the control-bytes
array and we use this seed to add more entropy to the hashed value  
When we find an empty slot (using _ctrls), we insert
the element there. 
But what do we store to the ctrls array in order to search
for this element that we have inserted afterwards? 
We insert the value of another function h2 that takes as input
the hash returned from h1 and keeps only the last 7bits
of this hash.  
What this does is that it allows us to probe only on
the _ctrls array (i.e. in cache)
without looking at _slots array that maybe cause inefficiency.  

The inspiration behind this implementation is this [talk](https://www.youtube.com/watch?v=ncHmEUmJZf4&t=1765s)
from CppCon.  

```
example pseudocode of find(key):

h1_hash = h1(hash);
pos = h1_hash % capacity;  
while (1) {
    if (is_empty(_ctrls[pos])) {
        return <code that handles if an element does not exist>
    }
    else if (is_deleted(_ctrls[pos])) {
       pos = probe(pos);
       continue;
    }
    else if (_ctrls[pos] == h2_hash) {
        // Notice how much of the probing happens in the _ctrls (cache efficiency)
        if (_slots[pos] equals key)  {
            return <code that handles element that does exist>
        } else {
            pos = probe(pos);
        }
    }
}
```
 
### adt::unordered_set iterators
unordered_set's iterators are forward iterators.

#### Properties
    * default-constructible, copy-constructible, copy-assignable and destructible  
    * Can be dereferenced as an rvalue (if in a dereferenceable state).  
    * Can be compared for equivalence using the equality/inequality operators
    (meaningful when both iterator values iterate over the same underlying sequence)
    * Can be incremented (if in a dereferenceable state).
    * The result is either also dereferenceable or a past-the-end iterator.
    * Two iterators that compare equal, keep comparing equal after being both increased
    * Can be dereferenced as an lvalue (if in a dereferenceable state).
    
### adt::unordered_set public API:
    using key_type = Key;
    using value_type = Key;
    using hasher = Hash;
    using key_equal = Eq;
    using pointer = value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using const_pointer = const value_type*;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
    
    /* Constructors/Destructors.  */
    explicit unordered_set(size_type cap = 16, const hasher& hash = hasher(),
                           const key_equal& keq = key_equal()) noexcept;
    unordered_set(const unordered_set &other);
    unordered_set(unordered_set &&other) noexcept;
    unordered_set& operator=(unordered_set rhs);
    ~unordered_set() noexcept;

    /* Capacity.  */
    bool empty() const noexcept;
    size_type size() const noexcept;

    /* Iterators.  */
    iterator begin() const noexcept;
    iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    /* Observers.  */
    hasher hash_function() const;
    key_equal key_eq() const;

    /* Modifiers.  */
    std::pair<iterator, bool> insert(const value_type &val);
    std::pair<iterator, bool> insert(value_type &&val);
    template <class... Args>
    std::pair<iterator, bool> emplace(Args&&... args);
    iterator erase(const_iterator pos);
    size_type erase(const value_type &val);
    iterator erase(const_iterator first, const_iterator last);
    void clear() noexcept;
    void swap(unordered_set& other);

    /* Element lookup.  */
    iterator find(const key_type &key);
    const_iterator find(const key_type &key) const;
    size_type count(const key_type &key) const;
    std::pair<iterator, iterator> equal_range(const key_type &key);
    std::pair<const_iterator, const_iterator> equal_range(const key_type &key) const;

### Benchmarks vs STL unordered_set
   ![unordered_set benchmarks](https://github.com/kchasialis/STLContainers/blob/master/benchmarks/unordered_set_benchmarks.png)

## adt::unordered_multiset

The internal implementation is the same as unordered_set
but we allow elements with equivalents keys to exist in the container.

### adt::unordered_multiset iterators
unordered_multiset's iterators are forward iterators.

#### Properties
    * default-constructible, copy-constructible, copy-assignable and destructible  
    * Can be dereferenced as an rvalue (if in a dereferenceable state).  
    * Can be compared for equivalence using the equality/inequality operators
    (meaningful when both iterator values iterate over the same underlying sequence)
    * Can be incremented (if in a dereferenceable state).
    * The result is either also dereferenceable or a past-the-end iterator.
    * Two iterators that compare equal, keep comparing equal after being both increased
    * Can be dereferenced as an lvalue (if in a dereferenceable state).
    
### adt::unordered_multiset public API:
        using key_type = Key;
        using value_type = Key;
        using hasher = Hash;
        using key_equal = Eq;
        using pointer = value_type *;
        using reference = value_type &;
        using const_reference = const value_type &;
        using const_pointer = const value_type *;
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        
        /* Constructors/Destructors.  */
        explicit unordered_multiset(size_type cap = 16, const hasher &hash = hasher(),
                                    const key_equal &keq = key_equal()) noexcept;
        unordered_multiset(const unordered_multiset &other);
        unordered_multiset(unordered_multiset &&other) noexcept;
        unordered_multiset &operator=(unordered_multiset rhs);
        ~unordered_multiset() noexcept;

        /* Capacity.  */
        bool empty() const noexcept;
        size_type size() const noexcept;

        /* Iterators.  */
        iterator begin() const noexcept;
        iterator end() const noexcept;
        const_iterator cbegin() const noexcept;
        const_iterator cend() const noexcept;

        /* Observers.  */
        hasher hash_function() const;
        key_equal key_eq() const;

        /* Modifiers.  */
        iterator insert(const value_type &val);
        iterator insert(value_type &&val);
        template<class... Args>
        iterator emplace(Args &&... args);
        iterator erase(const_iterator pos);
        size_type erase(const key_type &key);
        iterator erase(const_iterator first, const_iterator last);
        void clear() noexcept;
        void swap(unordered_multiset &other);

        /* Element lookup.  */
        iterator find(const key_type &key);
        const_iterator find(const key_type &key) const;
        size_type count(const key_type &key) const;
        std::pair<iterator, iterator> equal_range(const key_type &key);
        std::pair<iterator, const_iterator> equal_range(const key_type &key) const;

### Benchmarks vs STL unordered_multiset
   ![unordered_multiset benchmarks](https://github.com/kchasialis/STLContainers/blob/master/benchmarks/unordered_multiset_benchmarks.png)


## adt::unordered_map

The internal implementation is the same as unordered_set
but this container is associative (key, value).

### adt::unordered_map iterators
unordered_map's iterators are forward iterators.

#### Properties
    * default-constructible, copy-constructible, copy-assignable and destructible  
    * Can be dereferenced as an rvalue (if in a dereferenceable state).  
    * Can be compared for equivalence using the equality/inequality operators
    (meaningful when both iterator values iterate over the same underlying sequence)
    * Can be incremented (if in a dereferenceable state).
    * The result is either also dereferenceable or a past-the-end iterator.
    * Two iterators that compare equal, keep comparing equal after being both increased
    * Can be dereferenced as an lvalue (if in a dereferenceable state).
    
### adt::unordered_map public API:

    using key_type = K;
    using mapped_type = V;
    using value_type = std::pair<const K, V>;
    using hasher = Hash;
    using key_equal = Eq;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
    
    /* Constructors/Destructors.  */
    explicit unordered_map(size_type cap = 16, const hasher& hash = hasher(),
                       const key_equal& keq = key_equal()) noexcept;
    unordered_map(const unordered_map &other);
    unordered_map(unordered_map &&other) noexcept;
    unordered_map& operator=(unordered_map rhs);
    ~unordered_map() noexcept;
    
    /* Capacity.  */
    bool empty() const noexcept;
    size_type size() const noexcept;
    
    /* Iterators.  */
    iterator begin() const noexcept;
    iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    
    /* Observers.  */
    hasher hash_function() const;
    key_equal key_eq() const;
    
    /* Element lookup.  */
    mapped_type &operator[](const key_type &key);
    mapped_type &operator[](key_type &&k);
    mapped_type &at(const key_type &key) noexcept(false);
    const mapped_type &at(const key_type &key) const noexcept(false);
    
    /* Modifiers.  */
    std::pair<iterator, bool> insert(const value_type &val);
    template<class P>
    std::pair<iterator, bool> insert(P &&val, typename std::enable_if<std::is_constructible<P&&, value_type>::value, enabler>::type = enabler());
    template <class... Args>
    std::pair<iterator, bool> emplace(Args&&... args);
    iterator erase(const_iterator pos);
    size_type erase(const key_type &key);
    iterator erase(const_iterator first, const_iterator last);
    void clear() noexcept;
    void swap(unordered_map &other);
    
    /* Operations.  */
    iterator find(const key_type &key);
    const_iterator find(const key_type &key) const;
    size_type count(const key_type &key) const;
    std::pair<iterator, iterator> equal_range(const key_type &key);
    std::pair<const_iterator, const_iterator> equal_range(const key_type &key) const;

        
### Benchmarks vs STL unordered_map
   ![unordered_map benchmarks](https://github.com/kchasialis/STLContainers/blob/master/benchmarks/unordered_benchmarks.png)

## adt::unordered_multimap

The internal implementation is the same as unordered_map
but this container allows multiple key-value pairs
that have equivalent key.

### adt::unordered_multimap iterators
unordered_multimap's iterators are forward iterators.

#### Properties
    * default-constructible, copy-constructible, copy-assignable and destructible  
    * Can be dereferenced as an rvalue (if in a dereferenceable state).  
    * Can be compared for equivalence using the equality/inequality operators
    (meaningful when both iterator values iterate over the same underlying sequence)
    * Can be incremented (if in a dereferenceable state).
    * The result is either also dereferenceable or a past-the-end iterator.
    * Two iterators that compare equal, keep comparing equal after being both increased
    * Can be dereferenced as an lvalue (if in a dereferenceable state).
    
### adt::unordered_multimap public API:

    using key_type = K;
    using mapped_type = V;
    using value_type = std::pair<const K, V>;
    using hasher = Hash;
    using key_equal = Eq;
    using pointer = value_type *;
    using reference = value_type &;
    using const_reference = const value_type &;
    using const_pointer = const value_type *;
    using difference_type = std::ptrdiff_t;

    /* Constructors/Destructors.  */
    explicit unordered_multimap(size_type cap = 16, const hasher &hash = hasher(),
                                const key_equal &keq = key_equal()) noexcept;
    unordered_multimap(const unordered_multimap &other);
    unordered_multimap(unordered_multimap &&other) noexcept;
    unordered_multimap &operator=(unordered_multimap rhs);
    ~unordered_multimap() noexcept;

    /* Capacity.  */
    bool empty() const noexcept;
    size_type size() const noexcept;

    /* Iterators.  */
    iterator begin() const noexcept;
    iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    /* Observers.  */
    hasher hash_function() const;
    key_equal key_eq() const;

    /* Modifiers.  */
    iterator insert(const value_type &val);
    template<class P>
    iterator insert(P &&val, typename std::enable_if<std::is_constructible<P&&, value_type>::value, enabler>::type = enabler());
    template<class... Args>
    iterator emplace(Args &&... args);
    iterator erase(const_iterator pos);
    size_type erase(const key_type &key);
    iterator erase(const_iterator first, const_iterator last);
    void clear() noexcept;
    void swap(unordered_multimap &other);

    /* Element lookup.  */
    iterator find(const key_type &key);
    const_iterator find(const key_type &key) const;
    size_type count(const key_type &key) const;
    std::pair<iterator, iterator> equal_range(const key_type &key);
    std::pair<iterator, const_iterator> equal_range(const key_type &key) const;

### Benchmarks vs STL unordered_multimap
   ![unordered_multimap benchmarks](https://github.com/kchasialis/STLContainers/blob/master/benchmarks/unordered_multimap_benchmarks.png)

# License

This library is licensed under the terms of the MIT License. 
See LICENSE.md for more information.

# Notes
 Please don't hesitate to contact me if you
 believe the code can be improved or if you spot a bug.
 I will highly appreciate it.  
 
 Mails :
 * koschasialis@gmail.com
 * sdi1600195@di.uoa.gr
 
# Thoughts
 * Maybe implement some extra functionality like more constructors, try_emplace() etc.
 * I believe that a sorted array will be a more efficient implementation (due to 
 cache hit-rate being higher) than rbtrees for sorted containers, maybe try this approach. 
