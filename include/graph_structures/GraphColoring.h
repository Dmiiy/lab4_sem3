#ifndef GRAPH_COLORING_H
#define GRAPH_COLORING_H

#include "Graph.h"
#include <algorithm>
#include <stdexcept>
#include "../sequence/ArraySequence.h"
#include "../sequence/Pair.h"

/**
 * @brief Класс для раскраски графа.
 *
 * Класс `GraphColoring` предоставляет метод для выполнения жадной раскраски графа.
 * Жадный алгоритм раскраски назначает минимальное возможное число цветов вершинам графа
 * таким образом, чтобы никакие две смежные вершины не имели одинакового цвета.
 */
class GraphColoring {
public:
    /**
     * @brief Выполняет жадную раскраску графа.
     *
     * Метод `greedyColoring` назначает цвета вершинам графа, используя жадный подход.
     * Приоритет цвета отдается порядку следования вершин.
     *
     * @tparam T Тип веса рёбер графа.
     * @param graph Граф, который необходимо раскрасить.
     * @return ArraySequence<int> Последовательность цветов для каждой вершины графа,
     *                           где индекс соответствует номеру вершины, а значение — цвету.
     *
     * @throws std::out_of_range Если индекс соседа выходит за пределы допустимого диапазона.
     */
    template<typename T>
    static ArraySequence<int> greedyColoring(const Graph<T>& graph) {
        int n = graph.getVertexCount(); /**< Общее количество вершин в графе. */
        ArraySequence<int> result(-1, n); /**< Массив для хранения цвета каждой вершины. Инициализируется значением -1 (неокрашенные). */

        // Если граф пустой, возвращаем массив без изменений
        if (n == 0) {
            return result;
        }

        result[0] = 0; /**< Присваиваем первый цвет первой вершине. */

        /**
         * @brief Основной цикл раскраски.
         *
         * Присваиваем каждой вершине наименьший доступный цвет, который не используется
         * её смежными вершинами.
         */
        for (int u = 1; u < n; ++u) {
            ArraySequence<bool> available(true, n); /**< Массив доступных цветов. Изначально все цвета доступны. */

            ArraySequence<Pair<int, T>> neighbors = graph.getNeighbors(u); /**< Список соседей текущей вершины. */

            // Проходим по всем соседям текущей вершины
            for (int i = 0; i < neighbors.getLength(); ++i) {
                int neighbor = neighbors[i].first; /**< Индекс соседней вершины. */

                // Проверка валидности индекса соседа
                if (neighbor < 0 || neighbor >= n) {
                    throw std::out_of_range("Neighbor index out of range");
                }

                // Если сосед уже раскрашен, помечаем его цвет как недоступный
                if (result[neighbor] != -1) {
                    available[result[neighbor]] = false;
                }
            }

            // Находим первый доступный цвет
            int color = 0;
            while (color < n && !available[color]) {
                ++color;
            }

            result[u] = color; /**< Назначаем найденный цвет вершине u. */
        }

        return result; /**< Возвращаем массив с назначенными цветами для всех вершин. */
    }
};

#endif // GRAPH_COLORING_H