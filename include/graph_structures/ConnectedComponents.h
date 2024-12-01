#ifndef LAB4_SEM3_CONNECTEDCOMPONENTS_H
#define LAB4_SEM3_CONNECTEDCOMPONENTS_H

#include "Graph.h"
#include <stdexcept>

template <typename T>

class ConnectedComponents {
public:
    ConnectedComponents() = default;


    static ArraySequence<ArraySequence<int>> findComponents(const Graph<T>& graph) {
        int vertexCount = graph.getVertexCount();
        ArraySequence<bool> visited;
        for (int i = 0; i < vertexCount; ++i) {
            visited.append(false);
        }

        ArraySequence<ArraySequence<int>> components;

        for (int v = 0; v < vertexCount; ++v) {
            if (!visited[v]) {
                ArraySequence<int> component;
                dfs(v, graph, visited, component);
                components.append(component);
            }
        }

        return components;
    }

    ~ConnectedComponents() = default;

private:

    static void dfs(int v, const Graph<T>& graph, ArraySequence<bool>& visited, ArraySequence<int>& component) {
        visited[v] = true;
        component.append(v);

        ArraySequence<Pair<int, T>> neighbors = graph.getNeighbors(v);
        for (int i = 0; i < neighbors.getLength(); ++i) {
            int neighbor = neighbors[i].first;
            if (!visited[neighbor]) {
                dfs(neighbor, graph, visited, component);
            }
        }
    }
};

#endif //LAB4_SEM3_CONNECTEDCOMPONENTS_H