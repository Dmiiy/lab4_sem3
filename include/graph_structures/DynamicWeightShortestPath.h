// DynamicWeightShortestPath.h
#ifndef DYNAMIC_WEIGHT_SHORTEST_PATH_H
#define DYNAMIC_WEIGHT_SHORTEST_PATH_H

#include "Graph.h"
#include "../sequence/ArraySequence.h"
#include "../sequence/PriorityQueue.h"
#include "../sequence/Pair.h"
#include <limits>
#include <functional>
#include <stdexcept>

class DynamicWeightShortestPath {
public:
    typedef int Vertex;
    typedef double Weight;

    // Функция для обновления весов по времени
    std::function<Weight(Weight, double)> updateWeightFunction;

    DynamicWeightShortestPath(std::function<Weight(Weight, double)> updateFunc)
            : updateWeightFunction(updateFunc) {}

    Pair<ArraySequence<Weight>, ArraySequence<Vertex>> dijkstra(const Graph<Weight>& graph, Vertex source, double time) const {
        int n = graph.getVertexCount();
        ArraySequence<Weight> distances;
        ArraySequence<Vertex> predecessors;

        for (int i = 0; i < n; ++i) {
            distances.append(std::numeric_limits<Weight>::infinity());
            predecessors.append(-1);
        }
        if (source < 0 || source >= n) {
            throw std::out_of_range("Source vertex out of range");
        }

        distances[source] = 0;

        // Приоритетная очередь: пара (distance, vertex)
        PriorityQueue<Vertex, Weight> pq;
        pq.Enqueue(source, 0.0);

        while (!pq.isEmpty()) {
            Pair<Vertex, Weight> current = pq.Dequeue();
            Vertex u = current.first;
            Weight dist_u = current.second;

            if (dist_u > distances[u]) continue;

            auto neighbors = graph.getNeighbors(u);
            for (int i = 0; i < neighbors.getLength(); ++i) {
                Vertex v = neighbors[i].first;
                Weight originalWeight = neighbors[i].second;

                // Обновляем вес ребра по функции времени
                Weight weight = updateWeightFunction(originalWeight, time);

                if (distances[u] + weight < distances[v]) {
                    distances[v] = distances[u] + weight;
                    predecessors[v] = u;
                    pq.Enqueue(v, distances[v]);
                }
            }
        }

        return {distances, predecessors};
    }

    // Восстановление пути от источника к целевой вершине
    static ArraySequence<Vertex> getPath(const ArraySequence<Vertex>& predecessors, Vertex target) {
        ArraySequence<Vertex> path;
        Vertex current = target;

        while (current != -1) {
            path.prepend(current);
            current = predecessors[current];
        }

        return path;
    }
};

#endif // DYNAMIC_WEIGHT_SHORTEST_PATH_H