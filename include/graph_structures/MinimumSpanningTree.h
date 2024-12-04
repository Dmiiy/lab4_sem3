// MinimumSpanningTree.h
#ifndef LAB4_SEM3_MINIMUMSPANNINGTREE_H
#define LAB4_SEM3_MINIMUMSPANNINGTREE_H

#include "UndirectedGraph.h"
#include <functional>
#include "QuickSorter.h"
#include "../sequence/ArraySequence.h"
#include <tuple>

/**
 * @brief Класс для поиска минимального остова графа.
 */
template <typename T>
class MinimumSpanningTree {
public:
    /**
     * @brief Находит минимальный остов графа с использованием алгоритма Крускала.
     *
     * @param graph Неориентированный граф.
     * @return ArraySequence<std::tuple<int, int, T>> Список рёбер минимального остова.
     */
    static ArraySequence<std::tuple<int, int, T>> kruskal(const UndirectedGraph<T>& graph) {
        int vertices = graph.getVertexCount();
        ArraySequence<int> parent;
//        parent.resize(vertices);

        // Инициализируем каждую вершину как своего родителя
        for (int i = 0; i < vertices; ++i) {
            parent.append(i);
        }

        // Функция для нахождения корня сжатия пути
        std::function<int(int)> find = [&](int v) -> int {
            if (parent[v] != v) {
                parent[v] = find(parent[v]);  // Сжатие пути
            }
            return parent[v];
        };

        // Функция для объединения двух множеств
        std::function<void(int, int)> unite = [&](int u, int v) {
            int rootU = find(u);
            int rootV = find(v);
            if (rootU != rootV) {
                parent[rootU] = rootV;  // Объединение деревьев
            }
        };

        // Получаем все рёбра из графа
        ArraySequence<std::tuple<int, int, T>> mst;  // Минимальный остов
        ArraySequence<std::tuple<int, int, T>> edges = graph.getEdges();

        // Сортируем рёбра по весу с помощью быстрого сортировщика
        QuickSorter<std::tuple<int, int, T>> sorter([](const std::tuple<int, int, T>& a, const std::tuple<int, int, T>& b) {
            return std::get<2>(a) < std::get<2>(b);  // Сортировка по весу
        });

        sorter.sort(&edges);

        // Проходим по отсортированным рёбрам и добавляем их в остов
        for (int i = 0; i < edges.getLength(); ++i) {
            const auto& edge = edges[i];
            int u = std::get<0>(edge);
            int v = std::get<1>(edge);
            T weight = std::get<2>(edge);

            if (find(u) != find(v)) {
                mst.append(edge);      // Добавляем ребро в остов
                unite(u, v);           // Объединяем компоненты

                // Если остов содержит (vertices - 1) рёбер, алгоритм завершается
                if (mst.getLength() == vertices - 1) {
                    break;
                }
            }
        }

        return mst;  // Возвращаем минимальный остов
    }
};

#endif // LAB4_SEM3_MINIMUMSPANNINGTREE_H