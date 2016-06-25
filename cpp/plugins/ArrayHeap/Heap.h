#ifndef __Heap_H_
#define __Heap_H_

#include <deque>
#include <vector>

template<typename Value = int>
class Heap {

protected:
    typedef std::vector<Value> Container;
    Container _v;

public:

    bool operator<(Heap const &b) {
        return _v[0] < b._v[0];
    }

    friend std::ostream &operator<<(std::ostream &os, const Heap &heap) {
        return os << heap._v[0];
    }

    void set(Value val) {
        int index = _v.size();
        _v.push_back(val);
        _bubbleUp(index);
    }

    Value &top() {
        return _v[0];
    }

    void pop() {
        _pop();
    }

    int size() {
        return _v.size();
    }

    inline bool empty(){
        return _v.empty();
    }

    typename Container::iterator begin() {
        return _v.begin();
    }

    typename Container::iterator end() {
        return _v.end();
    }

protected:

    virtual void _pop(int index = 0) {

        int length = _v.size();
        if (length == 0) return;
        if (index < 0) return;

        if (length == 1) {
            _v.pop_back();
            return;
        }

        _swap(index, length - 1);
        _v.pop_back();
        if (index != 0) {
            _bubbleUp(index);
        }
        _bubbleDown(index);
    }

    void _heapify() {
        for (int i = _v.size() - 1; i >= 0; --i) {
            _bubbleDown(i);
        }
    }

    void _bubbleDown(int index) {
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        int length = _v.size();

        if (leftChild >= length) {
            return;
        } //index is a leaf

        int minIndex = index;

        if (_v[leftChild] < _v[index]) {
            minIndex = leftChild;
        }

        if ((rightChild < length) && (_v[rightChild] < _v[minIndex])) {
            minIndex = rightChild;
        }

        if (minIndex != index) {
            _swap(index, minIndex);
            _bubbleDown(minIndex);
        }
    }

    void _bubbleUp(int index) {
        if (index == 0 || index >= size())
            return;

        int parent = (index - 1) / 2;

        if (_v[index] < _v[parent]) {
            _swap(parent, index);
            _bubbleUp(parent);
        }
    }

    virtual inline void _swap(int i1, int i2) {
        std::swap(_v[i1], _v[i2]);
    }


};


#endif //__Heap_H_
