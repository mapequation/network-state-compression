#ifndef __memNet__Aggregates__
#define __memNet__Aggregates__

#include <map>
#include "../dependencies.h"
#include "../plogp.h"
#include "Aggregate.h"
#include "metric/Metric.h"
#include "metric/OutEntropyMetric.h"

class Aggregates {

    KMultiHeap multiHeap;
    std::map<int, double> fromFreqs;
    const int currentNode;
    Metric &m_metric;
    double freq;
    int counter;
    double metric0 = 0;
    double metricSum = 0;
    double p_out = 0;

    int nrOfDanglings, nrOfMemoryNodes;

    std::vector <Aggregate> danglings;

    void mergeAggregate(int id1, int id2) {
        Aggregate a = aggregates.find(id1)->second + aggregates.find(id2)->second;
        erase(id1);
        erase(id2);
        int newId = addAggregate(a);
        tree->merge(id1, id2, newId, totalOutEntropy());
    }

    int addAggregate(Aggregate &a) {

        for (auto &aggregate : aggregates) {
            double d = m_metric.metric_increase(a, aggregate.second);
            multiHeap.set(aggregate.first, counter, d);
        }
        aggregates.insert(std::make_pair(counter, a));
        p_out = sum(aggregates, [](const Aggregate &a) {
            return a.p_out;
        });
        return counter++;
    }

    void addDanglings(){
        if (danglings.size() > 0) {
            Aggregate res = danglings.back();
            danglings.pop_back();
            while (danglings.size() > 0) {
                res = res + danglings.back();
                danglings.pop_back();
            }
            addAggregate(res);
        }
    }


    inline void erase(int id) {
        multiHeap.remove(id);
        aggregates.erase(id);
    }


public:

    Tree *tree;

    double getFreq(){
        return freq;
    }

    double getTreeValue(){
        return tree->getValue();
    }

    double getTreeDiff(){
        return tree->diff();
    }

    int getPhysicalNode(){
        return currentNode;
    }

    int getNumberOfDanglings(){
        return nrOfDanglings;
    }
    int getNumberOfInitialStateNodes(){
        return nrOfMemoryNodes;
    }


    std::map<int, Aggregate> aggregates;

    double metric(Aggregate &a){
        return m_metric.metric(a);
    }


    double getInitialMetricValue(){
        return metric0;
    }

    double getMetricValue(){
        return metricSum;
    }


    Aggregates(int currentNode, Metric &metric) : currentNode(currentNode), m_metric(metric), freq(0), counter(0) {
        tree = new Tree();
    }

    int size() {
        return aggregates.size();
    }


    KMultiHeap::MultiHeapElement getMinimalMetric() {
        return multiHeap.top();
    }

    double smallest() {
        return multiHeap.top().val;
    }

    void mergeSmallest() {
        auto e = multiHeap.top();
        metricSum += e.val;
        mergeAggregate(e.key1, e.key2);
    }


    void prepare(){
        nrOfDanglings = danglings.size();

        addDanglings();

        nrOfMemoryNodes = aggregates.size();

        for (auto &b : aggregates) {
            metric0 += metric(b.second);
        }
        metricSum = metric0;

        tree->init(totalOutEntropy());

        std::cout << "prepare aggregates: " << currentNode << " " << metric0 << " " << freq << std::endl;
        if(metricSum < 0){
            printAggregates(true);
            exit(1);
        }
    }

    void addMemoryNode(int from, std::map<int, double> &out) {
        double w = 0;
        for(auto &a : out){
            w += a.second;
        }
        Aggregate agg(from, out);
        fromFreqs[from] = agg.p_out;
        freq += agg.p_out;
        if (w == 0) {
            std::cout << "added dangling\n";
            danglings.push_back(agg);
            return;
        }
        int counter = addAggregate(agg);
        tree->addLeaf(counter, from);
    }

    double totalOutEntropy() {
        return sum(aggregates, [](const Aggregate &a) {
            return a.H_out * a.p_out;
        });
    }

    double totalMetric() {
        double sum = 0;
        for (auto &a : aggregates) {
            sum += metric(a.second);
        }
        return sum;
    }

    void printAggregates(bool extended) {
        std::cout << "Size: " << aggregates.size() << std::endl;
        for (auto p : aggregates) {
            std::cout << p.first << " - ";
            p.second.print(extended);
            std::cout << std::endl;
        }
    }

};


#endif /* defined(__memNet__Aggregates__) */
