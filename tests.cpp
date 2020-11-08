#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>
#include <vector>
#include <string>

#include "list.h"
#include "vector.h"
#include "map.h"
#include "set.h"
#include "unordered_set.h"
#include "unordered_multiset.h"
#include "unordered_map.h"
#include "unordered_multimap.h"


void run_list_test() {
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
    assert(myints.empty());
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
    assert(myints.empty());
    assert(myints.begin() == myints.end());

    for (int i = 0 ; i < 1500 ; i++) {
        myints.push_back(i);
    }

    /* remove() test. */
    for (auto it = myints.begin() ; it != myints.end() ; ) {
        it = myints.erase(it);
    }
    assert(myints.empty());
    assert(myints.begin() == myints.end());

    for (int i = 0 ; i < 1500 ; i++) {
        myints.push_back(i);
    }

    /* clear() test. */
    myints.clear();
    assert(myints.empty());
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
    std::vector<int> numbers_added;
    std::vector<int> sorted_test;
    int num;
    size_t index;
    size_t ceil = 10000;

    srand(time(NULL));

    /* insert() test.  */
    for (size_t i = 0 ; i < ceil ; i++) {
        num = rand() % ceil;
        myset.insert(num);
        mystdset.insert(num);
    }

    for (auto it = myset.begin() ; it != myset.end() ; it++) {
        sorted_test.push_back(*it);
    }
    assert(std::is_sorted(sorted_test.begin(), sorted_test.end()));

    /* erase() test.  */
    for (auto it = myset.begin() ; it != myset.end() ; ) {
        it = myset.erase(it);
    }
    assert(myset.empty());
    assert(myset.begin() == myset.end());

    for (size_t i = 0 ; i < ceil ; i++) {
        num = rand() % ceil;
        myset.insert(num);
        numbers_added.push_back(num);
    }

    /* find() test.  */
    for (auto it = numbers_added.begin() ; it != numbers_added.end() ; it++) {
        assert(*(myset.find(*it)) == *it);
    }

    /* clear() test.  */
    myset.clear();
    numbers_added.clear();
    assert(myset.empty());
    assert(myset.begin() == myset.end());

    for (size_t i = 0 ; i < ceil ; i++) {
        myset.insert(i);
        mystdset.insert(i);
    }

    /* lower/upper bound test.  */
    auto l_bound = myset.lower_bound(10);
    auto u_bound = myset.upper_bound(150);
    auto l_std_bound = mystdset.lower_bound(10);
    auto u_std_bound = mystdset.upper_bound(150);

    for (auto it = l_bound ; it != u_bound ; it++) {
        numbers_added.push_back(*it);
    }
    index = 0;
    for (auto it = l_std_bound ; it != u_std_bound ; it++) {
        assert(*it == numbers_added[index]);
        index++;
    }

    /* reverse iterators test. */
    num = ceil - 1;
    for (auto it = myset.rbegin() ; it != myset.rend() ; it++, num--) {
        assert(*it == num);
    }
}

void run_multiset_test() {

}

static void gen_random (std::string& str, const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        str.push_back (alphanum[rand() % (sizeof(alphanum) - 1)]);
    }
}

bool reverse_order (int x, int y) {
    return x >= y;
}

void run_map_test() {
    adt::map<int, std::string> mymap;
    std::vector<int> sorted_test;
    std::string str;

    srand(time(NULL));

    /* operator[] test.  */
    for (size_t i = 0 ; i < 15000 ; i++) {
        gen_random (str, 15);
        mymap[rand() + 2] = str;
    }

    /* sort test.  */
    for (auto it = mymap.begin() ; it != mymap.end() ; it++) {
        sorted_test.push_back(it->first);
    }
    assert (std::is_sorted(sorted_test.begin(), sorted_test.end()));

    /* remove () test.  */
    for (auto it = mymap.begin() ; it != mymap.end() ; ) {
        it = mymap.remove(it);
    }
    assert (mymap.empty());
    assert (mymap.begin() == mymap.end());

    /* add () test.  */
    for (size_t i = 0 ; i < 15000 ; i++) {
        gen_random(str, 10);
        mymap.add(rand() + 2, str);
    }

    /* at () should throw exception.  */
    try {
        mymap.at(1);
        assert (0);
    } catch (const std::out_of_range &oor) {}

    /* clear () test.  */
    mymap.clear();
    assert (mymap.empty());
    assert (mymap.begin() == mymap.end());

    /* reverse iterators test.  */
    sorted_test.clear();
    for (auto it = mymap.rbegin() ; it != mymap.rend() ; it++) {
        sorted_test.push_back (it->first);
    }
    assert (std::is_sorted(sorted_test.begin(), sorted_test.end(), reverse_order));
}
  
