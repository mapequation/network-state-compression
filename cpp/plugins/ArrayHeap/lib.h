//
// Created by Christian Persson on 21/03/15.
//

#include <map>
#include <numeric>      // std::accumulate
#include <algorithm>

#ifndef _ARRAYHEAP_LIB_H_
#define _ARRAYHEAP_LIB_H_


template <typename Key, typename Value, typename F>
inline double sum(const std::map<Key, Value> &map, F f){
    double _ = 0;
    for(auto &elem : map){
        _ += f(elem.second);
    }
    return _;
}

template <typename Key, typename Value>
inline Value sum(const std::map<Key, Value> &map){
    Value _ = 0;
    for(auto &elem : map){
        _ += elem.second;
    }
    return _;
}

template<typename _Container, typename _Func>
inline double sum(const _Container &_c, _Func _f) {
    double _ = 0;
    for(auto &elem : _c){
        _ += _f(elem);
    }
    return _;
}




template<typename _Func>
inline void range(int stop, _Func f) {
    for (int i = 0; i < stop; i++) {
        f(i);
    }
}

template<typename _Func>
inline void bench_range(int stop, _Func f) {
    auto t = clock();
    for (int i = 0; i < stop; i++) {
        f(i);
    }
    std::cout << stop << " iterations in " << (clock() - t) / 1000 << " ms\n";
}

template<typename _Container, typename _Func>
void for_each(_Container &_c, _Func f){
    std::for_each(_c.begin(), _c.end(), f);
}


#endif //_ARRAYHEAP_LIB_H_
