#include <algorithm>
#include <cassert>
#include <iostream>

#include "vector.h"
#include "unordered_set.h"
#include "list.h"
/*
#include "map.h"
#include "set.h"
#include "multiset.h"
#include "multimap.h"
#include "unordered_set.h"
#include "unordered_multiset.h"
#include "unordered_map.h"
#include "unordered_multimap.h"
#include "vector.h"
#include "pqueue.h"
*/

struct string_comp {
    bool operator()(const std::string &s_left, const std::string &s_right) {
        return s_left < s_right;
    }
};

void run_list_test() {
    /* size constructor test.  */
    size_t index;
    adt::list<std::string> mylist(15, "kostas");

    /* size() test.  */
    assert(mylist.size() == 15);
    /* front() test.  */
    assert(mylist.front() == "kostas");
    /* back() test.  */
    assert(mylist.back() == "kostas");

    /* emplace_front() test.  */
    mylist.emplace_front("lee");
    assert(mylist.front() == "lee");

    /* push_front() test.  */
    mylist.push_front("john");
    assert(mylist.front() == "john");

    /* pop_front() test.  */
    mylist.pop_front();
    assert(mylist.front() == "lee");

    /* emplace_back() test.  */
    mylist.emplace_back("mary");
    assert(mylist.back() == "mary");

    /* push_back() test.  */
    mylist.push_back("wicked");
    assert(mylist.back() == "wicked");

    /* pop_back() test.  */
    mylist.pop_back();
    assert(mylist.back() == "mary");

    /* clear() test.  */
    mylist.clear();
    assert(mylist.begin() == mylist.end());
    assert(mylist.rbegin() == mylist.rend());
    assert(mylist.empty());

    /* emplace() test.  */
    auto emplace_it = mylist.begin();
    for (size_t i = 0; i < 1500; i++, emplace_it++) {
        emplace_it = mylist.emplace(emplace_it, "kostas" + std::to_string(i));
        assert(*emplace_it == "kostas" + std::to_string(i));
    }
    assert(mylist.size() == 1500);

    /* iterator() test.  */
    index = 0;
    for (auto it = mylist.cbegin(); it != mylist.cend(); it++, index++) {
        assert(*it == "kostas" + std::to_string(index));
    }

    /* this should not compile, and it doesnt.
    for (auto it = mylist.cbegin() ; it != mylist.cend() ; it++) {
        *it = "lee";
    }  */

    /* reverse_iterator() test.  */
    index = 1499;
    for (auto it = mylist.crbegin(); it != mylist.crend(); it++, index--) {
        assert(*it == "kostas" + std::to_string(index));
    }

    mylist.clear();
    assert(mylist.begin() == mylist.end());
    assert(mylist.rbegin() == mylist.rend());
    assert(mylist.empty());

    /* insert() test.  */
    auto insert_it = mylist.insert(mylist.begin(), 1500, "lee");
    for (auto it = insert_it; it != mylist.end(); it++) {
        assert(*it == "lee");
    }
    assert(mylist.size() == 1500);

    /* remove() test.  */
    mylist.remove("lee");
    mylist.clear();
    assert(mylist.begin() == mylist.end());
    assert(mylist.rbegin() == mylist.rend());
    assert(mylist.empty());

    insert_it = mylist.begin();
    for (size_t i = 0; i < 1500; i++, insert_it++) {
        insert_it = mylist.insert(insert_it, "lee");
        assert(*insert_it == "lee");
    }
    assert(mylist.size() == 1500);

    for (auto it = mylist.begin(); it != mylist.end();) {
        it = mylist.erase(it);
    }
    assert(mylist.begin() == mylist.end());
    assert(mylist.rbegin() == mylist.rend());
    assert(mylist.empty());

    insert_it = mylist.begin();
    for (size_t i = 0; i < 1500; i++, insert_it++) {
        insert_it = mylist.insert(insert_it, "lee");
        assert(*insert_it == "lee");
    }
    assert(mylist.size() == 1500);

    /* erase() range test.  */
    mylist.erase(mylist.cbegin(), mylist.cend());
    mylist.clear();
    assert(mylist.begin() == mylist.end());
    assert(mylist.rbegin() == mylist.rend());
    assert(mylist.empty());

    for (size_t i = 0; i < 1500; i++) {
        mylist.push_back("lee");
    }
    assert(mylist.size() == 1500);

    /* unique() test.  */
    mylist.unique();
    assert(mylist.size() == 1);
    assert(mylist.front() == mylist.back());
    assert(*mylist.begin() == "lee");

    mylist.erase(mylist.begin());
    assert(mylist.begin() == mylist.end());
    assert(mylist.rbegin() == mylist.rend());
    assert(mylist.empty());

    for (size_t i = 0 ; i < 1500 ; i++) {
        mylist.push_back("lee" + std::to_string(i));
    }
    assert(mylist.size() == 1500);

    /* sort() test.
     * TODO: sort() goes into inf loop, fix it.
     * */
    mylist.sort<string_comp>(string_comp());
    assert(std::is_sorted(mylist.begin(), mylist.end()));

    /* reverse() test.  */
    mylist.reverse();
    assert(std::is_sorted(mylist.rbegin(), mylist.rend()));
}

