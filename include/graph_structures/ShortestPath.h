#ifndef SHORTEST_PATH_H
#define SHORTEST_PATH_H

#include "Graph.h"
#include "../sequence/ArraySequence.h"
#include "../sequence/PriorityQueue.h"
#include "../sequence/Pair.h"
#include <limits>
#include <functional>
#include <stdexcept>

/**
 * @brief Класс для поиска кратчайших путей в графе с помощью алгоритма Дейкстры.
 *
 * Класс `ShortestPath` предоставляет методы для выполнения алгоритма Дейкстры
 * и восстановления кратчайшего пути от одного узла до другого в взвешенном графе.
 */
template<typename T>
class ShortestPath {
public:
    typedef int Vertex; /**< Тип для обозначения вершины графа. */

    /**
     * @brief Реализация алгоритма Дейкстры для поиска кратчайших путей от источника до всех вершин графа.
     *
     * Метод `dijkstra` вычисляет кратчайшие расстояния от заданного источника до всех других вершин
     * графа с использованием алгоритма Дейкстры. Он также сохраняет информацию о предшествующих
     * вершинах для восстановления путей.
     *
     * @param graph Ссылка на граф, в котором выполняется поиск.
     * @param source Исходная вершина (источник).
     * @return ArraySequence<Pair<T, Vertex>> Последовательность пар, где каждая пара содержит
     *                                            расстояние от источника и предшествующую вершину для каждой вершины графа.
     *
     * @throws std::out_of_range Если исходная вершина находится вне допустимого диапазона.
     */
    static ArraySequence<Pair<T, Vertex>> dijkstra(const Graph<T>& graph, Vertex source) {
        static_assert(std::is_arithmetic<T>::value, "Weight type must be numeric");

        int n = graph.getVertexCount();
        if (source < 0 || source >= n) {
            throw std::out_of_range("Source vertex is out of range");
        }

        // Инициализация массивов расстояний и предшественников
        ArraySequence<T> distances;
        ArraySequence<Vertex> predecessors;
        const T MAX_VALUE = std::numeric_limits<T>::max();

        for (int i = 0; i < n; ++i) {
            distances.append(MAX_VALUE);
            predecessors.append(-1);
        }
        distances[source] = T(0);

        // Приоритетная очередь для вершин
        PriorityQueue<Vertex, T> pq;
        pq.Enqueue(source, T(0));

        while (!pq.isEmpty()) {
            auto current = pq.Dequeue();
            Vertex u = current.first;
            T dist_u = current.second;

            // Пропускаем, если уже найден более короткий путь
            if (dist_u > distances[u]) continue;

            auto neighbors = graph.getNeighbors(u);
            for (int i = 0; i < neighbors.getLength(); ++i) {
                Vertex v = neighbors[i].first;
                T weight = neighbors[i].second;

                // Проверка на переполнение и корректность пути
                if (distances[u] != MAX_VALUE &&
                    weight != MAX_VALUE &&
                    distances[u] + weight < distances[v]) {

                    distances[v] = distances[u] + weight;
                    predecessors[v] = u;
                    pq.Enqueue(v, distances[v]);
                }
            }
        }

        // Создание результирующего массива
        ArraySequence<Pair<T, Vertex>> result;
        for (int i = 0; i < n; ++i) {
            result.append(Pair<T, Vertex>(distances[i], predecessors[i]));
        }

        return result;
    }

    /**
     * @brief Восстанавливает путь от источника до целевой вершины.
     *
     * Метод `getPath` выполняет восстановление пути от исходной вершины до заданной целевой вершины
     * с использованием информации о предшественниках, полученной из алгоритма Дейкстры.
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

#endif // SHORTEST_PATH_H