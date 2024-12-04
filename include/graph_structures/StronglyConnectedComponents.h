// StronglyConnectedComponents.h
#ifndef STRONGLY_CONNECTED_COMPONENTS_H
#define STRONGLY_CONNECTED_COMPONENTS_H

#include "DirectedGraph.h"
#include "../sequence/ArraySequence.h"
#include <functional>
#include <stack>

/**
 * @brief Класс для поиска сильно связанных компонент в ориентированном графе.
 */
template <typename T>
class StronglyConnectedComponents {
public:
    /**
     * @brief Находит сильно связанные компоненты графа с использованием алгоритма Косарайю.
     *
     * @param graph Ориентированный граф.
     * @return ArraySequence<ArraySequence<int>> Список сильно связанных компонент.
     */
    static ArraySequence<ArraySequence<int>> findSCC(const DirectedGraph<T>& graph) {
        int vertexCount = graph.getVertexCount();
        ArraySequence<bool> visited(false, vertexCount);
        //visited.resize(vertexCount, false); // Инициализация массива посещённых вершин
        std::stack<int> finishStack;

        // Первый проход DFS для заполнения стека порядком завершения
        for (int v = 0; v < vertexCount; ++v) {
            if (!visited[v]) {
                graphDfsFillOrder(graph, v, visited, finishStack);
            }
        }

        // Получаем транспонированный граф
        DirectedGraph<T> transposedGraph = getTranspose(graph);

        // Сбрасываем массив посещённых вершин
        //std::fill(visited.begin(), visited.end(), false);
        for (int i = 0; i < visited.getLength(); ++i) {
            visited[i] = false;

        }

        ArraySequence<ArraySequence<int>> sccList;

        // Второй проход DFS по транспонированному графу
        while (!finishStack.empty()) {
            int v = finishStack.top();
            finishStack.pop();

            if (!visited[v]) {
                ArraySequence<int> scc;
                // Lambda-функция для добавления вершины в текущую компоненту
                std::function<void(int)> visit = [&scc](int vertex) {
                    scc.append(vertex);
                };
                transposedGraph.dfs(v, visited, visit);
                sccList.append(scc);
            }
        }

        return sccList;
    }

private:
    /**
     * @brief Рекурсивный вспомогательный метод для первого прохода DFS.
     *
     * @param graph Граф.
     * @param vertex Текущая вершина.
     * @param visited Массив посещённых вершин.
     * @param finishStack Стек для хранения порядка завершения.
     */
    static void graphDfsFillOrder(const DirectedGraph<T>& graph, int vertex, ArraySequence<bool>& visited, std::stack<int>& finishStack) {
        visited[vertex] = true;

        auto neighbors = graph.getNeighbors(vertex);
        for (int i = 0; i < neighbors.getLength(); ++i) {
            int neighbor = neighbors[i].first;
            if (!visited[neighbor]) {
                graphDfsFillOrder(graph, neighbor, visited, finishStack);
            }
        }

        finishStack.push(vertex); // Добавляем вершину в стек после завершения
    }

    /**
     * @brief Создаёт транспонированный граф (инвертирует направления рёбер).
     *
     * @param graph Оригинальный граф.
     * @return DirectedGraph<T> Транспонированный граф.
     */
    static DirectedGraph<T> getTranspose(const DirectedGraph<T>& graph) {
        int vertexCount = graph.getVertexCount();
        DirectedGraph<T> transposedGraph(vertexCount);

        // Инвертируем направления всех рёбер
        for (int u = 0; u < vertexCount; ++u) {
            auto neighbors = graph.getNeighbors(u);
            for (int i = 0; i < neighbors.getLength(); ++i) {
                int v = neighbors[i].first;
                T weight = neighbors[i].second;
                transposedGraph.addEdge(v, u, weight); // Инвертируем ребро
            }
        }

        return transposedGraph;
    }
};

#endif // STRONGLY_CONNECTED_COMPONENTS_H