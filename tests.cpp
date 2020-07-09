#include <algorithm>
#include <iostream>
#include <cassert>
#include <vector>
#include <set>

#include "list.h"
#include "vector.h"
#include "set.h"

void run_list_test() {
    int val;
    size_t loops;
    adt::list<int> myints;
    
    /* push_back test. */
    for (int i = 0 ; i < 1500 ; i++) {
        myints.push_back(i);
    }
    assert(myints.size() == 1500);

    val = 0;
    for (auto it = myints.begin() ; it != myints.end() ; it++, val++) {
        assert(*it == val);
    }

    /* pop_back() test. */
    loops = myints.size();
    for (size_t i = 0 ; i < loops ; i++) {
        myints.pop_back();
    }
    assert(myints.size() == 0);
    assert(myints.begin() == myints.end());

    /* push_front() test. */
    for (int i = 0 ; i < 1500 ; i++) {
        myints.push_front(i);
    }
    assert(myints.size() == 1500);
    
    val = 1499;
    for (auto it = myints.begin() ; it != myints.end() ; it++, val--) {
        assert(*it == val);
    }

    /* pop_front() test. */
    for (size_t i = 0 ; i < loops ; i++) {
        myints.pop_front();
    }
    assert(myints.size() == 0);
    assert(myints.begin() == myints.end());

    for (int i = 0 ; i < 1500 ; i++) {
        myints.push_back(i);
    }

    /* remove() test. */
    val = 1499;
    for (size_t i = 0 ; i < loops ; i++, val--) {
        myints.remove(val);
    }
    assert(myints.size() == 0);
    assert(myints.begin() == myints.end());

    /* remove() test. */
    for (auto it = myints.begin() ; it != myints.end() ; ) {
        it = myints.remove(it);
    }
    assert(myints.size() == 0);
    assert(myints.begin() == myints.end());

    for (int i = 0 ; i < 1500 ; i++) {
        myints.push_back(i);
    }

    /* clear() test. */
    myints.clear();
    assert(myints.size() == 0);
    assert(myints.begin() == myints.end());
    
    for (int i = 0 ; i < 1500 ; i++) {
        myints.push_back(i);
    }

    /* reverse iterators test. */
    val = 1499;
    for (auto it = myints.rbegin() ; it != myints.rend() ; it++, val--) {
        assert(*it == val);
    }

    myints.clear();

    for (size_t i = 0 ; i < 100 ; i++) {
        myints.push_back(150);
    }

    /* remove() test. */
    myints.remove(150);

    assert(myints.size() == 0);
    assert(myints.begin() == myints.end());
}

void run_vector_test() {
    int val;
    size_t loops;
    adt::vector<int> myints;
    
    /*push_back test*/
    for (int i = 0 ; i < 1500 ; i++) {
        myints.push_back(i);
    }
    assert(myints.size() == 1500);

    val = 0;
    for (auto it = myints.begin() ; it != myints.end() ; it++, val++) {
        assert(*it == val);
    }

    /*pop_back() test*/
    loops = myints.size();
    for (size_t i = 0 ; i < loops ; i++) {
        myints.pop_back();
    }
    assert(myints.size() == 0);
    assert(myints.begin() == myints.end());
    assert(myints.rbegin() == myints.rend());

    /* operator[] test. */
    myints.resize(1500);
    for (int i = 0 ; i < 1500 ; i++) {
        myints[i] = i;
    }
    assert(myints.size() == 1500);
    
    val = 0;
    for (auto it = myints.begin() ; it != myints.end() ; it++, val++) {
        assert(*it == val);
    }

    /* pop_front() test. */
    for (size_t i = 0 ; i < loops ; i++) {
        myints.erase(0);
    }
    assert(myints.size() == 0);
    assert(myints.begin() == myints.end());

    for (int i = 0 ; i < 1500 ; i++) {
        myints.push_back(i);
    }

    /* remove() test. */
    for (auto it = myints.begin() ; it != myints.end() ; ) {
        it = myints.erase(it);
    }
    assert(myints.size() == 0);
    assert(myints.begin() == myints.end());

    for (int i = 0 ; i < 1500 ; i++) {
        myints.push_back(i);
    }

    /* clear() test. */
    myints.clear();
    assert(myints.size() == 0);
    assert(myints.begin() == myints.end());
    
    for (int i = 0 ; i < 1500 ; i++) {
        myints.push_back(i);
    }

    /* reverse iterators test. */
    val = 1499;
    for (auto it = myints.rbegin() ; it != myints.rend() ; it++, val--) {
        assert(*it == val);
    }    
}

void run_set_test() {
    adt::set<int> myset;
    std::set<int> mystdset;
    adt::vector<int> numbers_added;
    std::vector<int> sorted_test;
    int num;
    size_t index;
    size_t ceil = 10000;

    srand(time(NULL));

    /* add() test. */
    for (size_t i = 0 ; i < ceil ; i++) {
        myset.add(rand() % ceil);
    }

    for (auto it = myset.begin() ; it != myset.end() ; it++) {
        sorted_test.push_back(*it);
    }
    assert(std::is_sorted(sorted_test.begin(), sorted_test.end()));

    /* remove() test. */
    for (auto it = myset.begin() ; it != myset.end() ; ) {
        it = myset.remove(it);
    }
    assert(myset.size() == 0);
    assert(myset.begin() == myset.end());

    for (size_t i = 0 ; i < ceil ; i++) {
        num = rand() % ceil;
        myset.add(num);
        numbers_added.push_back(num);
    }

    /* search() test. */
    for (auto it = numbers_added.begin() ; it != numbers_added.end() ; it++) {
        assert(*(myset.search(*it)) == *it);
    }

    /* clear() test. */
    myset.clear();
    assert(myset.size() == 0);
    assert(myset.begin() == myset.end());

    for (size_t i = 0 ; i < ceil ; i++) {
        myset.add(i);
        mystdset.insert(i);
    }

    /* lower/upper bound test. */
    auto l_bound = myset.lower_bound(10);
    auto u_bound = myset.upper_bound(150);
    auto l_std_bound = mystdset.lower_bound(10);
    auto u_std_bound = mystdset.upper_bound(150);

    numbers_added.clear();
    for (auto it = l_bound ; it != u_bound ; it++) {
        numbers_added.push_back(*it);
    }    
    index = 0;
    for (auto it = l_std_bound ; it != u_std_bound ; it++) {
        assert(*it == numbers_added[index++]);
    }

    /* reverse iterators test. */
    num = ceil - 1;
    for (auto it = myset.rbegin() ; it != myset.rend() ; it++, num--) {
        assert(*it == num);
    }
}

void run_multiset_test() {

}

void run_map_test() {

}
    
void run_multimap_test() {

}

void run_unordered_set_test() {

}

void run_unordered_map_test() {
    
}

int main() {
    run_list_test();
    run_vector_test();
    run_set_test();
    run_multiset_test();
    run_map_test();
    run_multimap_test();
    run_unordered_set_test();
    run_unordered_map_test();

    return 0;    
}