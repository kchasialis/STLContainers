#include <algorithm>
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
#include "unordered_multiset.h"
#include "unordered_map.h"
#include "unordered_multimap.h"
#include "pqueue.h"

#define CONTAINERS_ASSERT(cond)                                            \
    do {                                                                         \
        if (!(cond)) throw std::runtime_error(std::string(__FILE__)              \
                                            + std::string(":")                   \
                                            + std::to_string(__LINE__)           \
                                            + std::string(" in ")                \
                                            + std::string(__PRETTY_FUNCTION__)); \
    } while(0)

/* The number of elements to add in each container.  */
#define ELEMENTS 3000
/* The extra number of elements to add in multi* containers.  */
#define EXTRA_ELEMENTS 5

void run_list_test() {
    /* size constructor test.  */
    size_t index;
    adt::list<std::string> mylist(15, "kostas");

    /* size() test.  */
    CONTAINERS_ASSERT(mylist.size() == 15);
    /* front() test.  */
    CONTAINERS_ASSERT(mylist.front() == "kostas");
    /* back() test.  */
    CONTAINERS_ASSERT(mylist.back() == "kostas");

    /* emplace_front() test.  */
    mylist.emplace_front("lee");
    CONTAINERS_ASSERT(mylist.front() == "lee");

    /* push_front() test.  */
    mylist.push_front("john");
    CONTAINERS_ASSERT(mylist.front() == "john");

    /* pop_front() test.  */
    mylist.pop_front();
    CONTAINERS_ASSERT(mylist.front() == "lee");

    /* emplace_back() test.  */
    mylist.emplace_back("mary");
    CONTAINERS_ASSERT(mylist.back() == "mary");

    /* push_back() test.  */
    mylist.push_back("wicked");
    CONTAINERS_ASSERT(mylist.back() == "wicked");

    /* pop_back() test.  */
    mylist.pop_back();
    CONTAINERS_ASSERT(mylist.back() == "mary");

    /* clear() test.  */
    mylist.clear();
    CONTAINERS_ASSERT(mylist.begin() == mylist.end());
    CONTAINERS_ASSERT(mylist.rbegin() == mylist.rend());
    CONTAINERS_ASSERT(mylist.empty());

    /* emplace() test.  */
    auto emplace_it = mylist.begin();
    for (size_t i = 0; i < ELEMENTS; i++, emplace_it++) {
        emplace_it = mylist.emplace(emplace_it, "kostas" + std::to_string(i));
        CONTAINERS_ASSERT(*emplace_it == "kostas" + std::to_string(i));
    }
    CONTAINERS_ASSERT(mylist.size() == ELEMENTS);

    /* iterator() test.  */
    index = 0;
    for (auto it = mylist.cbegin(); it != mylist.cend(); it++, index++) {
        CONTAINERS_ASSERT(*it == "kostas" + std::to_string(index));
    }

    /* this should not compile, and it doesnt.
    for (auto it = mylist.cbegin() ; it != mylist.cend() ; it++) {
        *it = "lee";
    }  */

    /* reverse_iterator() test.  */
    index = ELEMENTS - 1;
    for (auto it = mylist.crbegin(); it != mylist.crend(); it++, index--) {
        CONTAINERS_ASSERT(*it == "kostas" + std::to_string(index));
    }

    mylist.clear();
    CONTAINERS_ASSERT(mylist.begin() == mylist.end());
    CONTAINERS_ASSERT(mylist.rbegin() == mylist.rend());
    CONTAINERS_ASSERT(mylist.empty());

    /* insert() test.  */
    auto insert_it = mylist.insert(mylist.begin(), ELEMENTS, "lee");
    for (auto it = insert_it; it != mylist.end(); it++) {
        CONTAINERS_ASSERT(*it == "lee");
    }
    CONTAINERS_ASSERT(mylist.size() == ELEMENTS);

    /* remove() test.  */
    mylist.remove("lee");
    mylist.clear();
    CONTAINERS_ASSERT(mylist.begin() == mylist.end());
    CONTAINERS_ASSERT(mylist.rbegin() == mylist.rend());
    CONTAINERS_ASSERT(mylist.empty());

    insert_it = mylist.begin();
    for (size_t i = 0; i < ELEMENTS; i++, insert_it++) {
        insert_it = mylist.insert(insert_it, "lee");
        CONTAINERS_ASSERT(*insert_it == "lee");
    }
    CONTAINERS_ASSERT(mylist.size() == ELEMENTS);

    for (auto it = mylist.begin(); it != mylist.end();) {
        it = mylist.erase(it);
    }
    CONTAINERS_ASSERT(mylist.begin() == mylist.end());
    CONTAINERS_ASSERT(mylist.rbegin() == mylist.rend());
    CONTAINERS_ASSERT(mylist.empty());

    insert_it = mylist.begin();
    for (size_t i = 0; i < ELEMENTS; i++, insert_it++) {
        insert_it = mylist.insert(insert_it, "lee");
        CONTAINERS_ASSERT(*insert_it == "lee");
    }
    CONTAINERS_ASSERT(mylist.size() == ELEMENTS);

    /* erase() range test.  */
    mylist.erase(mylist.cbegin(), mylist.cend());
    mylist.clear();
    CONTAINERS_ASSERT(mylist.begin() == mylist.end());
    CONTAINERS_ASSERT(mylist.rbegin() == mylist.rend());
    CONTAINERS_ASSERT(mylist.empty());

    for (size_t i = 0; i < ELEMENTS; i++) {
        mylist.push_back("lee");
    }
    CONTAINERS_ASSERT(mylist.size() == ELEMENTS);

    /* unique() test.  */
    mylist.unique();
    CONTAINERS_ASSERT(mylist.size() == 1);
    CONTAINERS_ASSERT(mylist.front() == mylist.back());
    CONTAINERS_ASSERT(*mylist.begin() == "lee");

    mylist.erase(mylist.begin());
    CONTAINERS_ASSERT(mylist.begin() == mylist.end());
    CONTAINERS_ASSERT(mylist.rbegin() == mylist.rend());
    CONTAINERS_ASSERT(mylist.empty());

    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        mylist.push_back("lee" + std::to_string(i));
    }
    CONTAINERS_ASSERT(mylist.size() == ELEMENTS);

    /* sort() test.  */
    mylist.sort();
    CONTAINERS_ASSERT(std::is_sorted(mylist.begin(), mylist.end()));

    /* reverse() test.  */
    mylist.reverse();

    /* make sure we hve compatibility with STL iterators.  */
    CONTAINERS_ASSERT(std::is_sorted(mylist.rbegin(), mylist.rend()));
}