void run_vector_test() {
    /* size constructor test.  */
    adt::vector<int> myints(3);
    size_t sum, test_sum;

    srand(time(NULL));

    assert(myints.size() == 3);

    /* data() test.  */
    int *p = myints.data();
    *p = 10;
    ++p;
    *p = 20;
    --p;
    p[2] = 100;

    /* operator[] test.  */
    assert(myints[0] == 10);
    assert(myints[1] == 20);
    assert(myints[2] == 100);

    /* front() test.  */
    assert(myints.front() == 10);

    /* back() test.  */
    assert(myints.back() == 100);

    /* clear() test.  */
    myints.clear();
    assert(myints.begin() == myints.end());
    assert(myints.rbegin() == myints.rend());
    assert(myints.empty());

    /* push_back() test.  */
    sum = 0;
    for (size_t i = 0 ; i < 1500 ; i++) {
        auto item = rand() % 1500;
        myints.push_back(item);
        sum += item;
    }
    assert(myints.size() == 1500);

    /* iterators test.  */
    test_sum = 0;
    for (auto it = myints.begin() ; it != myints.end() ; it++) {
        test_sum += *it;
    }
    assert(sum == test_sum);

    /* this should not compile, and it doesnt.
    for (auto it = myints.cbegin() ; it != myints.cend() ; it++) {
        *it = 15;
    }  */

    myints.clear();
    assert(myints.begin() == myints.end());
    assert(myints.rbegin() == myints.rend());
    assert(myints.empty());

    /* assign() test.  */
    myints.assign({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15});

    auto item = 15;
    for (auto it = myints.rbegin() ; it != myints.rend() ; it++) {
        assert(*it == item--);
    }

    myints.clear();
    assert(myints.begin() == myints.end());
    assert(myints.rbegin() == myints.rend());
    assert(myints.empty());

    /* insert() test.  */
    auto insert_it = myints.begin();
    sum = 0;
    for (size_t i = 0 ; i < 1500 ; i++, insert_it++) {
        insert_it = myints.insert(insert_it, i);
        assert(*insert_it == i);
        sum += i;
    }
    assert(myints.size() == 1500);

    /* size() test.  */
    test_sum = 0;
    for (size_t i = 0 ; i < myints.size() ; i++) {
        test_sum += myints[i];
    }
    assert(sum == test_sum);

    myints.clear();
    assert(myints.begin() == myints.end());
    assert(myints.rbegin() == myints.rend());
    assert(myints.empty());

    /* insert() test.  */
    insert_it = myints.begin();
    sum = 0;
    for (size_t i = 0 ; i < 1500 ; i++, insert_it++) {
        insert_it = myints.emplace(insert_it, i);
        assert(*insert_it == i);
        sum += i;
    }
    assert(myints.size() == 1500);

    /* size() test.  */
    test_sum = 0;
    for (size_t i = 0 ; i < myints.size() ; i++) {
        test_sum += myints[i];
    }
    assert(sum == test_sum);

    /* erase() test.  */
    for (auto it = myints.cbegin() ; it != myints.cend() ; ) {
        it = myints.erase(it);
    }
    assert(myints.begin() == myints.end());
    assert(myints.rbegin() == myints.rend());
    assert(myints.empty());

    for (size_t i = 0 ; i < 1500 ; i++) {
        myints.push_back(i);
    }
    assert(myints.size() == 1500);

    /* pop_back() test.  */
    auto loops = myints.size();
    for (size_t i = 0 ; i < loops ; i++) {
        myints.pop_back();
    }
    assert(myints.begin() == myints.end());
    assert(myints.rbegin() == myints.rend());
    assert(myints.empty());

    /* resize() test.  */
    myints.resize(1500);

    /* operator[] assignment test.  */
    sum = 0;
    for (size_t i = 0 ; i < 1500 ; i++) {
        myints[i] = i;
        sum += i;
    }
    assert(myints.size() == 1500);

    test_sum = 0;
    for (auto it = myints.cbegin() ; it != myints.cend() ; it++) {
        test_sum += *it;
    }
    assert(sum == test_sum);

    /* erase() range test.  */
    myints.erase(myints.cbegin(), myints.cend());

    assert(myints.begin() == myints.end());
    assert(myints.rbegin() == myints.rend());
    assert(myints.empty());

    /* resize() with a val test.  */
    myints.resize(1500, 500);
    for (auto it = myints.begin() ; it != myints.end() ; it++) {
        assert(*it == 500);
    }
    assert(myints.size() == 1500);
    assert(myints.front() == myints.back());

    myints.erase(myints.begin(), myints.end());

    assert(myints.begin() == myints.end());
    assert(myints.rbegin() == myints.rend());
    assert(myints.empty());

    /* emplace_back() test.  */
    sum = 0;
    for (size_t i = 0 ; i < 1500 ; i++) {
        auto tmp = rand();
        myints.emplace_back(tmp);
        sum += tmp;
    }
    assert(myints.size() == 1500);

    /* const_reverse_iterator() test.  */
    test_sum = 0;
    for (auto it = myints.crbegin() ; it != myints.crend() ; it++) {
        test_sum += *it;
    }
    assert(test_sum == sum);
}

