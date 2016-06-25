#ifndef INFOMAP_2_0_TREE_H
#define INFOMAP_2_0_TREE_H

#include <vector>
#include <map>
#include <iostream>

class Leaf;

class INode {
public:
    virtual void getLeaves(std::vector <Leaf> &leaves) {
    }

    INode(int index) : index(index) { }

    int index = 0;

    int getIndex() {
        return index;
    }

    virtual std::vector<int> getChildrens() = 0;

};

typedef std::pair<INode *, INode *> NodePair;

class Node : public INode {


public:

    INode *left;
    INode *right;
    double weight = 0;

    std::vector<int> childrens;

    Node(INode *left, INode *right, int index, double weight = 0.0) : INode(index), left(left), right(right),
                                                                      weight(weight) {
        childrens.push_back(left->getIndex());
        childrens.push_back(right->getIndex());
    };

    void getLeaves(std::vector <Leaf> &leaves) {
        left->getLeaves(leaves);
        right->getLeaves(leaves);
    }

    std::vector<int> getChildrens(){
        return childrens;
    }

};


class Leaf : public INode {

public:
    int nodeIndex;

    std::vector<int> childrens;

    Leaf(int nodeIndex, int index) : INode(index), nodeIndex(nodeIndex) {
    }

    void getLeaves(std::vector <Leaf> &leaves) {
        leaves.push_back(*this);
    }

    std::vector<int> getChildrens(){
        return childrens;
    }
};

class Tree {

    double value;
    double initValue;


    void split() {
        int i = stack.back();
        stack.pop_back();
//        std::cout << "stack remove " << i << std::endl;
        heads.erase(i);
//        std::cout << "head erase " << i << std::endl;
        INode *node = nodes[i];
        for(int childIndex : node->getChildrens()){
            heads[childIndex] = nodes[childIndex];
//            std::cout << "head add " << childIndex << std::endl;
        }
        value = values.back();
        values.pop_back();
        setDiff();
    }



    std::map<int, INode *> nodes; // all nodes
    std::map<int, INode *> heads; // all aggregates
    std::vector <int> stack; // contains all aggregations nodes
    std::vector <double> values;
    
    void prepare(double w = 0){
        if(heads.size() == 0 && stack.size() > 0){
            int i = stack.back();
            heads[i] = nodes[i];
        }
    }

    int leaves = 0;



    void setDiff(){
        if(empty()){
            m_diff = -1;
        }
        m_diff = getValue() - values.back();
    }

    int getNumberOfNodes(){
        return leaves - stack.size();
    }


public:


    bool empty(){
        return stack.size() == 0;
    }

    double m_diff = -1;

    void addLeaf(int index, int from) {
        nodes[index] = new Leaf(from, index);
        leaves++;
    }

    int getNumberOfLeaves(){
        return leaves;
    }

    double diff(){
        return m_diff;
    }

    void init(double w){
        initValue = w;
        value = w;
    }

    void merge(int indexA, int indexB, int newIndex, double weight = 0) {
        nodes[newIndex] = new Node(nodes[indexA], nodes[indexB], newIndex, weight);
        stack.push_back(newIndex);
        values.push_back(value);
        value = weight;


        if(getNumberOfNodes() ==  1){
            prepare();
        }
        setDiff();
    }

    bool step(){
        if(!empty()){
            split();
        }
        return !empty();
    }

    double getInitValue(){
        return initValue;
    }

    double getValue(){
        return value;
    }

    void getPartitions(std::vector<std::vector<int>> & partitions){
        if(getNumberOfNodes() ==  1){
            prepare();
        }
        partitions.clear();
        if(leaves > 1){
            for (auto &it : heads) {
                partitions.push_back(std::vector<int>());
                std::vector<int> &temp = partitions.back();
                INode *node = it.second;
                std::vector <Leaf> leaves;
                node->getLeaves(leaves);
                for (Leaf &leaf : leaves) {
                    temp.push_back(leaf.nodeIndex);
                }
            }
        }else if(leaves == 1 && nodes.size() == 1){
            partitions.push_back(std::vector<int>());
            std::vector<int> &temp = partitions.back();
            INode *node = nodes[0];
            std::vector <Leaf> leaves;
                node->getLeaves(leaves);
                for (Leaf &leaf : leaves) {
                    temp.push_back(leaf.nodeIndex);
                }
        }
    }
};


#endif //INFOMAP_2_0_TREE_H