void run_vector_test() {
    /* size constructor test.  */
    adt::vector<int> myints(3);
    size_t sum, test_sum;

    srand(time(NULL));

    CONTAINERS_ASSERT(myints.size() == 3);

    /* data() test.  */
    int *p = myints.data();
    *p = 10;
    ++p;
    *p = 20;
    --p;
    p[2] = 100;

    /* operator[] test.  */
    CONTAINERS_ASSERT(myints[0] == 10);
    CONTAINERS_ASSERT(myints[1] == 20);
    CONTAINERS_ASSERT(myints[2] == 100);

    /* front() test.  */
    CONTAINERS_ASSERT(myints.front() == 10);

    /* back() test.  */
    CONTAINERS_ASSERT(myints.back() == 100);

    /* clear() test.  */
    myints.clear();
    CONTAINERS_ASSERT(myints.begin() == myints.end());
    CONTAINERS_ASSERT(myints.rbegin() == myints.rend());
    CONTAINERS_ASSERT(myints.empty());

    /* push_back() test.  */
    sum = 0;
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto item = rand() % ELEMENTS;
        myints.push_back(item);
        sum += item;
    }
    CONTAINERS_ASSERT(myints.size() == ELEMENTS);

    /* iterators test.  */
    test_sum = 0;
    for (auto it = myints.begin() ; it != myints.end() ; it++) {
        test_sum += *it;
    }
    CONTAINERS_ASSERT(sum == test_sum);

    /* this should not compile, and it doesnt.
    for (auto it = myints.cbegin() ; it != myints.cend() ; it++) {
        *it = 15;
    }  */

    myints.clear();
    CONTAINERS_ASSERT(myints.begin() == myints.end());
    CONTAINERS_ASSERT(myints.rbegin() == myints.rend());
    CONTAINERS_ASSERT(myints.empty());

    /* assign() test.  */
    myints.assign({1, 2, 3, 4, EXTRA_ELEMENTS, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15});

    auto item = 15;
    for (auto it = myints.rbegin() ; it != myints.rend() ; it++) {
        CONTAINERS_ASSERT(*it == item--);
    }

    myints.clear();
    CONTAINERS_ASSERT(myints.begin() == myints.end());
    CONTAINERS_ASSERT(myints.rbegin() == myints.rend());
    CONTAINERS_ASSERT(myints.empty());

    /* insert() test.  */
    auto insert_it = myints.begin();
    sum = 0;
    for (size_t i = 0 ; i < ELEMENTS ; i++, insert_it++) {
        insert_it = myints.insert(insert_it, i);
        CONTAINERS_ASSERT(*insert_it == i);
        sum += i;
    }
    CONTAINERS_ASSERT(myints.size() == ELEMENTS);

    /* size() test.  */
    test_sum = 0;
    for (size_t i = 0 ; i < myints.size() ; i++) {
        test_sum += myints[i];
    }
    CONTAINERS_ASSERT(sum == test_sum);

    myints.clear();
    CONTAINERS_ASSERT(myints.begin() == myints.end());
    CONTAINERS_ASSERT(myints.rbegin() == myints.rend());
    CONTAINERS_ASSERT(myints.empty());

    /* insert() test.  */
    insert_it = myints.begin();
    sum = 0;
    for (size_t i = 0 ; i < ELEMENTS ; i++, insert_it++) {
        insert_it = myints.emplace(insert_it, i);
        CONTAINERS_ASSERT(*insert_it == i);
        sum += i;
    }
    CONTAINERS_ASSERT(myints.size() == ELEMENTS);

    /* size() test.  */
    test_sum = 0;
    for (size_t i = 0 ; i < myints.size() ; i++) {
        test_sum += myints[i];
    }
    CONTAINERS_ASSERT(sum == test_sum);

    /* erase() test.  */
    for (auto it = myints.cbegin() ; it != myints.cend() ; ) {
        it = myints.erase(it);
    }
    CONTAINERS_ASSERT(myints.begin() == myints.end());
    CONTAINERS_ASSERT(myints.rbegin() == myints.rend());
    CONTAINERS_ASSERT(myints.empty());

    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        myints.push_back(i);
    }
    CONTAINERS_ASSERT(myints.size() == ELEMENTS);

    /* pop_back() test.  */
    auto loops = myints.size();
    for (size_t i = 0 ; i < loops ; i++) {
        myints.pop_back();
    }
    CONTAINERS_ASSERT(myints.begin() == myints.end());
    CONTAINERS_ASSERT(myints.rbegin() == myints.rend());
    CONTAINERS_ASSERT(myints.empty());

    /* resize() test.  */
    myints.resize(ELEMENTS);

    /* operator[] assignment test.  */
    sum = 0;
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        myints[i] = i;
        sum += i;
    }
    CONTAINERS_ASSERT(myints.size() == ELEMENTS);

    test_sum = 0;
    for (auto it = myints.cbegin() ; it != myints.cend() ; it++) {
        test_sum += *it;
    }
    CONTAINERS_ASSERT(sum == test_sum);

    /* erase() range test.  */
    myints.erase(myints.cbegin(), myints.cend());

    CONTAINERS_ASSERT(myints.begin() == myints.end());
    CONTAINERS_ASSERT(myints.rbegin() == myints.rend());
    CONTAINERS_ASSERT(myints.empty());

    /* resize() with a val test.  */
    myints.resize(ELEMENTS, 500);
    for (auto it = myints.begin() ; it != myints.end() ; it++) {
        CONTAINERS_ASSERT(*it == 500);
    }
    CONTAINERS_ASSERT(myints.size() == ELEMENTS);
    CONTAINERS_ASSERT(myints.front() == myints.back());

    myints.erase(myints.begin(), myints.end());

    CONTAINERS_ASSERT(myints.begin() == myints.end());
    CONTAINERS_ASSERT(myints.rbegin() == myints.rend());
    CONTAINERS_ASSERT(myints.empty());

    /* emplace_back() test.  */
    sum = 0;
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto tmp = rand();
        myints.emplace_back(tmp);
        sum += tmp;
    }
    CONTAINERS_ASSERT(myints.size() == ELEMENTS);

    /* const_reverse_iterator() test.  */
    test_sum = 0;
    for (auto it = myints.crbegin() ; it != myints.crend() ; it++) {
        test_sum += *it;
    }
    CONTAINERS_ASSERT(test_sum == sum);
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
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto val = i;
        if (set_test.find(val) == set_test.end()) {
            auto p = set_str_test.insert(std::to_string(val));
            CONTAINERS_ASSERT(*(p.first) == std::to_string(val));
            CONTAINERS_ASSERT(p.second);
            sum += val;
            n_elems_test++;
        }
    }
    CONTAINERS_ASSERT(set_str_test.size() == n_elems_test);

    /* iterators test.  */
    test_sum = 0;
    for (auto it = set_str_test.begin() ; it != set_str_test.end() ; it++) {
        test_sum += stoi(*it);
    }
    CONTAINERS_ASSERT(test_sum == sum);

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
    CONTAINERS_ASSERT(*(set_str_test.erase(set_str_test.begin(), set_str_test.find("150"))) == "150");

    /* clear() test.  */
    set_str_test.clear();
    CONTAINERS_ASSERT(set_str_test.empty());
    CONTAINERS_ASSERT(set_str_test.begin() == set_str_test.end());
    CONTAINERS_ASSERT(set_str_test.rbegin() == set_str_test.rend());

    /* emplace() test.  */
    sum = 0;
    n_elems_test = 0;
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto val = rand();
        if (set_test.find(val) == set_test.end()) {
            auto p = set_test.emplace(val);
            CONTAINERS_ASSERT(*(p.first) == val);
            CONTAINERS_ASSERT(p.second);
            sum += val;
            n_elems_test++;
        }
    }
    CONTAINERS_ASSERT(set_test.size() == n_elems_test);

    /* find() test.  */
    test_sum = 0;
    for (auto it = set_test.cbegin() ; it != set_test.cend() ; it++) {
        auto p = set_test.find(*it);
        CONTAINERS_ASSERT(*p == *it);
        test_sum += *p;
    }
    CONTAINERS_ASSERT(test_sum == sum);

    /* erase() test.  */
    for (auto it = set_test.begin() ; it != set_test.end() ;) {
        it = set_test.erase(it);
    }
    CONTAINERS_ASSERT(set_test.empty());
    CONTAINERS_ASSERT(set_test.begin() == set_test.end());
    CONTAINERS_ASSERT(set_test.rbegin() == set_test.rend());

    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto val = rand();
        if (set_test.find(val) == set_test.end()) {
            auto p = set_test.insert(val);
            CONTAINERS_ASSERT(*(p.first) == val);
            CONTAINERS_ASSERT(p.second);
        }
    }

    /* erase(range) test.  */
    set_test.erase(set_test.begin(), set_test.end());
    CONTAINERS_ASSERT(set_test.empty());
    CONTAINERS_ASSERT(set_test.begin() == set_test.end());
    CONTAINERS_ASSERT(set_test.rbegin() == set_test.rend());

    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto val = i;
        std_set_test.insert(val);
        auto p = set_test.insert(val);
        CONTAINERS_ASSERT(*(p.first) == val);
        CONTAINERS_ASSERT(p.second);
    }

    /* lower(), upper() bound check.  */
    auto myset_lower_b = set_test.lower_bound(150);
    auto myset_upper_b = set_test.upper_bound(ELEMENTS);
    auto stdset_lower_b = std_set_test.lower_bound(150);
    auto stdset_upper_b = std_set_test.upper_bound(ELEMENTS);

    auto myset_it = myset_lower_b;
    auto stdset_it = stdset_lower_b;
    for ( ; stdset_it != stdset_upper_b ; ++stdset_it, ++myset_it) {
        CONTAINERS_ASSERT(*myset_it == *stdset_it);
    }

    myset_it = myset_lower_b;
    stdset_it = stdset_lower_b;
    for ( ; myset_it != myset_upper_b ; ++myset_it, ++stdset_it) {
        CONTAINERS_ASSERT(*myset_it == *stdset_it);
    }

    /* equal_range() test.  */
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto myrange = set_test.equal_range(i);
        index = 0;
        for (auto it = myrange.first ; it != myrange.second ; it++, index++) {
            CONTAINERS_ASSERT(*it == i);
        }
        CONTAINERS_ASSERT(index == 1);
    }

    auto myrange = set_test.equal_range(-15);
    CONTAINERS_ASSERT(myrange.first == set_test.end());
    CONTAINERS_ASSERT(myrange.second == set_test.end());

    /* count() test.  */
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        CONTAINERS_ASSERT(set_test.count(i) == 1);
    }

    CONTAINERS_ASSERT(set_test.count(-15) == 0);

    /* sets are sorted, this also asserts compatibility with STL iterators.  */
    CONTAINERS_ASSERT(std::is_sorted(set_test.begin(), set_test.end()));
    CONTAINERS_ASSERT(std::is_sorted(set_test.rbegin(), set_test.rend(), ReverseSorted()));
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
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto val = rand();
        if (multiset_test.find(val) == multiset_test.end()) {
            /* Add EXTRA_ELEMENTS times the same value since it allows multiple copies of a key.  */
            for (size_t j = 0 ; j < EXTRA_ELEMENTS ; j++) {
                auto p = multiset_test.insert(val);
                CONTAINERS_ASSERT(*p == val);
                sum += val;
                n_elems_test++;
            }
        }
    }
    CONTAINERS_ASSERT(multiset_test.size() == n_elems_test);

    /* iterators test.  */
    test_sum = 0;
    for (auto it = multiset_test.begin() ; it != multiset_test.end() ; it++) {
        test_sum += *it;
    }
    CONTAINERS_ASSERT(test_sum == sum);

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
    CONTAINERS_ASSERT(multiset_test.empty());
    CONTAINERS_ASSERT(multiset_test.begin() == multiset_test.end());
    CONTAINERS_ASSERT(multiset_test.rbegin() == multiset_test.rend());

    /* emplace() test.  */
    sum = 0;
    n_elems_test = 0;
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto val = rand();
        if (multiset_test.find(val) == multiset_test.end()) {
            /* Add EXTRA_ELEMENTS times the same value since it allows multiple copies of a key.  */
            for (size_t j = 0 ; j < EXTRA_ELEMENTS ; j++) {
                auto p = multiset_test.emplace(val);
                CONTAINERS_ASSERT(*p == val);
                sum += val;
                n_elems_test++;
            }
        }
    }
    CONTAINERS_ASSERT(multiset_test.size() == n_elems_test);

    /* find() test.  */
    test_sum = 0;
    for (auto it = multiset_test.cbegin() ; it != multiset_test.cend() ; it++) {
        auto p = multiset_test.find(*it);
        CONTAINERS_ASSERT(*p == *it);
        test_sum += *p;
    }
    CONTAINERS_ASSERT(test_sum == sum);

    /* erase() test.  */
    for (auto it = multiset_test.begin() ; it != multiset_test.end() ;) {
        it = multiset_test.erase(it);
    }
    CONTAINERS_ASSERT(multiset_test.empty());
    CONTAINERS_ASSERT(multiset_test.begin() == multiset_test.end());
    CONTAINERS_ASSERT(multiset_test.rbegin() == multiset_test.rend());

    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto val = rand();
        if (multiset_test.find(val) == multiset_test.end()) {
            auto p = multiset_test.insert(val);
            CONTAINERS_ASSERT(*p == val);
        }
    }

    /* erase(range) test.  */
    multiset_test.erase(multiset_test.begin(), multiset_test.end());
    CONTAINERS_ASSERT(multiset_test.empty());
    CONTAINERS_ASSERT(multiset_test.begin() == multiset_test.end());
    CONTAINERS_ASSERT(multiset_test.rbegin() == multiset_test.rend());

    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        for (size_t j = 0 ; j < EXTRA_ELEMENTS ; j++) {
            std_multiset_test.insert(i);
            auto p = multiset_test.insert(i);
            CONTAINERS_ASSERT(*p == i);
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
        CONTAINERS_ASSERT(*myset_it == *stdset_it);
    }

    myset_it = myset_lower_b;
    stdset_it = stdset_lower_b;
    for ( ; myset_it != myset_upper_b ; myset_it++, stdset_it++) {
        CONTAINERS_ASSERT(*myset_it == *stdset_it);
    }

    /* equal_range() test.  */
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto myrange = multiset_test.equal_range(i);
        index = 0;
        for (auto it = myrange.first ; it != myrange.second ; it++, index++) {
            CONTAINERS_ASSERT(*it == i);
        }
        CONTAINERS_ASSERT(index == EXTRA_ELEMENTS);
    }

    auto myrange = multiset_test.equal_range(-15);
    CONTAINERS_ASSERT(myrange.first == multiset_test.end());
    CONTAINERS_ASSERT(myrange.second == multiset_test.end());

    /* count() test.  */
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        CONTAINERS_ASSERT(multiset_test.count(i) == EXTRA_ELEMENTS);
    }

    CONTAINERS_ASSERT(multiset_test.count(-15) == 0);

    /* multisets are sorted, this also asserts compatibility with STL iterators.  */
    CONTAINERS_ASSERT(std::is_sorted(multiset_test.begin(), multiset_test.end()));
    CONTAINERS_ASSERT(std::is_sorted(multiset_test.rbegin(), multiset_test.rend(), ReverseSorted()));
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
    CONTAINERS_ASSERT(map_test[3] == "kostas");
    CONTAINERS_ASSERT(map_test[0] == "lee");
    CONTAINERS_ASSERT(map_test[2] == "kostaslee");
    CONTAINERS_ASSERT(map_test[1] == "leekostas");

    map_test[1] = "changed";
    CONTAINERS_ASSERT(map_test[1] == "changed");

    map_test.clear();

    /* insert(), find() test.  */
    sum = 0;
    n_elems_test = 0;
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto val = rand();
        if (map_test.find(val) == map_test.end()) {
            auto p = map_test.insert({val, std::to_string(val)});
            CONTAINERS_ASSERT((p.first)->first == val);
            CONTAINERS_ASSERT((p.first)->second == std::to_string(val));
            CONTAINERS_ASSERT(p.second);
            sum += val;
            n_elems_test++;
        }
    }
    CONTAINERS_ASSERT(map_test.size() == n_elems_test);

    /* iterators test.  */
    test_sum = 0;
    for (auto it = map_test.begin() ; it != map_test.end() ; it++) {
        test_sum += it->first;
    }
    CONTAINERS_ASSERT(test_sum == sum);

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
    CONTAINERS_ASSERT(map_test.empty());
    CONTAINERS_ASSERT(map_test.begin() == map_test.end());
    CONTAINERS_ASSERT(map_test.rbegin() == map_test.rend());

    /* emplace() test.  */
    sum = 0;
    n_elems_test = 0;
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto val = rand();
        if (map_test.find(val) == map_test.end()) {
            auto p = map_test.emplace(std::make_pair(val, std::to_string(val)));
            CONTAINERS_ASSERT((p.first)->first == val);
            CONTAINERS_ASSERT((p.first)->second == std::to_string(val));
            CONTAINERS_ASSERT(p.second);
            sum += val;
            n_elems_test++;
        }
    }
    CONTAINERS_ASSERT(map_test.size() == n_elems_test);

    /* find() test.  */
    test_sum = 0;
    for (auto it = map_test.cbegin() ; it != map_test.cend() ; it++) {
        auto p = map_test.find(it->first);
        CONTAINERS_ASSERT(p->first == it->first);
        CONTAINERS_ASSERT(p->second == it->second);
        test_sum += p->first;
    }
    CONTAINERS_ASSERT(test_sum == sum);

    /* erase() test.  */
    for (auto it = map_test.begin() ; it != map_test.end() ;) {
        it = map_test.erase(it);
    }
    CONTAINERS_ASSERT(map_test.empty());
    CONTAINERS_ASSERT(map_test.begin() == map_test.end());
    CONTAINERS_ASSERT(map_test.rbegin() == map_test.rend());

    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto val = rand();
        if (map_test.find(val) == map_test.end()) {
            auto p = map_test.insert({val, std::to_string(val)});
            CONTAINERS_ASSERT((p.first)->first == val);
            CONTAINERS_ASSERT((p.first)->second == std::to_string(val));
            CONTAINERS_ASSERT(p.second);
        }
    }

    /* erase(range) test.  */
    map_test.erase(map_test.begin(), map_test.end());
    CONTAINERS_ASSERT(map_test.empty());
    CONTAINERS_ASSERT(map_test.begin() == map_test.end());
    CONTAINERS_ASSERT(map_test.rbegin() == map_test.rend());

    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto val = i;
        if (map_test.find(val) == map_test.end()) {
            std_map_test.insert({val, std::to_string(val)});
            auto p = map_test.insert({val, std::to_string(val)});
        }
    }

    /* lower(), upper() bound check.  */
    auto mymap_lower_b = map_test.lower_bound(150);
    auto mymap_upper_b = map_test.upper_bound(ELEMENTS);
    auto stdmap_lower_b = std_map_test.lower_bound(150);
    auto stdmap_upper_b = std_map_test.upper_bound(ELEMENTS);

    auto mymap_it = mymap_lower_b;
    auto stdmap_it = stdmap_lower_b;
    for ( ; stdmap_it != stdmap_upper_b ; ++stdmap_it, ++mymap_it) {
        CONTAINERS_ASSERT(mymap_it->first == stdmap_it->first);
        CONTAINERS_ASSERT(mymap_it->second == stdmap_it->second);
    }

    mymap_it = mymap_lower_b;
    stdmap_it = stdmap_lower_b;
    for ( ; mymap_it != mymap_upper_b ; ++mymap_it, ++stdmap_it) {
        CONTAINERS_ASSERT(mymap_it->first == stdmap_it->first);
        CONTAINERS_ASSERT(mymap_it->second == stdmap_it->second);
    }

    /* equal_range() test.  */
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto myrange = map_test.equal_range(i);
        index = 0;
        for (auto it = myrange.first ; it != myrange.second ; it++, index++) {
            CONTAINERS_ASSERT(it->first == i);
        }
        CONTAINERS_ASSERT(index == 1);
    }

    auto myrange = map_test.equal_range(-15);
    CONTAINERS_ASSERT(myrange.first == map_test.end());
    CONTAINERS_ASSERT(myrange.second == map_test.end());

    /* count() test.  */
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        CONTAINERS_ASSERT(map_test.count(i) == 1);
    }

    CONTAINERS_ASSERT(map_test.count(-15) == 0);

    /* maps are sorted, this also asserts compatibility with STL iterators.  */
    CONTAINERS_ASSERT(std::is_sorted(map_test.begin(), map_test.end()));
    CONTAINERS_ASSERT(std::is_sorted(map_test.rbegin(), map_test.rend(), ReverseSorted()));
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
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto val = rand();
        if (multimap_test.find(val) == multimap_test.end()) {
            /* Add EXTRA_ELEMENTS times the same value since it allows multiple copies of a key.  */
            for (size_t j = 0 ; j < EXTRA_ELEMENTS ; j++) {
                auto p = multimap_test.insert({val, std::to_string(val)});
                CONTAINERS_ASSERT(p->first == val);
                CONTAINERS_ASSERT(p->second == std::to_string(val));
                sum += val;
                n_elems_test++;
            }
        }
    }
    CONTAINERS_ASSERT(multimap_test.size() == n_elems_test);

    /* iterators test.  */
    test_sum = 0;
    for (auto it = multimap_test.begin() ; it != multimap_test.end() ; it++) {
        test_sum += it->first;
        CONTAINERS_ASSERT(it->second == std::to_string(it->first));
    }
    CONTAINERS_ASSERT(test_sum == sum);

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
    CONTAINERS_ASSERT(multimap_test.empty());
    CONTAINERS_ASSERT(multimap_test.begin() == multimap_test.end());
    CONTAINERS_ASSERT(multimap_test.rbegin() == multimap_test.rend());

    /* emplace() test.  */
    sum = 0;
    n_elems_test = 0;
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto val = rand();
        if (multimap_test.find(val) == multimap_test.end()) {
            /* Add EXTRA_ELEMENTS times the same value since it allows multiple copies of a key.  */
            for (size_t j = 0 ; j < EXTRA_ELEMENTS ; j++) {
                auto p = multimap_test.emplace(std::make_pair(val, std::to_string(val)));
                CONTAINERS_ASSERT(p->first == val);
                CONTAINERS_ASSERT(p->second == std::to_string(val));
                sum += val;
                n_elems_test++;
            }
        }
    }
    CONTAINERS_ASSERT(multimap_test.size() == n_elems_test);

    /* find() test.  */
    test_sum = 0;
    for (auto it = multimap_test.cbegin() ; it != multimap_test.cend() ; it++) {
        auto p = multimap_test.find(it->first);
        CONTAINERS_ASSERT(p->first == it->first);
        CONTAINERS_ASSERT(p->second == std::to_string(it->first));
        test_sum += p->first;
    }
    CONTAINERS_ASSERT(test_sum == sum);

    /* erase() test.  */
    for (auto it = multimap_test.begin() ; it != multimap_test.end() ;) {
        it = multimap_test.erase(it);
    }
    CONTAINERS_ASSERT(multimap_test.empty());
    CONTAINERS_ASSERT(multimap_test.begin() == multimap_test.end());
    CONTAINERS_ASSERT(multimap_test.rbegin() == multimap_test.rend());

    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto val = rand();
        if (multimap_test.find(val) == multimap_test.end()) {
            for (size_t j = 0 ; j < EXTRA_ELEMENTS ; j++) {
                auto p = multimap_test.insert({val, std::to_string(val)});
                CONTAINERS_ASSERT(p->first == val);
                CONTAINERS_ASSERT(p->second == std::to_string(val));
            }
        }
    }

    /* erase(range) test.  */
    multimap_test.erase(multimap_test.begin(), multimap_test.end());
    CONTAINERS_ASSERT(multimap_test.empty());
    CONTAINERS_ASSERT(multimap_test.begin() == multimap_test.end());
    CONTAINERS_ASSERT(multimap_test.rbegin() == multimap_test.rend());

    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        for (size_t j = 0 ; j < EXTRA_ELEMENTS ; j++) {
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
        CONTAINERS_ASSERT(mymultimap_it->first == stdmultimap_it->first);
        CONTAINERS_ASSERT(mymultimap_it->second == stdmultimap_it->second);
    }

    mymultimap_it = mymultimap_lower_b;
    stdmultimap_it = stdmultimap_lower_b;
    for ( ; mymultimap_it != mymultimap_upper_b ; mymultimap_it++, stdmultimap_it++) {
        CONTAINERS_ASSERT(mymultimap_it->first == stdmultimap_it->first);
        CONTAINERS_ASSERT(mymultimap_it->second == stdmultimap_it->second);
    }

    /* equal_range() test.  */
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto myrange = multimap_test.equal_range(i);
        index = 0;
        for (auto it = myrange.first ; it != myrange.second ; it++, index++) {
            CONTAINERS_ASSERT(it->first == i);
            CONTAINERS_ASSERT(it->second == std::to_string(i));
        }
        CONTAINERS_ASSERT(index == EXTRA_ELEMENTS);
    }

    auto myrange = multimap_test.equal_range(-15);
    CONTAINERS_ASSERT(myrange.first == multimap_test.end());
    CONTAINERS_ASSERT(myrange.second == multimap_test.end());

    /* count() test.  */
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        CONTAINERS_ASSERT(multimap_test.count(i) == EXTRA_ELEMENTS);
    }

    CONTAINERS_ASSERT(multimap_test.count(-15) == 0);

    /* multimaps are sorted, this also asserts compatibility with STL iterators.  */
    CONTAINERS_ASSERT(std::is_sorted(multimap_test.begin(), multimap_test.end()));
    CONTAINERS_ASSERT(std::is_sorted(multimap_test.rbegin(), multimap_test.rend(), ReverseSorted()));
}

