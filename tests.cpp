#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>
#include <map>
#include <utility>

#include "list.h"
#include "vector.h"
#include "set.h"
#include "multiset.h"
#include "map.h"
#include "multimap.h"
#include "unordered_set.h"
/*
#include "unordered_set.h"
#include "unordered_multiset.h"
#include "unordered_map.h"
#include "unordered_multimap.h"
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

    /* sort() test.  */
    mylist.sort<string_comp>(string_comp());
    assert(std::is_sorted(mylist.begin(), mylist.end()));

    /* reverse() test.  */
    mylist.reverse();

    /* make sure we hve compatibility with STL iterators.  */
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

struct ReverseSorted {
    bool operator()(const int& lhs, const int& rhs) {
        return lhs >= rhs;
    }
    bool operator()(const std::pair<int, std::string> &lhs, const std::pair<int, std::string> &rhs) {
        return lhs.first >= rhs.first;
    }
};

void run_set_test() {
    adt::set<std::string> set_str_test;
    adt::set<int> set_test;
    /* We will use std::set to check our lower/upper_bound functions.  */
    std::set<int> std_set_test;
    size_t sum, test_sum, n_elems_test, index;

    srand(time(nullptr));

    /* insert(), find() test.  */
    sum = 0;
    n_elems_test = 0;
    for (size_t i = 0 ; i < 1500 ; i++) {
        auto val = i;
        if (set_test.find(val) == set_test.end()) {
            auto p = set_str_test.insert(std::to_string(val));
            assert(*(p.first) == std::to_string(val));
            assert(p.second);
            sum += val;
            n_elems_test++;
        }
    }
    assert(set_str_test.size() == n_elems_test);

    /* iterators test.  */
    test_sum = 0;
    for (auto it = set_str_test.begin() ; it != set_str_test.end() ; it++) {
        test_sum += stoi(*it);
    }
    assert(test_sum == sum);

    /* these should not compile, and they dont.
    for (auto it = set_test.begin() ; it != set_test.end() ; it++) {
        *it = std::to_string(15);
    }
    for (auto it = set_test.cbegin() ; it != set_test.cend() ; it++) {
        *it = std::to_string(15);
    }
    for (auto it = set_test.rbegin() ; it != set_test.rend() ; it++) {
        *it = std::to_string(15);
    }
    for (auto it = set_test.crbegin() ; it != set_test.crend() ; it++) {
        *it = std::to_string(15);
    }  */

    /* erase elements until 150.  */
    assert(*(set_str_test.erase(set_str_test.begin(), set_str_test.find("150"))) == "150");

    /* clear() test.  */
    set_str_test.clear();
    assert(set_str_test.empty());
    assert(set_str_test.begin() == set_str_test.end());
    assert(set_str_test.rbegin() == set_str_test.rend());

    /* emplace() test.  */
    sum = 0;
    n_elems_test = 0;
    for (size_t i = 0 ; i < 1500 ; i++) {
        auto val = rand();
        if (set_test.find(val) == set_test.end()) {
            auto p = set_test.emplace(val);
            assert(*(p.first) == val);
            assert(p.second);
            sum += val;
            n_elems_test++;
        }
    }
    assert(set_test.size() == n_elems_test);

    /* find() test.  */
    test_sum = 0;
    for (auto it = set_test.cbegin() ; it != set_test.cend() ; it++) {
        auto p = set_test.find(*it);
        assert(*p == *it);
        test_sum += *p;
    }
    assert(test_sum == sum);

    /* erase() test.  */
    for (auto it = set_test.begin() ; it != set_test.end() ;) {
        it = set_test.erase(it);
    }
    assert(set_test.empty());
    assert(set_test.begin() == set_test.end());
    assert(set_test.rbegin() == set_test.rend());

    for (size_t i = 0 ; i < 1500 ; i++) {
        auto val = rand();
        if (set_test.find(val) == set_test.end()) {
            auto p = set_test.insert(val);
            assert(*(p.first) == val);
            assert(p.second);
        }
    }

    /* erase(range) test.  */
    set_test.erase(set_test.begin(), set_test.end());
    assert(set_test.empty());
    assert(set_test.begin() == set_test.end());
    assert(set_test.rbegin() == set_test.rend());

    for (size_t i = 0 ; i < 1500 ; i++) {
        auto val = i;
        if (set_test.find(val) == set_test.end()) {
            std_set_test.insert(val);
            auto p = set_test.insert(val);
            assert(*(p.first) == val);
            assert(p.second);
        }
    }

    /* lower(), upper() bound check.  */
    auto myset_lower_b = set_test.lower_bound(150);
    auto myset_upper_b = set_test.upper_bound(1500);
    auto stdset_lower_b = std_set_test.lower_bound(150);
    auto stdset_upper_b = std_set_test.upper_bound(1500);

    auto myset_it = myset_lower_b;
    auto stdset_it = stdset_lower_b;
    for ( ; stdset_it != stdset_upper_b ; ++stdset_it, ++myset_it) {
        assert(*myset_it == *stdset_it);
    }

    myset_it = myset_lower_b;
    stdset_it = stdset_lower_b;
    for ( ; myset_it != myset_upper_b ; ++myset_it, ++stdset_it) {
        assert(*myset_it == *stdset_it);
    }

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

    /* sets are sorted, this also asserts compatibility with STL iterators.  */
    assert(std::is_sorted(set_test.begin(), set_test.end()));
    assert(std::is_sorted(set_test.rbegin(), set_test.rend(), ReverseSorted()));
}