void run_multimap_test() {

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

void run_unordered_multiset_test() {
    adt::unordered_multiset<int> multiset_test;
    size_t sum, test_sum, index;

    srand(time(NULL));

    /* insert() test.  */
    sum = 0;
    for (size_t i = 0 ; i < 1500 ; i++) {
        /* Insert same key 3 times.  */
        auto p = multiset_test.insert(i);
        assert(*p == i);
        p = multiset_test.insert(i);
        assert(*p == i);
        p = multiset_test.insert(i);
        assert(*p == i);

        sum += i;
    }
    assert(multiset_test.size() == 1500 * 3);

    /* iterators test.  */
    test_sum = 0;
    for (auto it = multiset_test.begin() ; it != multiset_test.end() ; it++) {
        test_sum += *it;
    }
    assert(sum * 3 == test_sum);

    /* find() test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        assert(*multiset_test.find(i) == i);
    }

    /* this should not compile, keys are read-only.
    for (size_t i = 0 ; i < 1500 ; i++) {
        assert(*multiset_test.find(i) = i + 1);
    }  */

    /* clear() test.  */
    multiset_test.clear();
    assert(multiset_test.begin() == multiset_test.end());
    assert(multiset_test.empty());

    /* emplace() test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        auto p = multiset_test.emplace(i);
        assert(*p == i);
        p = multiset_test.emplace(i);
        assert(*p == i);
        p = multiset_test.emplace(i);
        assert(*p == i);
    }
    assert(multiset_test.size() == 1500 * 3);

    /* erase() test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        assert(multiset_test.erase(i) == 3);
    }
    assert(multiset_test.begin() == multiset_test.end());
    assert(multiset_test.empty());

    for (size_t i = 0 ; i < 1500 ; i++) {
        int trigger_const_ref = i;
        auto p = multiset_test.insert(trigger_const_ref);
        assert(*p == i);
        p = multiset_test.insert(trigger_const_ref);
        assert(*p == i);
        p = multiset_test.insert(trigger_const_ref);
        assert(*p == i);
    }
    assert(multiset_test.size() == 1500 * 3);

    /* equal_range() test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        auto myrange = multiset_test.equal_range(i);
        index = 0;
        for (auto it = myrange.first ; it != myrange.second ; it++, index++) {
            assert(*it == i);
        }
        assert(index == 3);
    }

    auto myrange = multiset_test.equal_range(-15);
    assert(myrange.first == multiset_test.end());
    assert(myrange.second == multiset_test.end());

    /* count() test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        assert(multiset_test.count(i) == 3);
    }

    assert(multiset_test.count(-15) == 0);

    for (auto it = multiset_test.begin() ; it != multiset_test.end() ; ) {
        /* this also asserts that we can convert iterator to const_iterator.  */
        it = multiset_test.erase(it);
    }
    assert(multiset_test.begin() == multiset_test.end());
    assert(multiset_test.empty());

    for (size_t i = 0 ; i < 1500 ; i++) {
        auto p = multiset_test.insert(i);
        assert(*p == i);
    }
    assert(multiset_test.size() == 1500);

    /* This should not compile, and it doesnt.
    for (auto it = multiset_test.begin() ; it != multiset_test.end() ; it++) {
        *it = 15;
    }  */
}