void run_unordered_set_test() {
    adt::unordered_set<int> uset_test;
    size_t sum, test_sum, n_elems_test, index;

    srand(time(nullptr));

    /* insert(), find() test.  */
    sum = 0;
    n_elems_test = 0;
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto val = rand();
        if (uset_test.find(val) == uset_test.end()) {
            auto p = uset_test.insert(val);
            CONTAINERS_ASSERT(*(p.first) == val);
            CONTAINERS_ASSERT(p.second);
            sum += val;
            n_elems_test++;
        }
    }
    CONTAINERS_ASSERT(uset_test.size() == n_elems_test);

    /* iterators test.  */
    test_sum = 0;
    for (auto it = uset_test.begin() ; it != uset_test.end() ; it++) {
        test_sum += *it;
    }
    CONTAINERS_ASSERT(test_sum == sum);

    /* these should not compile, and they dont.
    for (auto it = uset_test.begin() ; it != uset_test.end() ; it++) {
        *it = 15;
    }
    for (auto it = uset_test.cbegin() ; it != uset_test.cend() ; it++) {
        *it = 15;
    }  */

    /* clear() test.  */
    uset_test.clear();
    CONTAINERS_ASSERT(uset_test.empty());
    CONTAINERS_ASSERT(uset_test.begin() == uset_test.end());

    /* emplace() test.  */
    sum = 0;
    n_elems_test = 0;
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto val = rand();
        if (uset_test.find(val) == uset_test.end()) {
            auto p = uset_test.emplace(val);
            CONTAINERS_ASSERT(*(p.first) == val);
            CONTAINERS_ASSERT(p.second);
            sum += val;
            n_elems_test++;
        }
    }
    CONTAINERS_ASSERT(uset_test.size() == n_elems_test);

    /* find() test.  */
    test_sum = 0;
    for (auto it = uset_test.cbegin() ; it != uset_test.cend() ; it++) {
        auto p = uset_test.find(*it);
        CONTAINERS_ASSERT(*p == *it);
        test_sum += *p;
    }
    CONTAINERS_ASSERT(test_sum == sum);

    /* erase() test.  */
    for (auto it = uset_test.begin() ; it != uset_test.end() ;) {
        it = uset_test.erase(it);
    }
    CONTAINERS_ASSERT(uset_test.empty());
    CONTAINERS_ASSERT(uset_test.begin() == uset_test.end());

    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto val = rand();
        if (uset_test.find(val) == uset_test.end()) {
            auto p = uset_test.insert(val);
            CONTAINERS_ASSERT(*(p.first) == val);
            CONTAINERS_ASSERT(p.second);
        }
    }

    /* erase(range) test.  */
    uset_test.erase(uset_test.begin(), uset_test.end());
    CONTAINERS_ASSERT(uset_test.empty());
    CONTAINERS_ASSERT(uset_test.begin() == uset_test.end());

    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto p = uset_test.insert(i);
        CONTAINERS_ASSERT(*(p.first) == i);
        CONTAINERS_ASSERT(p.second);
    }

    /* equal_range() test.  */
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto myrange = uset_test.equal_range(i);
        index = 0;
        for (auto it = myrange.first ; it != myrange.second ; it++, index++) {
            CONTAINERS_ASSERT(*it == i);
        }
        CONTAINERS_ASSERT(index == 1);
    }

    auto myrange = uset_test.equal_range(-15);
    CONTAINERS_ASSERT(myrange.first == uset_test.end());
    CONTAINERS_ASSERT(myrange.second == uset_test.end());

    /* count() test.  */
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        CONTAINERS_ASSERT(uset_test.count(i) == 1);
    }

    CONTAINERS_ASSERT(uset_test.count(-15) == 0);
}

