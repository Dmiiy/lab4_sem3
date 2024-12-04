#ifndef STRONGLY_CONNECTED_COMPONENTS_H
#define STRONGLY_CONNECTED_COMPONENTS_H

#include "DirectedGraph.h"
#include "../sequence/ArraySequence.h"
#include <functional>
#include <stdexcept>

/**
 * @brief Класс для поиска сильно связанных компонент в ориентированном графе.
 *
 * Этот класс реализует алгоритм Косарайю для нахождения всех сильно связанных компонент
 * в заданном ориентированном графе. Сильно связанные компоненты представляют собой
 * подмножества вершин, где каждая вершина достижима из любой другой вершины внутри
 * этого множества.
 *
 * @tparam T Тип данных, ассоциированный с рёбрами графа (например, вес рёбер).
 */
template <typename T>
class StronglyConnectedComponents {
public:
    /**
     * @brief Находит сильно связанные компоненты графа с использованием алгоритма Косарайю.
     *
     * Метод выполняет два прохода глубинного обхода (DFS). Первый проход используется
     * для определения порядка завершения обхода вершин, а второй проход выполняется
     * на транспонированном графе для выявления сильно связанных компонент.
     *
     * @param graph Ориентированный граф, в котором необходимо найти сильно связанные компоненты.
     * @return ArraySequence<ArraySequence<int>> Список сильно связанных компонент,
     * каждая из которых представлена списком вершин.
     */
    static ArraySequence<ArraySequence<int>> findSCC(const DirectedGraph<T>& graph) {
        int vertexCount = graph.getVertexCount();
        ArraySequence<bool> visited; // Массив посещённых вершин

        // Инициализируем массив посещённых вершин значениями false
        for(int i = 0; i < vertexCount; ++i){
            visited.append(false);
        }

        ArraySequence<int> finishOrder; // Порядок завершения обхода

        // Первый проход DFS для заполнения порядка завершения
        std::function<void(int)> dfs_first_pass = [&](int v) {
            visited[v] = true;
            auto neighbors = graph.getNeighbors(v);
            for(int i = 0; i < neighbors.getLength(); ++i){
                int neighbor = neighbors[i].first;
                if(!visited[neighbor]){
                    dfs_first_pass(neighbor);
                }
            }
            finishOrder.append(v); // Добавляем вершину после обхода её соседей
        };

        // Запуск первого прохода DFS для всех непосещённых вершин
        for(int v = 0; v < vertexCount; ++v){
            if(!visited[v]){
                dfs_first_pass(v);
            }
        }

        // Получаем транспонированный граф (граф с инвертированными рёбрами)
        DirectedGraph<T> transposedGraph = graph.getTranspose(graph);

        // Сбрасываем массив посещённых вершин для второго прохода
        for(int i = 0; i < vertexCount; ++i){
            visited[i] = false;
        }

        ArraySequence<ArraySequence<int>> sccList; // Список сильно связанных компонент

        // Второй проход DFS по транспонированному графу в порядке обратном завершения
        for(int i = finishOrder.getLength() - 1; i >=0; --i){
            int v = finishOrder[i];
            if(!visited[v]){
                ArraySequence<int> scc;

                // Второй проход DFS для сбора вершин текущей компоненты
                std::function<void(int)> dfs_second_pass = [&](int u) {
                    visited[u] = true;
                    scc.append(u);
                    auto neighbors = transposedGraph.getNeighbors(u);
                    for(int j = 0; j < neighbors.getLength(); ++j){
                        int neighbor = neighbors[j].first;
                        if(!visited[neighbor]){
                            dfs_second_pass(neighbor);
                        }
                    }
                };

                dfs_second_pass(v);
                sccList.append(scc); // Добавляем найденную компоненту в список
            }
        }

        return sccList; // Возвращаем список всех сильно связанных компонент
    }

};

#endif // STRONGLY_CONNECTED_COMPONENTS_H