void run_unordered_map_test() {
    adt::unordered_map<int, std::string> unordered_map_test;
    size_t sum, test_sum, index;

    /* insert() test.  */
    sum = 0;
    for (size_t i = 0 ; i < 1500 ; i++) {
        auto p = unordered_map_test.insert({i, "kostas" + std::to_string(i)});
        assert(p.second);
        assert(p.first->first == i);
        assert(p.first->second == "kostas" + std::to_string(i));
        sum += i;
    }

    /* iterators test.  */
    test_sum = 0;
    for (auto it = unordered_map_test.cbegin() ; it != unordered_map_test.cend() ; it++) {
        test_sum += it->first;
        assert(it->second == "kostas" + std::to_string(it->first));
    }
    assert(sum == test_sum);

    /* operator[] test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        assert(unordered_map_test[i] == "kostas" + std::to_string(i));
    }

    /* at() test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        assert(unordered_map_test.at(i) == "kostas" + std::to_string(i));
    }

    try {
        unordered_map_test.at(1232132);
        assert(0);
    } catch (const std::out_of_range& out_of_range) {}

    /* find() test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        auto p = unordered_map_test.find(i);
        assert(p->first == i);
        assert(p->second == "kostas" + std::to_string(i));
    }

    /* this should not compile, keys are read-only.
    for (size_t i = 0 ; i < 1500 ; i++) {
        assert(unordered_map_test.find(i)->first = i + 1);
    }  */

    /* this should compile fine.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        unordered_map_test.find(i)->second = "kostas" + std::to_string(i);
    }

    /* clear() test.  */
    unordered_map_test.clear();
    assert(unordered_map_test.begin() == unordered_map_test.end());
    assert(unordered_map_test.cbegin() == unordered_map_test.cend());
    assert(unordered_map_test.empty());

    /* emplace() test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        std::string str("kostas" + std::to_string(i));
        auto p = unordered_map_test.emplace(i, str);
        assert(p.second);
        assert(p.first->first == i);
        assert(p.first->second == "kostas" + std::to_string(i));

        /* Try to insert value with same key.  */
        p = unordered_map_test.emplace(i, str + "randomstr");
        assert(!p.second);
        assert(p.first->first == i);
        assert(p.first->second == "kostas" + std::to_string(i));
    }
    assert(unordered_map_test.size() == 1500);

    /* erase() test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        assert(unordered_map_test.erase(i) == 1);
    }
    assert(unordered_map_test.begin() == unordered_map_test.end());
    assert(unordered_map_test.cbegin() == unordered_map_test.cend());
    assert(unordered_map_test.empty());

    for (size_t i = 0 ; i < 1500 ; i++) {
        auto p = unordered_map_test.insert(std::make_pair<int, std::string>(i, "kostas" + std::to_string(i)));
        assert(p.second);
        assert(p.first->first == i);
        assert(p.first->second == "kostas" + std::to_string(i));
    }
    assert(unordered_map_test.size() == 1500);

    /* equal_range() test.  */
    for (size_t i = 0; i < 1500; i++) {
        auto myrange = unordered_map_test.equal_range(i);
        index = 0;
        for (auto it = myrange.first; it != myrange.second; it++, index++) {
            assert(it->first == i);
            assert(it->second == "kostas" + std::to_string(i));
        }
        assert(index == 1);
    }

    auto myrange = unordered_map_test.equal_range(-15);
    assert(myrange.first == unordered_map_test.end());
    assert(myrange.second == unordered_map_test.end());

    /* count() test.  */
    for (size_t i = 0; i < 1500; i++) {
        assert(unordered_map_test.count(i) == 1);
    }
    assert(unordered_map_test.count(-15) == 0);

    /* erase() test.  */
    for (auto it = unordered_map_test.begin() ; it != unordered_map_test.end() ; ) {
        /* this also asserts that we can convert iterator to const_iterator.  */
        it = unordered_map_test.erase(it);
    }
    assert(unordered_map_test.begin() == unordered_map_test.end());
    assert(unordered_map_test.cbegin() == unordered_map_test.cend());
    assert(unordered_map_test.empty());

    /* check if we are able to change the mapped values.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        unordered_map_test[i] = "0";
    }
    assert(unordered_map_test.size() == 1500);

    for (auto it = unordered_map_test.begin() ; it != unordered_map_test.end() ; it++, index++) {
        assert(it->second == "0");
    }

    /* This should not compile, and it doesnt.
    for (auto it = unordered_map_test.cbegin() ; it != unordered_map_test.cend() ; it++) {
        it->first = 15;
        it->second = "asd";
    }  */
}