void run_unordered_multiset_test() {
    adt::unordered_multiset<int> umultiset_test;
    size_t sum, test_sum, n_elems_test, index;

    srand(time(nullptr));

    /* insert(), find() test.  */
    sum = 0;
    n_elems_test = 0;
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto val = rand();
        if (umultiset_test.find(val) == umultiset_test.end()) {
            /* Add EXTRA_ELEMENTS times the same value since it allows multiple copies of a key.  */
            for (size_t j = 0 ; j < EXTRA_ELEMENTS ; j++) {
                auto p = umultiset_test.insert(val);
                CONTAINERS_ASSERT(*p == val);
                sum += val;
                n_elems_test++;
            }
        }
    }
    CONTAINERS_ASSERT(umultiset_test.size() == n_elems_test);

    /* iterators test.  */
    test_sum = 0;
    for (auto it = umultiset_test.begin() ; it != umultiset_test.end() ; it++) {
        test_sum += *it;
    }
    CONTAINERS_ASSERT(test_sum == sum);

    /* these should not compile, and they dont.
    for (auto it = umultiset_test.begin() ; it != umultiset_test.end() ; it++) {
        *it = 15;
    }
    for (auto it = umultiset_test.cbegin() ; it != umultiset_test.cend() ; it++) {
        *it = 15;
    }  */

    /* clear() test.  */
    umultiset_test.clear();
    CONTAINERS_ASSERT(umultiset_test.empty());
    CONTAINERS_ASSERT(umultiset_test.begin() == umultiset_test.end());

    /* emplace() test.  */
    sum = 0;
    n_elems_test = 0;
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto val = rand();
        if (umultiset_test.find(val) == umultiset_test.end()) {
            /* Add EXTRA_ELEMENTS times the same value since it allows multiple copies of a key.  */
            for (size_t j = 0 ; j < EXTRA_ELEMENTS ; j++) {
                auto p = umultiset_test.emplace(val);
                CONTAINERS_ASSERT(*p == val);
                sum += val;
                n_elems_test++;
            }
        }
    }
    CONTAINERS_ASSERT(umultiset_test.size() == n_elems_test);

    /* find() test.  */
    test_sum = 0;
    for (auto it = umultiset_test.cbegin() ; it != umultiset_test.cend() ; it++) {
        auto p = umultiset_test.find(*it);
        CONTAINERS_ASSERT(*p == *it);
        test_sum += *p;
    }
    CONTAINERS_ASSERT(test_sum == sum);

    /* erase() test.  */
    for (auto it = umultiset_test.begin() ; it != umultiset_test.end() ;) {
        it = umultiset_test.erase(it);
    }
    CONTAINERS_ASSERT(umultiset_test.empty());
    CONTAINERS_ASSERT(umultiset_test.begin() == umultiset_test.end());

    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto val = rand();
        if (umultiset_test.find(val) == umultiset_test.end()) {
            auto p = umultiset_test.insert(val);
            CONTAINERS_ASSERT(*p == val);
        }
    }

    /* erase(range) test.  */
    umultiset_test.erase(umultiset_test.begin(), umultiset_test.end());
    CONTAINERS_ASSERT(umultiset_test.empty());
    CONTAINERS_ASSERT(umultiset_test.begin() == umultiset_test.end());

    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        for (size_t j = 0 ; j < EXTRA_ELEMENTS ; j++) {
            auto p = umultiset_test.insert(i);
            CONTAINERS_ASSERT(*p == i);
        }
    }

    /* equal_range() test.  */
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto myrange = umultiset_test.equal_range(i);
        index = 0;
        for (auto it = myrange.first ; it != myrange.second ; it++, index++) {
            CONTAINERS_ASSERT(*it == i);
        }
        CONTAINERS_ASSERT(index == EXTRA_ELEMENTS);
    }

    auto myrange = umultiset_test.equal_range(-15);
    CONTAINERS_ASSERT(myrange.first == umultiset_test.end());
    CONTAINERS_ASSERT(myrange.second == umultiset_test.end());

    /* count() test.  */
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        CONTAINERS_ASSERT(umultiset_test.count(i) == EXTRA_ELEMENTS);
    }

    CONTAINERS_ASSERT(umultiset_test.count(-15) == 0);
}

