// ShortestPath.h
#ifndef LAB4_SEM3_SHORTESTPATH_H
#define LAB4_SEM3_SHORTESTPATH_H

#include "Graph.h"
#include "../sequence/PriorityQueue.h"

#include <vector>
#include <limits>
#include <utility>

// Типы для удобства
typedef int Vertex;
typedef double Weight;

// Класс для поиска кратчайших путей с использованием алгоритма Дейкстры
template <typename T>
class ShortestPath {
public:
    // Метод Дейкстры для поиска кратчайшего пути от источника к всем другим вершинам
    static Pair<ArraySequence<Weight>, ArraySequence<Vertex>> Dijkstra(const Graph<Weight>& graph, Vertex source) {
        int n = graph.getVertexCount();
        ArraySequence<Weight> distances;
        ArraySequence<Vertex> predecessors;

        // Обработка пустого графа
        if(n == 0){
            return {ArraySequence<Weight>(), ArraySequence<Vertex>()};
        }

        // Проверка валидности источника
        if(source < 0 || source >= n){
            throw std::out_of_range("Source vertex out of range");
        }

        // Инициализация расстояний и предшественников
        for (int i = 0; i < n; ++i) {
            distances.append(std::numeric_limits<Weight>::infinity());
            predecessors.append(-1);
        }

        distances[source] = 0;

        // Приоритетная очередь: пара (vertex, distance)
        PriorityQueue<Vertex, Weight> pq;
        pq.Enqueue(source, 0.0);

        while (!pq.isEmpty()) {
            Pair<Vertex, Weight> current = pq.Dequeue();
            Vertex u = current.first;
            Weight dist_u = current.second;

            // Если извлеченное расстояние больше текущего зарегистрированного, пропускаем
            if (dist_u > distances[u]) continue;

            // Обработка соседей
            ArraySequence<Pair<int, Weight>> neighbors = graph.getNeighbors(u);
            for (int i = 0; i < neighbors.getLength(); ++i) {
                Vertex v = neighbors[i].first;
                Weight weight = neighbors[i].second;

                if (distances[u] + weight < distances[v]) {
                    distances[v] = distances[u] + weight;
                    predecessors[v] = u;
                    pq.Enqueue(v, distances[v]);
                }
            }
        }

        return {distances, predecessors};
    }

    // Метод для восстановления пути от источника к целевой вершине
    static ArraySequence<Vertex> GetPath(const ArraySequence<Vertex>& predecessors, Vertex target) {
        ArraySequence<Vertex> path;
        Vertex current = target;

        while (current != -1) {
            path.prepend(current);
            current = predecessors[current];
        }

        return path;
    }
};

#endif //LAB4_SEM3_SHORTESTPATH_H