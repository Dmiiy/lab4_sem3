#ifndef GRAPH_H
#define GRAPH_H

#include <stdexcept>
#include <iostream>
#include "../sequence/ArraySequence.h"
#include "../sequence/Pair.h"
#include "../data_structures/IDictionaryBinaryTree.h"

template<class T>
class Graph {
public:
    virtual ~Graph() = default;

    virtual void addEdge(int from, int to, T weight) = 0;
    virtual void removeEdge(int from, int to) = 0;
    virtual bool hasEdge(int from, int to) const = 0;
    virtual int getDegree(int vertex) const = 0;
    virtual ArraySequence<Pair<int, T>> getNeighbors(int vertex) const = 0;
    virtual T getEdgeWeight(int from, int to) const = 0;
    virtual void printGraph() const = 0;
    virtual int getVertexCount() const = 0;
    virtual void dfs(int startVertex, ArraySequence<bool>& visited, std::function<void(int)> visit) const = 0;
    virtual ArraySequence<std::tuple<int, int, T>> getEdges() const = 0;
};

#endif