void run_unordered_map_test() {
    adt::unordered_map<int, std::string> umap_test;
    size_t sum, test_sum, n_elems_test, index;

    srand(time(nullptr));

    /* operator[] test.  */
    umap_test[3] = "kostas";
    umap_test[0] = "lee";
    umap_test[2] = "kostaslee";
    umap_test[1] = "leekostas";
    CONTAINERS_ASSERT(umap_test[3] == "kostas");
    CONTAINERS_ASSERT(umap_test[0] == "lee");
    CONTAINERS_ASSERT(umap_test[2] == "kostaslee");
    CONTAINERS_ASSERT(umap_test[1] == "leekostas");

    umap_test[1] = "changed";
    CONTAINERS_ASSERT(umap_test[1] == "changed");

    umap_test.clear();

    /* insert(), find() test.  */
    sum = 0;
    n_elems_test = 0;
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto val = rand();
        if (umap_test.find(val) == umap_test.end()) {
            auto p = umap_test.insert({val, std::to_string(val)});
            CONTAINERS_ASSERT((p.first)->first == val);
            CONTAINERS_ASSERT((p.first)->second == std::to_string(val));
            CONTAINERS_ASSERT(p.second);
            sum += val;
            n_elems_test++;
        }
    }
    CONTAINERS_ASSERT(umap_test.size() == n_elems_test);

    /* iterators test.  */
    test_sum = 0;
    for (auto it = umap_test.begin() ; it != umap_test.end() ; it++) {
        test_sum += it->first;
    }
    CONTAINERS_ASSERT(test_sum == sum);

    /* these should not compile, and they dont.
    for (auto it = umap_test.begin() ; it != umap_test.end() ; it++) {
        it->first = 15;
    }
    for (auto it = umap_test.cbegin() ; it != umap_test.cend() ; it++) {
        it->first = 150;
        it->second = "random";
    }  */

    /* clear() test.  */
    umap_test.clear();
    CONTAINERS_ASSERT(umap_test.empty());
    CONTAINERS_ASSERT(umap_test.begin() == umap_test.end());

    /* emplace() test.  */
    sum = 0;
    n_elems_test = 0;
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto val = rand();
        if (umap_test.find(val) == umap_test.end()) {
            auto p = umap_test.emplace(std::make_pair(val, std::to_string(val)));
            CONTAINERS_ASSERT((p.first)->first == val);
            CONTAINERS_ASSERT((p.first)->second == std::to_string(val));
            CONTAINERS_ASSERT(p.second);
            sum += val;
            n_elems_test++;
        }
    }
    CONTAINERS_ASSERT(umap_test.size() == n_elems_test);

    /* find() test.  */
    test_sum = 0;
    for (auto it = umap_test.cbegin() ; it != umap_test.cend() ; it++) {
        auto p = umap_test.find(it->first);
        CONTAINERS_ASSERT(p->first == it->first);
        CONTAINERS_ASSERT(p->second == it->second);
        test_sum += p->first;
    }
    CONTAINERS_ASSERT(test_sum == sum);

    /* erase() test.  */
    for (auto it = umap_test.begin() ; it != umap_test.end() ;) {
        it = umap_test.erase(it);
    }
    CONTAINERS_ASSERT(umap_test.empty());
    CONTAINERS_ASSERT(umap_test.begin() == umap_test.end());

    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto val = rand();
        if (umap_test.find(val) == umap_test.end()) {
            auto p = umap_test.insert({val, std::to_string(val)});
            CONTAINERS_ASSERT((p.first)->first == val);
            CONTAINERS_ASSERT((p.first)->second == std::to_string(val));
            CONTAINERS_ASSERT(p.second);
        }
    }

    /* erase(range) test.  */
    umap_test.erase(umap_test.begin(), umap_test.end());
    CONTAINERS_ASSERT(umap_test.empty());
    CONTAINERS_ASSERT(umap_test.begin() == umap_test.end());

    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto val = i;
        if (umap_test.find(val) == umap_test.end()) {
            umap_test.insert({val, std::to_string(val)});
        }
    }

    /* equal_range() test.  */
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto myrange = umap_test.equal_range(i);
        index = 0;
        for (auto it = myrange.first ; it != myrange.second ; it++, index++) {
            CONTAINERS_ASSERT(it->first == i);
        }
        CONTAINERS_ASSERT(index == 1);
    }

    auto myrange = umap_test.equal_range(-15);
    CONTAINERS_ASSERT(myrange.first == umap_test.end());
    CONTAINERS_ASSERT(myrange.second == umap_test.end());

    /* count() test.  */
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        CONTAINERS_ASSERT(umap_test.count(i) == 1);
    }

    CONTAINERS_ASSERT(umap_test.count(-15) == 0);
}

