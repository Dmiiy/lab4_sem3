#ifndef LAB4_SEM3_GRAPHGENERATOR_H
#define LAB4_SEM3_GRAPHGENERATOR_H

#include "Graph.h"
#include <algorithm>
#include <random>
#include <stdexcept>

class GraphGenerator {
public:

    static Graph<int> generateCompleteGraph(int vertices, int maxWeight = 100) {
        if (vertices <= 0) {
            throw std::invalid_argument("Number of vertices must be positive.");
        }

        Graph<int> graph(vertices);
        std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<> dis(1, maxWeight);

        for (int i = 0; i < vertices; ++i) {
            for (int j = i + 1; j < vertices; ++j) {
                int weight = dis(gen); // Вес в диапазоне [1, maxWeight]
                graph.addUndirectedEdge(i, j, weight);
            }
        }
        return graph;
    }

    static Graph<int> generateSparseGraph(int vertices, int maxWeight = 100) {
        if (vertices <= 0) {
            throw std::invalid_argument("Number of vertices must be positive.");
        }

        Graph<int> graph(vertices);
        std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<> dis(1, maxWeight);

        for (int i = 1; i < vertices; ++i) {
            std::uniform_int_distribution<> parent_dis(0, i - 1);
            int parent = parent_dis(gen);
            int weight = dis(gen);
            graph.addUndirectedEdge(i, parent, weight);
        }

        return graph;
    }

     // Генерация случайного графа с заданной плотностью [0.0, 1.0]
    static Graph<int> generateRandomGraph(int vertices, double density, int maxWeight = 100) {
        if (vertices <= 0) {
            throw std::invalid_argument("Number of vertices must be positive.");
        }
        if (density < 0.0 || density > 1.0) {
            throw std::invalid_argument("Density must be between 0.0 and 1.0.");
        }

        Graph<int> graph(vertices);
        std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<> dis_weight(1, maxWeight);

        // Создание списка всех возможных неориентированных ребер
        ArraySequence<Pair<int, int>> allEdges;
        for (int i = 0; i < vertices; ++i) {
            for (int j = i + 1; j < vertices; ++j) {
                allEdges.append({i, j});
            }
        }

        // Перемешивание ребер
        shuffleArraySequence(allEdges, gen);

        // Определение количества ребер для достижения заданной плотности
        int maxEdges = vertices * (vertices - 1) / 2;
        int targetEdges = static_cast<int>(density * maxEdges);

        // Добавление ребер в граф
        for (int i = 0; i < targetEdges && i < allEdges.getLength(); ++i) {
            int u = allEdges.get(i).first;
            int v = allEdges.get(i).second;
            int weight = dis_weight(gen);
            graph.addUndirectedEdge(u, v, weight);
        }

        return graph;
    }

private:

     //алгоритм Фишера-Йейтса
    template <typename T>
    static void shuffleArraySequence(ArraySequence<T>& seq, std::mt19937& gen) {
        for (int i = seq.getLength() - 1; i > 0; --i) {
            std::uniform_int_distribution<> dis(0, i);
            int j = dis(gen);
            Swap(seq, i, j);
        }
    }

    template <typename T>
    static void Swap(ArraySequence<T>& seq, int i, int j) {
        T temp = seq[i];
        seq[i] = seq[j];
        seq[j] = temp;
    }
};

#endif // LAB4_SEM3_GRAPHGENERATOR_H