void run_multiset_test() {
    adt::multiset<int> multiset_test;
    /* We will use std::multiset to check our lower/upper_bound functions.  */
    std::multiset<int> std_multiset_test;
    size_t sum, test_sum, n_elems_test, index;

    srand(time(nullptr));

    /* insert(), find() test.  */
    sum = 0;
    n_elems_test = 0;
    for (size_t i = 0 ; i < 1500 ; i++) {
        auto val = rand();
        if (multiset_test.find(val) == multiset_test.end()) {
            /* Add 5 times the same value since it allows multiple copies of a key.  */
            for (size_t j = 0 ; j < 5 ; j++) {
                auto p = multiset_test.insert(val);
                assert(*p == val);
                sum += val;
                n_elems_test++;
            }
        }
    }
    assert(multiset_test.size() == n_elems_test);

    /* iterators test.  */
    test_sum = 0;
    for (auto it = multiset_test.begin() ; it != multiset_test.end() ; it++) {
        test_sum += *it;
    }
    assert(test_sum == sum);

    /* these should not compile, and they dont.
    for (auto it = multiset_test.begin() ; it != multiset_test.end() ; it++) {
        *it = 15;
    }
    for (auto it = multiset_test.cbegin() ; it != multiset_test.cend() ; it++) {
        *it = 15;
    }
    for (auto it = multiset_test.rbegin() ; it != multiset_test.rend() ; it++) {
        *it = 15;
    }
    for (auto it = multiset_test.crbegin() ; it != multiset_test.crend() ; it++) {
        *it = 15;
    }   */

    /* clear() test.  */
    multiset_test.clear();
    assert(multiset_test.empty());
    assert(multiset_test.begin() == multiset_test.end());
    assert(multiset_test.rbegin() == multiset_test.rend());

    /* emplace() test.  */
    sum = 0;
    n_elems_test = 0;
    for (size_t i = 0 ; i < 1500 ; i++) {
        auto val = rand();
        if (multiset_test.find(val) == multiset_test.end()) {
            /* Add 5 times the same value since it allows multiple copies of a key.  */
            for (size_t j = 0 ; j < 5 ; j++) {
                auto p = multiset_test.emplace(val);
                assert(*p == val);
                sum += val;
                n_elems_test++;
            }
        }
    }
    assert(multiset_test.size() == n_elems_test);

    /* find() test.  */
    test_sum = 0;
    for (auto it = multiset_test.cbegin() ; it != multiset_test.cend() ; it++) {
        auto p = multiset_test.find(*it);
        assert(*p == *it);
        test_sum += *p;
    }
    assert(test_sum == sum);

    /* erase() test.  */
    for (auto it = multiset_test.begin() ; it != multiset_test.end() ;) {
        it = multiset_test.erase(it);
    }
    assert(multiset_test.empty());
    assert(multiset_test.begin() == multiset_test.end());
    assert(multiset_test.rbegin() == multiset_test.rend());

    for (size_t i = 0 ; i < 1500 ; i++) {
        auto val = rand();
        if (multiset_test.find(val) == multiset_test.end()) {
            auto p = multiset_test.insert(val);
            assert(*p == val);
        }
    }

    /* erase(range) test.  */
    multiset_test.erase(multiset_test.begin(), multiset_test.end());
    assert(multiset_test.empty());
    assert(multiset_test.begin() == multiset_test.end());
    assert(multiset_test.rbegin() == multiset_test.rend());

    for (size_t i = 0 ; i < 1500 ; i++) {
        for (size_t j = 0 ; j < 5 ; j++) {
            std_multiset_test.insert(i);
            auto p = multiset_test.insert(i);
            assert(*p == i);
        }
    }

    /* lower(), upper() bound check.  */
    auto myset_lower_b = multiset_test.lower_bound(150);
    auto myset_upper_b = multiset_test.upper_bound(150);
    auto stdset_lower_b = std_multiset_test.lower_bound(150);
    auto stdset_upper_b = std_multiset_test.upper_bound(150);

    auto myset_it = myset_lower_b;
    auto stdset_it = stdset_lower_b;
    for ( ; stdset_it != stdset_upper_b ; stdset_it++, myset_it++) {
        assert(*myset_it == *stdset_it);
    }

    myset_it = myset_lower_b;
    stdset_it = stdset_lower_b;
    for ( ; myset_it != myset_upper_b ; myset_it++, stdset_it++) {
        assert(*myset_it == *stdset_it);
    }

    /* equal_range() test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        auto myrange = multiset_test.equal_range(i);
        index = 0;
        for (auto it = myrange.first ; it != myrange.second ; it++, index++) {
            assert(*it == i);
        }
        assert(index == 5);
    }

    auto myrange = multiset_test.equal_range(-15);
    assert(myrange.first == multiset_test.end());
    assert(myrange.second == multiset_test.end());

    /* count() test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        assert(multiset_test.count(i) == 5);
    }

    assert(multiset_test.count(-15) == 0);

    /* multisets are sorted, this also asserts compatibility with STL iterators.  */
    assert(std::is_sorted(multiset_test.begin(), multiset_test.end()));
    assert(std::is_sorted(multiset_test.rbegin(), multiset_test.rend(), ReverseSorted()));
}