void run_unordered_set_test() {
    adt::unordered_set<int> set_test;
    size_t sum, test_sum, index;

    srand(time(NULL));

    /* insert() test.  */
    sum = 0;
    for (size_t i = 0 ; i < 1500 ; i++) {
        auto p = set_test.insert(i);
        assert(p.second);
        assert(*p.first == i);
        sum += i;
    }
    assert(set_test.size() == 1500);

    /* iterators test.  */
    test_sum = 0;
    for (auto it = set_test.begin() ; it != set_test.end() ; it++) {
        test_sum += *it;
    }
    assert(sum == test_sum);

    /* find() test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        assert(*set_test.find(i) == i);
    }

    /* this should not compile, keys are read-only.
    for (size_t i = 0 ; i < 1500 ; i++) {
        assert(*test_test.find(i) = i + 1);
    }*/

    /* clear() test.  */
    set_test.clear();
    assert(set_test.begin() == set_test.end());
    assert(set_test.empty());

    /* emplace() test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        auto p = set_test.emplace(i);
        assert(p.second);
        assert(*p.first == i);
    }
    assert(set_test.size() == 1500);

    /* erase() test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        assert(set_test.erase(i) == 1);
    }
    assert(set_test.begin() == set_test.end());
    assert(set_test.empty());

    for (size_t i = 0 ; i < 1500 ; i++) {
        int trigger_const_ref = i;
        auto p = set_test.insert(trigger_const_ref);
        assert(p.second);
        assert(*p.first == trigger_const_ref);
    }
    assert(set_test.size() == 1500);

    /* equal_range() test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        auto myrange = set_test.equal_range(i);
        index = 0;
        for (auto it = myrange.first ; it != myrange.second ; it++, index++) {
            assert(*it == i);
        }
        assert(index == 1);
    }

    auto myrange = set_test.equal_range(-15);
    assert(myrange.first == set_test.end());
    assert(myrange.second == set_test.end());

    /* count() test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        assert(set_test.count(i) == 1);
    }

    assert(set_test.count(-15) == 0);

    index = 0;
    for (auto it = set_test.begin() ; it != set_test.end() ; index++) {
        /* this also asserts that we can convert iterator to const_iterator.  */
        it = set_test.erase(it);
    }
    assert(set_test.begin() == set_test.end());
    assert(set_test.empty());

    for (size_t i = 0 ; i < 1500 ; i++) {
        auto p = set_test.insert(i);
        assert(p.second);
        assert(*p.first == i);
    }

    /* This should not compile, and it doesnt.
    for (auto it = set_test.begin() ; it != set_test.end() ; it++) {
        *it = 15;
    }  */
}

int main() {
    run_list_test();
    run_vector_test();
//    run_set_test();
//    run_multiset_test();
//    run_map_test();
//    run_multimap_test();
    run_unordered_set_test();
//    run_unordered_multiset_test();
//    run_unordered_map_test();
//    run_unordered_multimap_test();

    return 0;    
}