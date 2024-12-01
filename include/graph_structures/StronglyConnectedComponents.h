// StronglyConnectedComponents.h
#ifndef LAB4_SEM3_STRONGLYCONNECTEDCOMPONENTS_H
#define LAB4_SEM3_STRONGLYCONNECTEDCOMPONENTS_H

#include "Graph.h"
#include <stdexcept>

/**
 * @brief Класс для поиска компонент сильной связности в ориентированном графе.
 */
class StronglyConnectedComponents {
public:
    /**
     * @brief Находит все компоненты сильной связности в ориентированном графе.
     *
     * @tparam T Тип веса рёбер графа.
     * @param graph Ссылка на ориентированный граф.
     * @return ArraySequence<ArraySequence<int>> Список компонент сильной связности.
     */
    template <typename T>
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
    /**
     * @brief Заполняет порядок завершения вершин с помощью DFS.
     *
     * @tparam T Тип веса рёбер графа.
     * @param v Текущая вершина.
     * @param graph Ссылка на граф.
     * @param visited Ссылка на массив посещённых вершин.
     * @param finishOrder Ссылка на массив порядка завершения.
     */
    template <typename T>
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

    /**
     * @brief Транспонирует граф.
     *
     * @tparam T Тип веса рёбер графа.
     * @param graph Ссылка на исходный граф.
     * @return Транспонированный граф.
     */
    template <typename T>
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

    /**
     * @brief Рекурсивный обход в глубину на транспонированном графе.
     *
     * @tparam T Тип веса рёбер графа.
     * @param v Текущая вершина.
     * @param graph Ссылка на транспонированный граф.
     * @param visited Ссылка на массив посещённых вершин.
     * @param scc Ссылка на текущую компоненту сильной связности.
     */
    template <typename T>
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