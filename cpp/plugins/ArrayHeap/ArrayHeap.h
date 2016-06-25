#ifndef __ArrayHeap_H_
#define __ArrayHeap_H_

#include "Heap.h"
#include <iostream>

template<typename Value>
struct Element {
    int key;
    Value val;

    Element(int key, Value v) : key(key), val(v) {
    }

    bool operator<(Element const &b) {
        return val < b.val;
    }

    friend std::ostream &operator<<(std::ostream &os, const Element &obj) {
        return os << obj.key << " " << obj.val;
    }

};

template<typename Value = int>
class ArrayHeap : public Heap<Element<Value>> {

    typedef Element<Value> E;
    typedef Heap<E> Base;

public:
    std::vector<int> _k;

public:

    void print() {
        std::cout << "VALUES\n";
        for (auto &h : Base::_v) {
            std::cout << h << std::endl;
        }
        std::cout << "KEYS\n";
        for (int i = 0; i < _k.size(); i++) {
            std::cout << i << " " << _k[i] << std::endl;
        }
        std::cout << "END\n";
    }

    void set(int key, Value value) {
        if (key >= _k.size()) {
            _k.resize(key + 1, -1);
        }
        int index = _k[key];

        if (index == -1) {
            index = Base::size();
            E e(key, value);
            updateReference(key, index);
            Base::set(e);
        }
        else {
            Base::_v[index].val = value;
            _update(index);
        }
    }

    Value &get(int key) {
        return _get(key2index(key));
    }

    int remove(int key) {
        int index = key2index(key);
        if (index != -1) {
            int index = key2index(key);
            if (Base::_v[index].key != key) {
                std::cout << "Key failure: " << Base::_v[index].key << " / " << key << std::endl;
                exit(1);
            }
            _pop(index);
        } else {
            // Trying to remove an object with key that not exists
        }
        return Base::size();
    }

    void update(int key) {
        _update(key2index(key));
    }

private:

    Value &_get(int index) {
        return Base::_v[index].val;
    }

    inline int key2index(int key) {
        if (key >= _k.size() || key < 0)
            return -1;
        int index = _k[key];
        if (index >= Base::_v.size() || index < 0) {
            return -1;
        }
        return index;
    }

    void _update(int index) {
        Base::_bubbleUp(index);
        Base::_bubbleDown(index);
    }

    void _pop(int index = 0) {
        int key = Base::_v[index].key;
        Base::_pop(index);
        updateReference(key, -1);
    }

    void updateReference(int key) {
        int index = key2index(key);
        _k[key] = Base::_v[index]->key;
    }

    void updateReference(int key, int index) {
        _k[key] = index;
    }

    void _updateReference(int index) {
        updateReference(Base::_v[index].key, index);
    }


    void _swap(int index1, int index2) {
        std::swap(Base::_v[index1], Base::_v[index2]);
        _updateReference(index1);
        _updateReference(index2);
    }

    inline int _index2key(int index) {
        int key = Base::_v[index]->key;
        if (key >= _k.size() || key < 0)
            return -1;
        return key;
    }
};

#endif //__ArrayHeap_H_