void run_map_test() {
    adt::map<int, std::string> map_test;
    /* We will use std::map to check our lower/upper_bound functions.  */
    std::map<int, std::string> std_map_test;
    size_t sum, test_sum, n_elems_test, index;

    srand(time(nullptr));

    /* operator[] test.  */
    map_test[3] = "kostas";
    map_test[0] = "lee";
    map_test[2] = "kostaslee";
    map_test[1] = "leekostas";
    assert(map_test[3] == "kostas");
    assert(map_test[0] == "lee");
    assert(map_test[2] == "kostaslee");
    assert(map_test[1] == "leekostas");

    map_test[1] = "changed";
    assert(map_test[1] == "changed");

    map_test.clear();

    /* insert(), find() test.  */
    sum = 0;
    n_elems_test = 0;
    for (size_t i = 0 ; i < 1500 ; i++) {
        auto val = rand();
        if (map_test.find(val) == map_test.end()) {
            auto p = map_test.insert({val, std::to_string(val)});
            assert((p.first)->first == val);
            assert((p.first)->second == std::to_string(val));
            assert(p.second);
            sum += val;
            n_elems_test++;
        }
    }
    assert(map_test.size() == n_elems_test);

    /* iterators test.  */
    test_sum = 0;
    for (auto it = map_test.begin() ; it != map_test.end() ; it++) {
        test_sum += it->first;
    }
    assert(test_sum == sum);

    /* these should not compile, and they dont.
    for (auto it = map_test.begin() ; it != map_test.end() ; it++) {
        it->first = 15;
    }
    for (auto it = map_test.cbegin() ; it != map_test.cend() ; it++) {
        it->first = 150;
        it->second = "random";
    }
    for (auto it = map_test.rbegin() ; it != map_test.rend() ; it++) {
        it->first = 15;
    }
    for (auto it = map_test.crbegin() ; it != map_test.crend() ; it++) {
        it->second = "random1";
    }  */

    /* clear() test.  */
    map_test.clear();
    assert(map_test.empty());
    assert(map_test.begin() == map_test.end());
    assert(map_test.rbegin() == map_test.rend());

    /* emplace() test.  */
    sum = 0;
    n_elems_test = 0;
    for (size_t i = 0 ; i < 1500 ; i++) {
        auto val = rand();
        if (map_test.find(val) == map_test.end()) {
            auto p = map_test.emplace(std::make_pair(val, std::to_string(val)));
            assert((p.first)->first == val);
            assert((p.first)->second == std::to_string(val));
            assert(p.second);
            sum += val;
            n_elems_test++;
        }
    }
    assert(map_test.size() == n_elems_test);

    /* find() test.  */
    test_sum = 0;
    for (auto it = map_test.cbegin() ; it != map_test.cend() ; it++) {
        auto p = map_test.find(it->first);
        assert(p->first == it->first);
        assert(p->second == it->second);
        test_sum += p->first;
    }
    assert(test_sum == sum);

    /* erase() test.  */
    for (auto it = map_test.begin() ; it != map_test.end() ;) {
        it = map_test.erase(it);
    }
    assert(map_test.empty());
    assert(map_test.begin() == map_test.end());
    assert(map_test.rbegin() == map_test.rend());

    for (size_t i = 0 ; i < 1500 ; i++) {
        auto val = rand();
        if (map_test.find(val) == map_test.end()) {
            auto p = map_test.insert({val, std::to_string(val)});
            assert((p.first)->first == val);
            assert((p.first)->second == std::to_string(val));
            assert(p.second);
        }
    }

    /* erase(range) test.  */
    map_test.erase(map_test.begin(), map_test.end());
    assert(map_test.empty());
    assert(map_test.begin() == map_test.end());
    assert(map_test.rbegin() == map_test.rend());

    for (size_t i = 0 ; i < 1500 ; i++) {
        auto val = i;
        if (map_test.find(val) == map_test.end()) {
            std_map_test.insert({val, std::to_string(val)});
            auto p = map_test.insert({val, std::to_string(val)});
        }
    }

    /* lower(), upper() bound check.  */
    auto mymap_lower_b = map_test.lower_bound(150);
    auto mymap_upper_b = map_test.upper_bound(1500);
    auto stdmap_lower_b = std_map_test.lower_bound(150);
    auto stdmap_upper_b = std_map_test.upper_bound(1500);

    auto mymap_it = mymap_lower_b;
    auto stdmap_it = stdmap_lower_b;
    for ( ; stdmap_it != stdmap_upper_b ; ++stdmap_it, ++mymap_it) {
        assert(mymap_it->first == stdmap_it->first);
        assert(mymap_it->second == stdmap_it->second);
    }

    mymap_it = mymap_lower_b;
    stdmap_it = stdmap_lower_b;
    for ( ; mymap_it != mymap_upper_b ; ++mymap_it, ++stdmap_it) {
        assert(mymap_it->first == stdmap_it->first);
        assert(mymap_it->second == stdmap_it->second);
    }

    /* equal_range() test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        auto myrange = map_test.equal_range(i);
        index = 0;
        for (auto it = myrange.first ; it != myrange.second ; it++, index++) {
            assert(it->first == i);
        }
        assert(index == 1);
    }

    auto myrange = map_test.equal_range(-15);
    assert(myrange.first == map_test.end());
    assert(myrange.second == map_test.end());

    /* count() test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        assert(map_test.count(i) == 1);
    }

    assert(map_test.count(-15) == 0);

    /* maps are sorted, this also asserts compatibility with STL iterators.  */
    assert(std::is_sorted(map_test.begin(), map_test.end()));
    assert(std::is_sorted(map_test.rbegin(), map_test.rend(), ReverseSorted()));
}

