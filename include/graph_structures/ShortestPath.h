#ifndef SHORTEST_PATH_H
#define SHORTEST_PATH_H

#include "Graph.h"
#include "../sequence/ArraySequence.h"
#include "../sequence/PriorityQueue.h"
#include "../sequence/Pair.h"
#include <limits>
#include <functional>
#include <stdexcept>

template<typename T>
class ShortestPath {
public:
    typedef int Vertex;

    static ArraySequence<Pair<T, Vertex>> dijkstra(const Graph<T>& graph, Vertex source) {
        static_assert(std::is_arithmetic<T>::value, "Weight type must be numeric");

        int n = graph.getVertexCount();
        if (source < 0 || source >= n) {
            throw std::out_of_range("Source vertex is out of range");
        }

        // Initialize distances and predecessors
        ArraySequence<T> distances;
        ArraySequence<Vertex> predecessors;
        const T MAX_VALUE = std::numeric_limits<T>::max();

        for (int i = 0; i < n; ++i) {
            distances.append(MAX_VALUE);
            predecessors.append(-1);
        }
        distances[source] = T(0);

        // Priority queue for vertices
        PriorityQueue<Vertex, T> pq;
        pq.Enqueue(source, T(0));

        while (!pq.isEmpty()) {
            auto current = pq.Dequeue();
            Vertex u = current.first;
            T dist_u = current.second;

            // Skip if we already found a better path
            if (dist_u > distances[u]) continue;

            auto neighbors = graph.getNeighbors(u);
            for (int i = 0; i < neighbors.getLength(); ++i) {
                Vertex v = neighbors[i].first;
                T weight = neighbors[i].second;

                // Check for overflow and valid path
                if (distances[u] != MAX_VALUE &&
                    weight != MAX_VALUE &&
                    distances[u] + weight < distances[v]) {

                    distances[v] = distances[u] + weight;
                    predecessors[v] = u;
                    pq.Enqueue(v, distances[v]);
                }
            }
        }

        // Create result array
        ArraySequence<Pair<T, Vertex>> result;
        for (int i = 0; i < n; ++i) {
            result.append(Pair<T, Vertex>(distances[i], predecessors[i]));
        }

        return result;
    }

    static ArraySequence<Vertex> getPath(const ArraySequence<Pair<T, Vertex>>& data, Vertex target) {
        if (target < 0 || target >= data.getLength()) {
            throw std::out_of_range("Target vertex is out of range");
        }

        ArraySequence<Vertex> path;

        // Check if target is unreachable
        if (data[target].first == std::numeric_limits<T>::max()) {
            throw std::runtime_error("No path exists to target vertex");
        }

        // Reconstruct path
        Vertex current = target;
        while (current != -1) {
            path.prepend(current);
            current = data[current].second;

            // Check for cycles
            if (path.getLength() > data.getLength()) {
                throw std::runtime_error("Invalid path: cycle detected");
            }
        }

        // Validate path
        if (path.getLength() == 0) {
            throw std::runtime_error("Empty path generated");
        }

        // Verify path continuity
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