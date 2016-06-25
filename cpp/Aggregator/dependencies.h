#ifndef AGG_DEPENDENCIES
#define AGG_DEPENDENCIES

#include "../plugins/ArrayHeap/KMultiHeap.h"
#include "../plugins/ArrayHeap/lib.h"
#include "../plugins/Tree/Tree.h"
// TODO: smarter merge algorithm is possible..
template<typename Key, typename Value>
static void merge(const std::map <Key, Value> &s1, const std::map <Key, Value> &s2, std::map <Key, Value> &t) {
    for (auto &it : s1) {
        t[it.first] += it.second;
    }
    for (auto &it : s2) {
        t[it.first] += it.second;
    }
}

template <typename Key>
double static inline sum(std::map<Key, double> &w) {
    double sum = 0;
    for (auto &d : w) {
        sum += d.second;
    }
    return sum;
}
#endif


