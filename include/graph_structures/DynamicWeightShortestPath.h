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
#include <random>

/**
 * @brief Класс для поиска кратчайших путей в графе с динамическими весами ребер с помощью алгоритма Дейкстры.
 *
 * Класс `DynamicWeightShortestPath` предоставляет методы для выполнения алгоритма Дейкстры
 * с динамическим изменением весов рёбер в зависимости от времени выполнения алгоритма.
 */
template <typename T>
class DynamicWeightShortestPath {
public:
    typedef int Vertex; /**< Тип для обозначения вершины графа. */
    using Clock = std::chrono::steady_clock; /**< Тип для отслеживания времени. */
    using TimePoint = std::chrono::time_point<Clock>; /**< Тип для хранения временной метки. */

private:
    TimePoint startTime; /**< Временная метка начала выполнения алгоритма. */
    double timeInfluenceFactor; /**< Фактор влияния времени на изменение весов. */
    mutable std::mt19937 rng; /**< Генератор случайных чисел для добавления случайных вариаций. */
    mutable std::uniform_real_distribution<double> dist; /**< Распределение для случайных вариаций. */

public:
    /**
     * @brief Конструктор класса.
     *
     * @param factor Фактор влияния времени на изменение весов (по умолчанию 0.1).
     */
    DynamicWeightShortestPath(double factor = 0.1)
            : startTime(Clock::now())
            , timeInfluenceFactor(factor)
            , rng(std::random_device{}())
            , dist(-0.1, 0.1)
    {
        static_assert(std::is_arithmetic<T>::value, "Weight type must be numeric");
    }

    /**
     * @brief Обновляет вес ребра в зависимости от времени выполнения алгоритма.
     *
     * @param originalWeight Оригинальный вес ребра.
     * @return Обновленный вес ребра.
     */
    T updateWeight(T originalWeight) const {
        auto now = Clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();

        // Вычисляем множитель на основе времени
        double timeFactor = 1.0 + (elapsed * timeInfluenceFactor / 1000.0);

        // Добавляем случайную вариацию с использованием mutable членов
        double randomFactor = 1.0 + dist(rng);

        return static_cast<T>(originalWeight * timeFactor * randomFactor);
    }

    /**
     * @brief Реализация алгоритма Дейкстры с динамическим изменением весов ребер.
     *
     * @param graph Ссылка на граф, в котором выполняется поиск.
     * @param source Исходная вершина (источник).
     * @return ArraySequence<Pair<T, Vertex>> Последовательность пар, где каждая пара содержит расстояние от источника и предшествующую вершину для каждой вершины графа.
     *
     * @throws std::out_of_range Если исходная вершина находится вне допустимого диапазона.
     */
    ArraySequence<Pair<T, Vertex>> dijkstra(const Graph<T>& graph, Vertex source) {
        int n = graph.getVertexCount();
        if (source < 0 || source >= n) {
            throw std::out_of_range("Source vertex is out of range");
        }

        const T MAX_VALUE = std::numeric_limits<T>::max();
        ArraySequence<T> distances; /**< Массив для хранения кратчайших расстояний до вершин. */
        ArraySequence<Vertex> predecessors; /**< Массив для хранения предшествующих вершин для восстановления пути. */

        for (int i = 0; i < n; ++i) {
            distances.append(MAX_VALUE);
            predecessors.append(-1);
        }
        distances[source] = T(0);

        PriorityQueue<Vertex, T> pq; /**< Приоритетная очередь для выбора вершины с минимальным расстоянием. */
        pq.Enqueue(source, T(0));

        while (!pq.isEmpty()) {
            auto current = pq.Dequeue();
            Vertex u = current.first;
            T dist_u = current.second;

            if (dist_u > distances[u]) continue;

            auto neighbors = graph.getNeighbors(u);
            for (int i = 0; i < neighbors.getLength(); ++i) {
                Vertex v = neighbors[i].first;
                T originalWeight = neighbors[i].second;

                // Обновляем вес на основе времени
                T weight = updateWeight(originalWeight);

                if (distances[u] != MAX_VALUE &&
                    weight != MAX_VALUE &&
                    distances[u] + weight < distances[v]) {

                    distances[v] = distances[u] + weight;
                    predecessors[v] = u;
                    pq.Enqueue(v, distances[v]);
                }
            }
        }

        ArraySequence<Pair<T, Vertex>> result;
        for (int i = 0; i < n; ++i) {
            result.append(Pair<T, Vertex>(distances[i], predecessors[i]));
        }

        return result;
    }

    /**
     * @brief Восстанавливает путь от источника до целевой вершины.
     *
     * @param data Последовательность пар расстояний и предшественников для каждой вершины.
     * @param target Целевая вершина, до которой требуется восстановить путь.
     * @return ArraySequence<Vertex> Последовательность вершин, представляющая путь от источника до целевой вершины.
     *
     * @throws std::out_of_range Если целевая вершина находится вне допустимого диапазона.
     * @throws std::runtime_error Если путь не существует или путь некорректен.
     */
    static ArraySequence<Vertex> getPath(const ArraySequence<Pair<T, Vertex>>& data, Vertex target) {
        if (target < 0 || target >= data.getLength()) {
            throw std::out_of_range("Target vertex is out of range");
        }

        ArraySequence<Vertex> path;

        // Проверка, достижима ли целевая вершина
        if (data[target].first == std::numeric_limits<T>::max()) {
            throw std::runtime_error("No path exists to target vertex");
        }

        // Восстановление пути
        Vertex current = target;
        while (current != -1) {
            path.prepend(current);
            current = data[current].second;

            // Проверка на циклы
            if (path.getLength() > data.getLength()) {
                throw std::runtime_error("Invalid path: cycle detected");
            }
        }

        // Проверка корректности пути
        if (path.getLength() == 0) {
            throw std::runtime_error("Empty path generated");
        }

        // Проверка непрерывности пути
        for (int i = 0; i < path.getLength() - 1; ++i) {
            Vertex current = path.get(i);
            Vertex next = path.get(i + 1);
            if (data[next].second != current) {
                throw std::runtime_error("Invalid path: discontinuous vertices");
            }
        }

        return path;
    }
};

#endif // DYNAMIC_WEIGHT_SHORTEST_PATH_H