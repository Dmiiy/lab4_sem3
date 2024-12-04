#ifndef DYNAMIC_WEIGHT_SHORTEST_PATH_H
#define DYNAMIC_WEIGHT_SHORTEST_PATH_H

#include "Graph.h"
#include "../sequence/ArraySequence.h"
#include "../sequence/PriorityQueue.h"
#include "../sequence/Pair.h"
#include <limits>
#include <functional>
#include <stdexcept>
#include <chrono>

/**
 * @brief Класс для поиска кратчайших путей в графе с динамическими весами ребер с помощью алгоритма Дейкстры.
 */
class DynamicWeightShortestPath {
public:
    typedef int Vertex;    // Тип для обозначения вершины
    typedef double Weight; // Тип для обозначения веса ребра

    /**
     * @brief Функция для обновления весов ребер в зависимости от текущего времени.
     */
    std::function<Weight(Weight)> updateWeightFunction;

    /**
     * @brief Конструктор класса.
     *
     * @param updateFunc Функция для обновления весов ребер.
     */
    DynamicWeightShortestPath(std::function<Weight(Weight)> updateFunc)
            : updateWeightFunction(updateFunc) {}

    /**
     * @brief Реализация алгоритма Дейкстры с динамическим изменением весов ребер.
     *
     * @param graph Ссылка на граф, в котором выполняется поиск.
     * @param source Исходная вершина (источник).
     * @return ArraySequence<Pair<Weight, Vertex>> Последовательность пар, где каждая пара содержит расстояние от источника и предшествующую вершину для каждой вершины графа.
     *
     * @throws std::out_of_range Если исходная вершина находится вне допустимого диапазона.
     */
    ArraySequence<Pair<Weight, Vertex>> dijkstra(const Graph<Weight>& graph, Vertex source) const {
        int n = graph.getVertexCount(); // Получаем количество вершин в графе

        // Инициализация массивов расстояний и предшественников
        ArraySequence<Weight> distances;
        ArraySequence<Vertex> predecessors;

        for (int i = 0; i < n; ++i) {
            distances.append(std::numeric_limits<Weight>::infinity()); // Устанавливаем расстояние до всех вершин как бесконечность
            predecessors.append(-1); // Инициализируем предшественников как -1 (нет предшественника)
        }

        // Проверка валидности исходной вершины
        if (source < 0 || source >= n) {
            throw std::out_of_range("Source vertex out of range");
        }

        distances[source] = 0; // Расстояние до источника равно 0

        // Приоритетная очередь вершин, сортируемых по текущему известному расстоянию от источника
        PriorityQueue<Vertex, Weight> pq;
        pq.Enqueue(source, 0.0); // Добавляем исходную вершину в очередь

        // Основной цикл алгоритма Дейкстры
        while (!pq.isEmpty()) {
            auto current = pq.Dequeue(); // Извлекаем вершину с минимальным расстоянием
            Vertex u = current.first;
            Weight dist_u = current.second;

            // Если извлеченное расстояние больше известного, пропускаем вершину
            if (dist_u > distances[u]) continue;

            // Получаем соседей текущей вершины
            auto neighbors = graph.getNeighbors(u);
            // Итерируемся по всем соседям
            for (int i = 0; i < neighbors.getLength(); ++i) {
                Vertex v = neighbors[i].first;          // Соседняя вершина
                Weight originalWeight = neighbors[i].second; // Оригинальный вес ребра

                // Обновляем вес ребра в зависимости от текущего времени
                Weight weight = updateWeightFunction(originalWeight);

                // Проверяем, можно ли улучшить путь до соседа через текущую вершину
                if (distances[u] + weight < distances[v]) {
                    distances[v] = distances[u] + weight; // Обновляем расстояние до соседа
                    predecessors[v] = u;                   // Обновляем предшественника
                    pq.Enqueue(v, distances[v]);           // Добавляем/обновляем соседнюю вершину в очереди
                }
            }
        }

        // Формируем результат: для каждой вершины сохраняем расстояние и предшественника
        ArraySequence<Pair<Weight, Vertex>> result;
        for (int i = 0; i < n; ++i) {
            result.append(Pair<Weight, Vertex>(distances[i], predecessors[i]));
        }

        return result;
    }

    /**
     * @brief Восстанавливает путь от источника до целевой вершины.
     *
     * @param data Последовательность пар расстояний и предшественников для каждой вершины.
     * @param target Целевая вершина, до которой требуется восстановить путь.
     * @return ArraySequence<Vertex> Последовательность вершин, представляющая путь от источника до целевой вершины.
     */
    static ArraySequence<Vertex> getPath(const ArraySequence<Pair<Weight, Vertex>>& data, Vertex target) {
        ArraySequence<Vertex> path;
        Vertex current = target;

        // Построение пути путем перехода от целевой вершины к исходной через предшественников
        while (current != -1) {
            path.prepend(current);          // Добавляем вершину в начало пути
            current = data[current].second; // Переходим к предшественнику
        }

        return path;
    }
};

#endif // DYNAMIC_WEIGHT_SHORTEST_PATH_H