#include <algorithm>
#include <iostream>
#include <cassert>
#include <vector>
#include <set>
#include <string>
#include <unordered_set>

#include "list.h"
#include "vector.h"
#include "map.h"
#include "set.h"
#include "unordered_set.h"
#include "unordered_map.h"

struct emplace_test_t {
    int x;
    int y;
    double z;
    std::string str;

    emplace_test_t(int _x = 0, int _y = 1, double _z = 0.0, std::string _str = "lee")
                  : x(_x), y(_y), z(_z), str(std::move(_str)) {}
};

struct hash_functor {
    size_t operator()(const emplace_test_t &key) const noexcept {
        return std::hash<int>{}(key.x);
    }
};

struct equal_functor {
    bool operator()(const emplace_test_t &lhs, const emplace_test_t &rhs) {
        return lhs.x == rhs.x;
    }
};

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
    assert(myints.empty());
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
    assert(myints.empty());
    assert(myints.begin() == myints.end());

    for (int i = 0 ; i < 1500 ; i++) {
        myints.push_back(i);
    }

    /* remove() test. */
    val = 1499;
    for (size_t i = 0 ; i < loops ; i++, val--) {
        myints.remove(val);
    }
    assert(myints.empty());
    assert(myints.begin() == myints.end());

    /* remove() test. */
    for (auto it = myints.begin() ; it != myints.end() ; ) {
        it = myints.remove(it);
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

    myints.clear();

    for (size_t i = 0 ; i < 100 ; i++) {
        myints.push_back(150);
    }

    /* remove() test. */
    myints.remove(150);

    assert(myints.empty());
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
    assert (std::is_sorted (sorted_test.begin(), sorted_test.end(), reverse_order));
}
  
void run_multimap_test() {

}

void run_unordered_set_test() {
    adt::unordered_set<int> test_set;
    adt::unordered_set<emplace_test_t, hash_functor, equal_functor> test_set_1;
    size_t sum = 0;

    srand(time(NULL));

    /* Insert test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        test_set.insert(i);
    }

    /* Iterators test.  */
    for (auto it = test_set.begin() ; it != test_set.end() ; it++) {
        sum += *it;
    }
    assert(sum == 1124250);

    /* Find test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        assert(*test_set.find(i) == i);
    }

    /* Erase key test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        assert(test_set.erase(i) == 1);
    }
    assert(test_set.empty());
    for (auto it = test_set.begin() ; it != test_set.end() ; ) {
        assert(0);
    }

    for (size_t i = 0 ; i < 1500 ; i++) {
        test_set.insert(i);
    }

    /* Clear test.  */
    test_set.clear();
    assert(test_set.empty());
    for (auto it = test_set.begin() ; it != test_set.end() ; ) {
        assert(0);
    }

    /* Emplace test.  */
    test_set_1.emplace(1, 2, 0.5, "Kostas");
    assert(test_set_1.begin()->x == 1);
    assert(test_set_1.begin()->y == 2);
    assert(test_set_1.begin()->z == 0.5);
    assert(test_set_1.begin()->str == "Kostas");
}

void run_unordered_multiset_test() {
}

void run_unordered_map_test() {
    adt::unordered_map<int, std::string> map_test;
    size_t sum, test_sum, index;

    /* insert() test.  */
    sum = 0;
    for (size_t i = 0 ; i < 1500 ; i++) {
        auto p = map_test.insert({i, "kostas" + std::to_string(i)});
        assert(p.second);
        assert(p.first->first == i);
        assert(p.first->second == "kostas" + std::to_string(i));
        sum += i;
    }

    /* iterators test.  */
    test_sum = 0;
    for (auto it = map_test.cbegin() ; it != map_test.cend() ; it++) {
        test_sum += it->first;
    }
    assert(sum == test_sum);

    /* operator[] test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        assert(map_test[i] == "kostas" + std::to_string(i));
    }

    /* at() test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        assert(map_test.at(i) == "kostas" + std::to_string(i));
    }

    try {
        map_test.at(1232132);
        assert(0);
    } catch (const std::out_of_range& out_of_range) {}

    /* find() test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        assert(map_test.find(i)->second == "kostas" + std::to_string(i));
    }

    /* this should not compile, keys are read-only.
    for (size_t i = 0 ; i < 1500 ; i++) {
        assert(map_test.find(i)->first = i + 1);
    } */

    /* this should compile fine.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        map_test.find(i)->second = "kostas" + std::to_string(i);
    }

    /* clear() test.  */
    map_test.clear();
    assert(map_test.begin() == map_test.end());
    assert(map_test.cbegin() == map_test.cend());
    assert(map_test.empty());

    /* emplace() test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        std::string str("kostas" + std::to_string(i));
        auto p = map_test.emplace(i, str);
        assert(p.second);
        assert(p.first->first == i);
        assert(p.first->second == "kostas" + std::to_string(i));
    }

    /* erase() test.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        assert(map_test.erase(i) == 1);
    }

    for (size_t i = 0 ; i < 1500 ; i++) {
        auto p = map_test.insert(std::make_pair<int, std::string>(i, "kostas" + std::to_string(i)));
        assert(p.second);
        assert(p.first->first == i);
        assert(p.first->second == "kostas" + std::to_string(i));
    }

    index = 0;
    for (auto it = map_test.begin() ; it != map_test.end() ; index++) {
        /* this also asserts that we can convert iterator to const_iterator.  */
        it = map_test.erase(it);
    }

    /* This should not compile, and it doesnt.
    for (auto it = map_test.cbegin() ; it != map_test.cend() ; it++) {
        it->first = 15;
        it->second = "asd";
    } */

    /* check if we are able to change the mapped values.  */
    for (size_t i = 0 ; i < 1500 ; i++) {
        map_test[i] = "0";
    }

    for (auto it = map_test.begin() ; it != map_test.end() ; it++, index++) {
        assert(it->second == "0");
    }
}

void run_unordered_multimap_test() {

}

int main() {
//    run_list_test();
//    run_vector_test();
//    run_set_test();
//    run_multiset_test();
//    run_map_test();
//    run_multimap_test();
    run_unordered_set_test();
    run_unordered_map_test();
//    run_unordered_multimap_test();
//    run_unordered_multiset_test();

    return 0;    
}