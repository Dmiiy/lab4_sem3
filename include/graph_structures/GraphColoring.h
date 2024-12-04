// GraphColoring.h
#ifndef GRAPH_COLORING_H
#define GRAPH_COLORING_H

#include "Graph.h"
#include <algorithm>
#include <stdexcept>
#include "../sequence/ArraySequence.h"
#include "../sequence/Pair.h"

/**
 * @brief Класс для раскраски графа.
 */
class GraphColoring {
public:
    /**
     * @brief Выполняет жадную раскраску графа.
     *
     * @tparam T Тип веса рёбер.
     * @param graph Граф для раскраски.
     * @return ArraySequence<int> Список цветов для каждой вершины.
     */
    template<typename T>
    static ArraySequence<int> greedyColoring(const Graph<T>& graph) {
        int n = graph.getVertexCount();
        ArraySequence<int> result(-1,n);
        //result.resize(n, -1);   // Инициализация всех вершин как некрашенных

        if (n == 0) {
            return result;
        }

        result[0] = 0; // Присваиваем первый цвет первой вершине

        // Для каждой вершины выбираем наименьший доступный цвет
        for (int u = 1; u < n; ++u) {
            ArraySequence<bool> available(true,n); // Массив доступных цветов

            ArraySequence<Pair<int, T>> neighbors = graph.getNeighbors(u);

            // Помечаем недоступные цвета, занятые соседями
            for (int i = 0; i < neighbors.getLength(); ++i) {
                int neighbor = neighbors[i].first;

                if (neighbor < 0 || neighbor >= n) {
                    throw std::out_of_range("Neighbor index out of range");
                }

                if (result[neighbor] != -1) {
                    available[result[neighbor]] = false;
                }
            }

            // Находим первый доступный цвет
            int color = 0;
            while (color < n && !available[color]) {
                ++color;
            }

            result[u] = color;
        }

        return result;
    }
};

#endif // GRAPH_COLORING_H