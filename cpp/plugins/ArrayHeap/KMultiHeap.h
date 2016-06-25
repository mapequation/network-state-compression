#ifndef __KMutliHeap_H_
#define __KMutliHeap_H_

#include <iostream>
#include <vector>
#include "KHeap.h"

class KMultiHeap {
    typedef KHeap<double> Heap;
    KHeap<Heap> heap;
public:

    struct MultiHeapElement {
        MultiHeapElement(KHeap<Heap> &heap) {
            key1 = heap.top();
            key2 = heap.get().top();
            val = heap.get().get();
        }

        int key1, key2;
        double val;

        friend std::ostream &operator<<(std::ostream &os, const MultiHeapElement &obj) {
            double val = obj.val;
            return os << obj.key1 << " " << obj.key2 << " " << val;
        }
    };

    MultiHeapElement top() {
        return MultiHeapElement(heap);
    }

    int size() {
        return heap.size();
    }

    bool empty(){
        return heap.empty();
    }


    void remove(int key) {
//        std::cout << "Remove " << key << "\n";
        heap.erase(key);
        std::vector<int> keys;
        heap.getKeys(keys);
        for (auto k : keys) {
            remove(key, k);
        }
    }

    inline void remove(int key1, int key2) {
//        std::cout << "remove " << key1 << " " << key2 << "\n";
        sort(key1, key2);
        if (heap.exists(key1)) {
            auto &h = heap.get(key1);
            h.erase(key2);
            if (h.empty()) {
                heap.erase(key1);
            } else {
                heap.update(key1);
            }
        }
    }

    void set(int key1, int key2, double value) {
        sort(key1, key2);
        if (heap.exists(key1)) {
            heap.get(key1).set(key2, value);
            heap.update(key1);
        } else {
            Heap temp;
            temp.set(key2, value);
            heap.set(key1, temp);
        }
    }

    void print() {
        std::cout << " <<<< " << top() << " >>>\n";
        for (auto &h : heap) {
            std::cout << h.key << " " << h.value << std::endl;
        }
    }

private:
    static inline void sort(int &key1, int &key2) {
        if (key1 < key2) {
            std::swap(key1, key2);
        }
    }
};


#endif //__MutliHeap_H_
