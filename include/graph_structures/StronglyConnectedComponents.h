#ifndef LAB4_SEM3_STRONGLYCONNECTEDCOMPONENTS_H
#define LAB4_SEM3_STRONGLYCONNECTEDCOMPONENTS_H

#include "Graph.h"
#include <stdexcept>

template <typename T>
class StronglyConnectedComponents {
public:
    static ArraySequence<ArraySequence<int>> findSCC(const Graph<T>& graph) {
        int vertexCount = graph.getVertexCount();
        ArraySequence<bool> visited;
        for (int i = 0; i < vertexCount; ++i) {
            visited.append(false);
        }

        ArraySequence<int> finishOrder;
        for (int v = 0; v < vertexCount; ++v) {
            if (!visited[v]) {
                fillOrder(v, graph, visited, finishOrder);
            }
        }

        Graph<T> transposed = getTranspose(graph);

        for (int i = 0; i < vertexCount; ++i) {
            visited[i] = false;
        }

        ArraySequence<ArraySequence<int>> sccList;

        for (int i = finishOrder.getLength() - 1; i >= 0; --i) {
            int v = finishOrder[i];
            if (!visited[v]) {
                ArraySequence<int> scc;
                transposedDFS(v, transposed, visited, scc);
                sccList.append(scc);
            }
        }

        return sccList;
    }

private:

    static void fillOrder(int v, const Graph<T>& graph, ArraySequence<bool>& visited, ArraySequence<int>& finishOrder) {
        visited[v] = true;

        ArraySequence<Pair<int, T>> neighbors = graph.getNeighbors(v);
        for (int i = 0; i < neighbors.getLength(); ++i) {
            int neighbor = neighbors[i].first;
            if (!visited[neighbor]) {
                fillOrder(neighbor, graph, visited, finishOrder);
            }
        }

        finishOrder.append(v);
    }

    static Graph<T> getTranspose(const Graph<T>& graph) {
        int vertexCount = graph.getVertexCount();
        Graph<T> transposed(vertexCount);

        const ArraySequence<std::tuple<int, int, T>>& edges = graph.getEdges();
        for (int i = 0; i < edges.getLength(); ++i) {
            int from = std::get<1>(edges[i]);
            int to = std::get<0>(edges[i]);
            T weight = std::get<2>(edges[i]);
            transposed.addEdge(from, to, weight);
        }

        return transposed;
    }

    static void transposedDFS(int v, const Graph<T>& graph, ArraySequence<bool>& visited, ArraySequence<int>& scc) {
        visited[v] = true;
        scc.append(v);

        ArraySequence<Pair<int, T>> neighbors = graph.getNeighbors(v);
        for (int i = 0; i < neighbors.getLength(); ++i) {
            int neighbor = neighbors[i].first;
            if (!visited[neighbor]) {
                transposedDFS(neighbor, graph, visited, scc);
            }
        }
    }
};

#endif //LAB4_SEM3_STRONGLYCONNECTEDCOMPONENTS_H