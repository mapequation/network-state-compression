#include "./Tree.h"


void print(std::vector<std::vector<int>> &partitions, double value, double diff) {
    std::string str = "";
    for(auto & partition : partitions){
        for(auto & leaf : partition){
            str += std::to_string(leaf) + " , ";
        }
        str = str.substr(0, str.size()-3);
        str += " | ";
    }
    std::cout << str.substr(0, str.size()-3) << "    " << value << "     " << diff << std::endl;
}


int main2() {

    Tree tree;

    // tree.addLead(index, prior);
    tree.addLeaf(0, 12);
    tree.addLeaf(1, 1);
    tree.addLeaf(2, 2);
    tree.addLeaf(3, 332);
    tree.addLeaf(4, 4);
    tree.addLeaf(5, 5);
    tree.addLeaf(6, 6);
    tree.addLeaf(7, 7);

    tree.init(1.01);

    // tree.merge(indexA, indexB, newIndex, weight);
    tree.merge(0, 3, 8, 1.05);
    tree.merge(5, 6, 9, 1.1);
    tree.merge(9, 2, 10, 1.2);
    tree.merge(1, 4, 11, 1.45);
    tree.merge(11, 8, 12, 1.6);
    tree.merge(10, 12, 13, 1.8);
    tree.merge(13, 7, 14, 2);


    std::cout << tree.diff() << std::endl;

    std::vector <std::vector<int>> partitions;
    do {
        tree.getPartitions(partitions);
        print(partitions, tree.getValue(), tree.diff());
    } while (tree.step());


    tree.getPartitions(partitions);
    print(partitions, tree.getValue(), tree.diff());

    return 0;
}