void run_unordered_multimap_test() {
    adt::unordered_multimap<int, std::string> umultimap_test;
    size_t sum, test_sum, n_elems_test, index;

    srand(time(nullptr));

    /* insert(), find() test.  */
    sum = 0;
    n_elems_test = 0;
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto val = rand();
        if (umultimap_test.find(val) == umultimap_test.end()) {
            /* Add EXTRA_ELEMENTS times the same value since it allows multiple copies of a key.  */
            for (size_t j = 0 ; j < EXTRA_ELEMENTS ; j++) {
                auto p = umultimap_test.insert({val, std::to_string(val)});
                CONTAINERS_ASSERT(p->first == val);
                CONTAINERS_ASSERT(p->second == std::to_string(val));
                sum += val;
                n_elems_test++;
            }
        }
    }
    CONTAINERS_ASSERT(umultimap_test.size() == n_elems_test);

    /* iterators test.  */
    test_sum = 0;
    for (auto it = umultimap_test.begin() ; it != umultimap_test.end() ; it++) {
        test_sum += it->first;
        CONTAINERS_ASSERT(it->second == std::to_string(it->first));
    }
    CONTAINERS_ASSERT(test_sum == sum);

    /* these should not compile, and they dont.
    for (auto it = umultimap_test.begin() ; it != umultimap_test.end() ; it++) {
        it->first = 15;
    }
    for (auto it = umultimap_test.cbegin() ; it != umultimap_test.cend() ; it++) {
        it->first = 15;
        it->second = "random";
    }  */

    /* clear() test.  */
    umultimap_test.clear();
    CONTAINERS_ASSERT(umultimap_test.empty());
    CONTAINERS_ASSERT(umultimap_test.begin() == umultimap_test.end());

    /* emplace() test.  */
    sum = 0;
    n_elems_test = 0;
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto val = rand();
        if (umultimap_test.find(val) == umultimap_test.end()) {
            /* Add EXTRA_ELEMENTS times the same value since it allows multiple copies of a key.  */
            for (size_t j = 0 ; j < EXTRA_ELEMENTS ; j++) {
                auto p = umultimap_test.emplace(std::make_pair(val, std::to_string(val)));
                CONTAINERS_ASSERT(p->first == val);
                CONTAINERS_ASSERT(p->second == std::to_string(val));
                sum += val;
                n_elems_test++;
            }
        }
    }
    CONTAINERS_ASSERT(umultimap_test.size() == n_elems_test);

    /* find() test.  */
    test_sum = 0;
    for (auto it = umultimap_test.cbegin() ; it != umultimap_test.cend() ; it++) {
        auto p = umultimap_test.find(it->first);
        CONTAINERS_ASSERT(p->first == it->first);
        CONTAINERS_ASSERT(p->second == std::to_string(it->first));
        test_sum += p->first;
    }
    CONTAINERS_ASSERT(test_sum == sum);

    /* erase() test.  */
    for (auto it = umultimap_test.begin() ; it != umultimap_test.end() ;) {
        it = umultimap_test.erase(it);
    }
    CONTAINERS_ASSERT(umultimap_test.empty());
    CONTAINERS_ASSERT(umultimap_test.begin() == umultimap_test.end());

    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto val = rand();
        if (umultimap_test.find(val) == umultimap_test.end()) {
            for (size_t j = 0 ; j < EXTRA_ELEMENTS ; j++) {
                auto p = umultimap_test.insert({val, std::to_string(val)});
                CONTAINERS_ASSERT(p->first == val);
                CONTAINERS_ASSERT(p->second == std::to_string(val));
            }
        }
    }

    /* erase(range) test.  */
    umultimap_test.erase(umultimap_test.begin(), umultimap_test.end());
    CONTAINERS_ASSERT(umultimap_test.empty());
    CONTAINERS_ASSERT(umultimap_test.begin() == umultimap_test.end());

    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        for (size_t j = 0 ; j < EXTRA_ELEMENTS ; j++) {
            umultimap_test.insert({i, std::to_string(i)});
        }
    }

    /* equal_range() test.  */
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        auto myrange = umultimap_test.equal_range(i);
        index = 0;
        for (auto it = myrange.first ; it != myrange.second ; it++, index++) {
            CONTAINERS_ASSERT(it->first == i);
            CONTAINERS_ASSERT(it->second == std::to_string(i));
        }
        CONTAINERS_ASSERT(index == EXTRA_ELEMENTS);
    }

    auto myrange = umultimap_test.equal_range(-15);
    CONTAINERS_ASSERT(myrange.first == umultimap_test.end());
    CONTAINERS_ASSERT(myrange.second == umultimap_test.end());

    /* count() test.  */
    for (size_t i = 0 ; i < ELEMENTS ; i++) {
        CONTAINERS_ASSERT(umultimap_test.count(i) == EXTRA_ELEMENTS);
    }

    CONTAINERS_ASSERT(umultimap_test.count(-15) == 0);
}

void run_pqueue_test() {
    adt::pqueue<int> max_heap;
    adt::pqueue<int, std::greater<int>> min_heap;

    max_heap.push(15);
    max_heap.push(-15);
    max_heap.push(25);
    max_heap.push(105);
    max_heap.push(250);
    max_heap.push(100);

    /* assert the max value is on top since its a max_heap.  */
    CONTAINERS_ASSERT(max_heap.top() == std::max({15, -15, 25, 105, 250, 100}));

    min_heap.push(15);
    min_heap.push(-15);
    min_heap.push(25);
    min_heap.push(105);
    min_heap.push(250);
    min_heap.push(100);

    /* assert the min value is on top since its a min_heap.  */
    CONTAINERS_ASSERT(min_heap.top() == std::min({15, -15, 25, 105, 250, 100}));
}

int main() {
//    run_list_test();
//    run_vector_test();
//    run_set_test();
//    run_multiset_test();
//    run_map_test();
//    run_multimap_test();
//    run_unordered_set_test();
//    run_unordered_multiset_test();
//    run_unordered_map_test();
//    run_unordered_multimap_test();
    run_pqueue_test();

    return 0;    
}