void run_unordered_multimap_test() {
    adt::unordered_multimap<int, std::string> unordered_multimap_test;
    size_t sum, test_sum, index;

    srand(time(NULL));

    /* insert() test.  */
    sum = 0;
    for (size_t i = 0; i < 1500; i++) {
        /* Insert same key 3 times.  */
        auto p = unordered_multimap_test.insert({i, "kostas" + std::to_string(i)});
        assert(p->first == i);
        assert(p->second == "kostas" + std::to_string(i));
        p = unordered_multimap_test.insert({i, "kostas" + std::to_string(i)});
        assert(p->first == i);
        assert(p->second == "kostas" + std::to_string(i));
        p = unordered_multimap_test.insert({i, "kostas" + std::to_string(i)});
        assert(p->first == i);
        assert(p->second == "kostas" + std::to_string(i));

        sum += i;
    }
    assert(unordered_multimap_test.size() == 1500 * 3);

    /* iterators test.  */
    test_sum = 0;
    for (auto it = unordered_multimap_test.begin(); it != unordered_multimap_test.end(); it++) {
        test_sum += it->first;
    }
    assert(sum * 3 == test_sum);

    /* find() test.  */
    for (size_t i = 0; i < 1500; i++) {
        assert(unordered_multimap_test.find(i)->first == i);
        assert(unordered_multimap_test.find(i)->second == "kostas" + std::to_string(i));
    }

    /* this should not compile, keys are read-only.
    for (size_t i = 0 ; i < 1500 ; i++) {
        assert(unordered_multimap_test.find(i)->first = i + 1);
    }  */

    /* clear() test.  */
    unordered_multimap_test.clear();
    assert(unordered_multimap_test.begin() == unordered_multimap_test.end());
    assert(unordered_multimap_test.empty());

    /* emplace() test.  */
    for (size_t i = 0; i < 1500; i++) {
        auto p = unordered_multimap_test.emplace(i, "kostas" + std::to_string(i));
        assert(p->first == i);
        assert(p->second == "kostas" + std::to_string(i));
        p = unordered_multimap_test.emplace(i, "kostas" + std::to_string(i));
        assert(p->first == i);
        assert(p->second == "kostas" + std::to_string(i));
        p = unordered_multimap_test.emplace(i, "kostas" + std::to_string(i));
        assert(p->first == i);
        assert(p->second == "kostas" + std::to_string(i));
    }
    assert(unordered_multimap_test.size() == 1500 * 3);

    /* erase() test.  */
    for (size_t i = 0; i < 1500; i++) {
        assert(unordered_multimap_test.erase(i) == 3);
    }
    assert(unordered_multimap_test.begin() == unordered_multimap_test.end());
    assert(unordered_multimap_test.cbegin() == unordered_multimap_test.cend());
    assert(unordered_multimap_test.empty());

    for (size_t i = 0; i < 1500; i++) {
        std::pair<int, std::string> trigger_const_ref = {i, "kostas" + std::to_string(i)};
        auto p = unordered_multimap_test.insert(trigger_const_ref);
        assert(p->first == i);
        assert(p->second == "kostas" + std::to_string(i));
        p = unordered_multimap_test.insert(trigger_const_ref);
        assert(p->first == i);
        assert(p->second == "kostas" + std::to_string(i));
        p = unordered_multimap_test.insert(trigger_const_ref);
        assert(p->first == i);
        assert(p->second == "kostas" + std::to_string(i));
    }
    assert(unordered_multimap_test.size() == 1500 * 3);

    /* equal_range() test.  */
    for (size_t i = 0; i < 1500; i++) {
        auto myrange = unordered_multimap_test.equal_range(i);
        index = 0;
        for (auto it = myrange.first; it != myrange.second; it++, index++) {
            assert(it->first == i);
            assert(it->second == "kostas" + std::to_string(i));
        }
        assert(index == 3);
    }

    auto myrange = unordered_multimap_test.equal_range(-15);
    assert(myrange.first == unordered_multimap_test.end());
    assert(myrange.second == unordered_multimap_test.end());

    /* count() test.  */
    for (size_t i = 0; i < 1500; i++) {
        assert(unordered_multimap_test.count(i) == 3);
    }
    assert(unordered_multimap_test.count(-15) == 0);

    for (auto it = unordered_multimap_test.begin(); it != unordered_multimap_test.end() ;) {
        /* this also asserts that we can convert iterator to const_iterator.  */
        it = unordered_multimap_test.erase(it);
    }
    assert(unordered_multimap_test.begin() == unordered_multimap_test.end());
    assert(unordered_multimap_test.cbegin() == unordered_multimap_test.cend());
    assert(unordered_multimap_test.empty());

    for (size_t i = 0; i < 1500; i++) {
        auto p = unordered_multimap_test.emplace(i, "whatever");
        assert(p->first == i);
        assert(p->second == "whatever");
    }
    assert(unordered_multimap_test.size() == 1500);

    /* This should compile fine, we can change the mapped value.  */
    for (auto it = unordered_multimap_test.begin(); it != unordered_multimap_test.end(); it++) {
        it->second = "lee";
    }
    assert(unordered_multimap_test.find(rand() % 1500)->second == "lee");

    /* This should not compile, its const iterator.
    for (auto it = unordered_multimap_test.cbegin() ; it != unordered_multimap_test.cend() ; it++) {
        it->second = "lee";
    }  */
}


int main() {
//    run_list_test();
//    run_vector_test();
//    run_set_test();
//    run_multiset_test();
//    run_map_test();
//    run_multimap_test();
    run_unordered_set_test();
    run_unordered_multiset_test();
    run_unordered_map_test();
    run_unordered_multimap_test();

    return 0;    
}