void run_multimap_test() {
    adt::multimap<int, std::string> multimap_test;
    /* We will use std::multimap to check our lower/upper_bound functions.  */
    std::multimap<int, std::string> std_multimap_test;
    size_t sum, test_sum, n_elems_test, index;

    srand(time(nullptr));

    /* insert(), find() test.  */
    sum = 0;
    n_elems_test = 0;
    for (size_t i = 0 ; i < 1500 ; i++) {
        auto val = rand();
        if (multimap_test.find(val) == multimap_test.end()) {
            /* Add 5 times the same value since it allows multiple copies of a key.  */
            for (size_t j = 0 ; j < 5 ; j++) {
                auto p = multimap_test.insert({val, std::to_string(val)});
                assert(p->first == val);
                assert(p->second == std::to_string(val));
                sum += val;
                n_elems_test++;
            }
        }
    }
    assert(multimap_test.size() == n_elems_test);

    /* iterators test.  */
    test_sum = 0;
    for (auto it = multimap_test.begin() ; it != multimap_test.end() ; it++) {
        test_sum += it->first;
        assert(it->second == std::to_string(it->first));
    }
    assert(test_sum == sum);

    /* these should not compile, and they dont.
    for (auto it = multimap_test.begin() ; it != multimap_test.end() ; it++) {
        it->first = 15;
    }
    for (auto it = multimap_test.cbegin() ; it != multimap_test.cend() ; it++) {
        it->first = 15;
        it->second = "random";
    }
    for (auto it = multimap_test.rbegin() ; it != multimap_test.rend() ; it++) {
        it->first = 15;
    }
    for (auto it = multimap_test.crbegin() ; it != multimap_test.crend() ; it++) {
        it->first = 15;
        it->second = "random";
    }   */

    /* clear() test.  */
    multimap_test.clear();
    assert(multimap_test.empty());
    assert(multimap_test.begin() == multimap_test.end());
    assert(multimap_test.rbegin() == multimap_test.rend());

    /* emplace() test.  */
    sum = 0;
    n_elems_test = 0;
    for (size_t i = 0 ; i < 1500 ; i++) {
        auto val = rand();
        if (multimap_test.find(val) == multimap_test.end()) {
            /* Add 5 times the same value since it allows multiple copies of a key.  */
            for (size_t j = 0 ; j < 5 ; j++) {
                auto p = multimap_test.emplace(std::make_pair(val, std::to_string(val)));
                assert(p->first == val);
                assert(p->second == std::to_string(val));
                sum += val;
                n_elems_test++;
            }
        }
    }
    assert(multimap_test.size() == n_elems_test);

    /* find() test.  */
    test_sum = 0;
    for (auto it = multimap_test.cbegin() ; it != multimap_test.cend() ; it++) {
        auto p = multimap_test.find(it->first);
        assert(p->first == it->first);
        assert(p->second == std::to_string(it->first));
        test_sum += p->first;
    }
    assert(test_sum == sum);

    /* erase() test.  */
    for (auto it = multimap_test.begin() ; it != multimap_test.end() ;) {
        it = multimap_test.erase(it);
    }
    assert(multimap_test.empty());
    assert(multimap_test.begin() == multimap_test.end());
    assert(multimap_test.rbegin() == multimap_test.rend());

    for (size_t i = 0 ; i < 1500 ; i++) {
        auto val = rand();
        if (multimap_test.find(val) == multimap_test.end()) {
            for (size_t j = 0 ; j < 5 ; j++) {
                auto p = multimap_test.insert({val, std::to_string(val)});
                assert(p->first == val);
                assert(p->second == std::to_string(val));
            }
        }
    }

    /* erase(range) test.  */
    multimap_test.erase(multimap_test.begin(), multimap_test.end());
    assert(multimap_test.empty());
    assert(multimap_test.begin() == multimap_test.end());
    assert(multimap_test.rbegin() == multimap_test.rend());

    for (size_t i = 0 ; i < 1500 ; i++) {
        for (size_t j = 0 ; j < 5 ; j++) {
            std_multimap_test.insert({i, std::to_string(i)});
            multimap_test.insert({i, std::to_string(i)});
        }
    }

    /* lower(), upper() bound check.  */
    auto mymultimap_lower_b = multimap_test.lower_bound(150);
    auto mymultimap_upper_b = multimap_test.upper_bound(250);
    auto stdmultimap_lower_b = std_multimap_test.lower_bound(150);
    auto stdmultimap_upper_b = std_multimap_test.upper_bound(250);

    auto mymultimap_it = mymultimap_lower_b;
    auto stdmultimap_it = stdmultimap_lower_b;
    for ( ; stdmultimap_it != stdmultimap_upper_b ; stdmultimap_it++, mymultimap_it++) {
        assert(mymultimap_it->first == stdmultimap_it->first);
        assert(mymultimap_it->second == stdmultimap_it->second);
    }

    mymultimap_it = mymultimap_lower_b;
    stdmultimap_it = stdmultimap_lower_b;
    for ( ; mymultimap_it != mymultimap_upper_b ; mymultimap_it++, stdmultimap_it++) {
        assert(mymultimap_it->first == stdmultimap_it->first);
        assert(mymultimap_it->second == stdmultimap_it->second);
    }

    /* equal_range() test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        auto myrange = multimap_test.equal_range(i);
        index = 0;
        for (auto it = myrange.first ; it != myrange.second ; it++, index++) {
            assert(it->first == i);
            assert(it->second == std::to_string(i));
        }
        assert(index == 5);
    }

    auto myrange = multimap_test.equal_range(-15);
    assert(myrange.first == multimap_test.end());
    assert(myrange.second == multimap_test.end());

    /* count() test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        assert(multimap_test.count(i) == 5);
    }

    assert(multimap_test.count(-15) == 0);

    /* multimaps are sorted, this also asserts compatibility with STL iterators.  */
    assert(std::is_sorted(multimap_test.begin(), multimap_test.end()));
    assert(std::is_sorted(multimap_test.rbegin(), multimap_test.rend(), ReverseSorted()));
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
    run_set_test();
    run_multiset_test();
    run_map_test();
    run_multimap_test();
//    run_unordered_set_test();
//    run_unordered_multiset_test();
//    run_unordered_map_test();
//    run_unordered_multimap_test();

    return 0;    
}