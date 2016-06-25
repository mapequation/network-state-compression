#ifndef __KHeap_H_
#define __KHeap_H_

#include <deque>
#include "ArrayHeap.h"

template<typename Value>
struct KHeapElem {
    KHeapElem(int key) : key(key) {
    }


    int bucket = -1;
    int index;
    Value value;
    int key;

    bool operator<(KHeapElem const &b) {
        return value < b.value;
    }

    friend std::ostream &operator<<(std::ostream &os, const KHeapElem &obj) {
        return os << obj.key << " " << obj.value;
    }
};


const int capacity = 4;

template<typename Value = double>
class KHeap {

    typedef int Key;
    typedef std::vector<KHeapElem<Value>> Container;
    typedef std::vector<Key> KeyContainer;

    struct KHeapBucket {
        KeyContainer keys;
        double maxVal = 0;
        double minVal = 0;

        void add(KHeapElem<Value> &e) {
            if (maxVal < e.value)
                maxVal = e.value;
            keys.push_back(e.key);
            e.index = keys.size() - 1;
        }
    };

    double maxVal = 0;

    ArrayHeap<double> heap;
    Container elements;
    std::vector<KHeapBucket> buckets;
    KeyContainer keys;

public:

    inline operator double() {
        return heap.top().val;
    }

    friend std::ostream &operator<<(std::ostream &os, const KHeap &obj) {
        return os << obj.heap;
    }

    typename Container::iterator begin() {
        return elements.begin();
    }

    typename Container::iterator end() {
        return elements.end();
    }

    inline int size() {
        return elements.size();
    }

    bool empty(){
        return elements.empty();
    }

    inline void getKeys(std::vector<int> &_) {
        for (int i = 0; i < keys.size(); i++) {
            if (keys[i] != -1) {
                _.push_back(i);
            }
        }
    };

    inline void update(Key key) {
        int index = _key2Index(key);
        if (index != -1) {
            KHeapElem<Value> &e = elements[index];
            update(e);
        }
    }

    inline void update(KHeapElem<Value> &e) {
        removeReferences(e);
        _insert(e);
    }

    void set(Key key, Value val) {
        int index = _key2Index(key);
        if (index != -1) {
            KHeapElem<Value> &e = elements[index];
            e.value = val;
            update(e);
        }
        else {
            if (key >= keys.size()) {
                keys.resize(key + 1, -1);
            }
            elements.push_back(KHeapElem<Value>(key));
            KHeapElem<Value> &e = elements.back();
            keys[key] = elements.size() - 1;
            e.value = val;
            _insert(e);
        }
    }

    void erase(Key key) {
        int index = _key2Index(key);

        if (index != -1) {
            KHeapElem<Value> &e = elements[index];
            removeReferences(e);

            // remove object
            if (index != elements.size() - 1) {
                elements[index] = std::move(elements.back());
            }
            elements.pop_back();

            // update key for swapped element
            keys[elements[index].key] = index;
            keys[key] = -1;
        }
    }

    inline Value &get() {
        return get(top());
    }

    bool exists(Key key) {
        return _key2Index(key) != -1;
    }

    Value &get(Key key) {
        int index = _key2Index(key);
        if (index == -1) {
            throw -1;
        }
        return elements[_key2Index(key)].value;
    }

    void pop() {
        int key = top();
        heap.pop();
        if (heap.empty()) {
            _handleEmptyHeap();
        }
        erase(key);
    }

    inline Key top() {
        return heap.top().key;
    }

