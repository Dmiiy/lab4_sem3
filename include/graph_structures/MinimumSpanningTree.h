#ifndef LAB4_SEM3_MINIMUMSPANNINGTREE_H
#define LAB4_SEM3_MINIMUMSPANNINGTREE_H

#include "Graph.h"
#include <algorithm>
#include <functional>
#include "QuickSorter.h"
template <typename T>
class MinimumSpanningTree {
public:
    // Метод для поиска минимального остова с использованием алгоритма Крускала
    static ArraySequence<std::tuple<int, int, T>> kruskal(int vertices, const Graph<T>& graph) {
        ArraySequence<int> parent;  // Массив родительских вершин для объединения
        for (int i = 0; i < vertices; ++i) {
            parent.append(i);  // Изначально каждая вершина является своим родителем
        }

        // Используем std::function для рекурсивного вызова find
        std::function<int(int)> find = [&](int v) -> int {
            if (parent[v] != v) {
                parent[v] = find(parent[v]);  // Поиск сжатия пути
            }
            return parent[v];
        };

        auto unite = [&](int u, int v) {
            int rootU = find(u);
            int rootV = find(v);
            if (rootU != rootV) {
                parent[rootU] = rootV;  // Объединение двух компонент
            }
        };

        // Получаем рёбра из графа
        ArraySequence<std::tuple<int, int, T>> mst;  // Для хранения рёбер минимального остова
        ArraySequence<std::tuple<int, int, T>> edges = graph.getEdges();

        // Сортируем рёбра по весу
        ArraySequence<std::tuple<int, int, T>> sortedEdges = edges;
        QuickSorter<std::tuple<int, int, T>> sorter([](const std::tuple<int, int, T>& a, const std::tuple<int, int, T>& b) {
            return std::get<2>(a) < std::get<2>(b);  // Сортировка по весу рёбер
        });

        sorter.sort(&sortedEdges);


        // Проходим по отсортированным рёбрам и строим минимальный остов
        for (int i = 0; i < sortedEdges.getLength(); ++i) {
            const auto& edge = sortedEdges[i];
            int u = std::get<0>(edge);
            int v = std::get<1>(edge);
            T weight = std::get<2>(edge);

            if (find(u) != find(v)) {
                mst.append(edge);  // Добавляем ребро в минимальный остов
                unite(u, v);  // Объединяем компоненты
                if (mst.getLength() == vertices - 1) {
                    break; // Остов построен
                }
            }
        }

        return mst;  // Возвращаем найденный минимальный остов
    }
};

#endif //LAB4_SEM3_MINIMUMSPANNINGTREE_H