#ifndef UNDIRECTEDGRAPH_H
#define UNDIRECTEDGRAPH_H

#include "Graph.h"
#include <set>
#include <stack>

template<class T>
class UndirectedGraph : public Graph<T> {
private:
    IDictionaryBinaryTree<int, IDictionaryBinaryTree<int, T>> adjacencyList;
    int vertexCount;

public:
    explicit UndirectedGraph(int vertices) : vertexCount(vertices) {
        // Инициализируем список смежности пустыми словарями для каждой вершины
        for (int i = 0; i < vertices; ++i) {
            adjacencyList.Add(i, IDictionaryBinaryTree<int, T>());
        }
    }

    void addEdge(int from, int to, T weight) override {
        if (from < 0 || from >= vertexCount || to < 0 || to >= vertexCount) {
            throw std::out_of_range("Invalid vertex index");
        }
        adjacencyList.GetReference(from).Add(to, weight);
        adjacencyList.GetReference(to).Add(from, weight);
    }

    void removeEdge(int from, int to) override {
        bool removed = false;
        if (adjacencyList.ContainsKey(from) && adjacencyList.Get(from).ContainsKey(to)) {
            adjacencyList.GetReference(from).Remove(to);
            removed = true;
        }
        if (adjacencyList.ContainsKey(to) && adjacencyList.Get(to).ContainsKey(from)) {
            adjacencyList.GetReference(to).Remove(from);
            removed = true;
        }
        if (!removed) {
            throw std::invalid_argument("Edge not found");
        }
    }

    bool hasEdge(int from, int to) const override {
        return adjacencyList.ContainsKey(from) && adjacencyList.Get(from).ContainsKey(to);
    }

    int getDegree(int vertex) const override {
        if (!adjacencyList.ContainsKey(vertex)) {
            throw std::out_of_range("Vertex not found");
        }
        return adjacencyList.Get(vertex).GetCount();
    }

    ArraySequence<Pair<int, T>> getNeighbors(int vertex) const override {
        ArraySequence<Pair<int, T>> neighbors;
        if (adjacencyList.ContainsKey(vertex)) {
            auto neighborDict = adjacencyList.Get(vertex);
            auto keys = neighborDict.GetKeys();
            for (int i = 0; i < keys.getLength(); ++i) {
                int neighbor = keys[i];
                T weight = neighborDict.Get(neighbor);
                neighbors.append(Pair<int, T>(neighbor, weight));
            }
        }
        return neighbors;
    }

    T getEdgeWeight(int from, int to) const override {
        if (hasEdge(from, to)) {
            return adjacencyList.Get(from).Get(to);
        }
        throw std::invalid_argument("Edge not found");
    }

    void printGraph() const override {
        auto keys = adjacencyList.GetKeys();
        std::set<std::pair<int, int>> printedEdges;
        for (int i = 0; i < keys.getLength(); ++i) {
            int from = keys[i];
            auto neighborDict = adjacencyList.Get(from);
            auto neighbors = neighborDict.GetKeys();
            for (int j = 0; j < neighbors.getLength(); ++j) {
                int to = neighbors[j];
                if (printedEdges.find(std::make_pair(to, from)) == printedEdges.end()) {
                    T weight = neighborDict.Get(to);
                    std::cout << "Edge (" << from << " -- " << to << ") with weight: " << weight << std::endl;
                    printedEdges.insert(std::make_pair(from, to));
                }
            }
        }
    }

    int getVertexCount() const override {
        return vertexCount;
    }

    // Метод DFS
    void dfs(int startVertex, ArraySequence<bool>& visited, std::function<void(int)> visit = nullptr) const {
        if (startVertex < 0 || startVertex >= vertexCount) {
            throw std::out_of_range("Invalid start vertex");
        }
        if (visited.getLength() != vertexCount) {
            ArraySequence<bool>falseVisited;
            for (int i = 0; i < vertexCount; ++i) {
                falseVisited.append(false);
            }
            visited = falseVisited;
        }
        dfsUtil(startVertex, visited, visit);
    }

    ArraySequence<std::tuple<int, int, T>> getEdges() const override {
        ArraySequence<std::tuple<int, int, T>> edges;
        std::set<std::pair<int, int>> seen; // Для избегания дублирования рёбер

        auto keys = adjacencyList.GetKeys();
        for (int i = 0; i < keys.getLength(); ++i) {
            int from = keys[i];
            auto neighborDict = adjacencyList.Get(from);
            auto neighbors = neighborDict.GetKeys();
            for (int j = 0; j < neighbors.getLength(); ++j) {
                int to = neighbors[j];
                if (from < to && seen.find(std::make_pair(from, to)) == seen.end()) {
                    T weight = neighborDict.Get(to);
                    edges.append(std::make_tuple(from, to, weight));
                    seen.emplace(from, to);
                }
            }
        }

        return edges;
    }

    bool hasPath(int from, int to) const {
        if (from < 0 || from >= vertexCount || to < 0 || to >= vertexCount) {
            throw std::out_of_range("Invalid vertex index");
        }
        if (from == to) return true;

        ArraySequence<bool> visited(vertexCount, false);
        std::stack<int> stack;
        stack.push(from);
        visited[from] = true;

        while (!stack.empty()) {
            int current = stack.top();
            stack.pop();

            auto neighbors = getNeighbors(current);
            for (int i = 0; i < neighbors.getLength(); ++i) {
                int neighbor = neighbors[i].first;
                if (neighbor == to) return true;
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    stack.push(neighbor);
                }
            }
        }

        return false;
    }

    IDictionaryBinaryTree<int, IDictionaryBinaryTree<int, T>> getAdjacencyList() const {
        return adjacencyList;
    }

    ~UndirectedGraph() override = default;

private:
    void dfsUtil(int vertex, ArraySequence<bool>& visited, std::function<void(int)> visit) const {
        visited[vertex] = true;
        if (visit) {
            visit(vertex);
        }

        auto neighbors = getNeighbors(vertex);
        for (int i = 0; i < neighbors.getLength(); ++i) {
            int neighbor = neighbors[i].first;
            if (!visited[neighbor]) {
                dfsUtil(neighbor, visited, visit);
            }
        }
    }
};

#endif // UNDIRECTEDGRAPH_H