    void print(bool extended = false) {
        int sum = 0;
        for (auto &bucket : buckets) {
            sum += bucket.keys.size();
        }
        int i = 0;
        for (auto &bucket : buckets) {
            int size = bucket.keys.size();
            std::cout << i++ << ": " << size << " (" << (double) size / sum << ") \n";

            if (extended) {
                std::cout << "[ ";
                auto bucketKeys = bucket.keys;
                std::deque<KHeapElem<Value>> values;
                for (auto key : bucketKeys) {
                    values.push_back(elements[_key2Index(key)]);
                }
                for (auto &hvalue : values) {
                    std::cout << "(" << hvalue.key << "," << hvalue.value << ") ";
                }
                std::cout << "]\n";
            }
        }
        std::cout << heap.size() << " elements in heap\n";
        std::cout << sum << " elements in buckets\n";
    }


protected:

    inline void removeFromBucket(KHeapElem<Value> &e) {
        if (e.bucket >= buckets.size()) {
            exit(32);
        }
        auto &keys = buckets[e.bucket].keys;
        if (keys[e.index] == e.key) {
            int lastElement = keys.size() - 1;
            if (e.index < lastElement) {
                // swap
                Key otherKey = keys[lastElement];
                keys[e.index] = otherKey;
                elements[_key2Index(otherKey)].index = e.index;
            } else if (e.index > lastElement) {
                exit(32);
            }
            e.bucket = -1;
            keys.pop_back();
        }
    }

    inline void removeReferences(KHeapElem<Value> &e) {
        if (e.bucket == -1) {
            return;
        }
        if (e.bucket == -2) {
            heap.remove(e.key);
            if (heap.empty()) {
                _handleEmptyHeap();
            }
        } else {
            removeFromBucket(e);
        }
    }


    inline void _insert(KHeapElem<Value> &e) {
        if (heap.size() < capacity || buckets.empty()) {
            if (maxVal < e.value) {
                maxVal = e.value;
            }
            addToHeap(e);
        }
        else if (e.value < maxVal) {
            addToHeap(e);
        }
        else {
            addToBuffer(e);
        }
    }


    inline void addToHeap(KHeapElem<Value> &e) {
//        std::cerr << "addToHeap(" << e.key << "," << e.value << ")\n";
        heap.set(e.key, e.value);
        e.bucket = -2;

        if (heap.size() > capacity * 2) {
            _handleFullHeap();
        }

    }

    inline void addToBuffer(KHeapElem<Value> &e) {
//        std::cerr << "addToBuffer(" << e.key << "," << e.value << ")\n";
        int i = 0;
        for (auto &bucket : buckets) {
            if (e.value < bucket.minVal && i < buckets.size() - 1) {
                i++;
                continue;
            }
            e.bucket = i;
            bucket.add(e);
            return;
        }
        std::cerr << "EROOR\n";
        exit(123);
    }


    void _handleFullHeap() {
        // Create a new heap with capacity elements and put rest of elements in a new buffer.
//        std::cerr << "*******handleFullHeap()\n";
        if (!buckets.empty()) {
            buckets.back().minVal = maxVal;
        }
        maxVal = 0;

        std::deque<int> keys;
        while (heap.size() > 0) {
            auto a = heap.top();
            keys.push_back(a.key);
            heap.pop();
        }

        buckets.push_back(KHeapBucket());

        for (auto &key : keys) {
            KHeapElem<Value> &e = elements[_key2Index(key)];
            _insert(e);
        }
    }

    inline int _key2Index(Key key) {
        if (key >= keys.size() || key < 0)
            return -1;
        int index = keys[key];
        if (index >= elements.size() || index < 0) {
            return -1;
        }
        return index;
    }

    void _handleEmptyHeap() {
//        std::cerr << "EMPTY HEAP\n";
        maxVal = 0;
        while (!buckets.empty() && heap.size() < capacity) {
            auto keys = buckets.back().keys;
            for (auto key : keys) {
                int index = _key2Index(key);
                KHeapElem<Value> &e = elements[index];
                removeFromBucket(e);
            }
            buckets.pop_back();

            for (auto key : keys) {
                int index = _key2Index(key);
                KHeapElem<Value> &e = elements[index];
                _insert(e);
            }
        }
    }

};


#endif //__KHeap_H_
