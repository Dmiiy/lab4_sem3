// GraphGenerator.h
#ifndef LAB4_SEM3_GRAPHGENERATOR_H
#define LAB4_SEM3_GRAPHGENERATOR_H

#include "Graph.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdexcept>
#include <algorithm>            // Для std::swap
#include <random>

class GraphGenerator {
public:
    /**
     * @brief Генерация полного графа.
     *
     * Полный граф - это граф, в котором каждая пара вершин соединена ребром.
     *
     * @param vertices Количество вершин в графе.
     * @param maxWeight Максимальный вес ребра (по умолчанию 100).
     * @return Сгенерированный полный граф типа Graph<int>.
     */
    static Graph<int> generateCompleteGraph(int vertices, int maxWeight = 100) {
        Graph<int> graph(vertices);
        for (int i = 0; i < vertices; ++i) {
            for (int j = i + 1; j < vertices; ++j) {
                int weight = rand() % maxWeight + 1; // Вес в диапазоне [1, maxWeight]
                graph.addUndirectedEdge(i, j, weight);
            }
        }
        return graph;
    }

    /**
     * @brief Генерация разреженного графа (графа в виде дерева).
     *
     * Разреженный граф - это граф, содержащий минимальное количество ребер для связности.
     *
     * @param vertices Количество вершин в графе.
     * @param maxWeight Максимальный вес ребра (по умолчанию 100).
     * @return Сгенерированный разреженный граф типа Graph<int>.
     */
    static Graph<int> generateSparseGraph(int vertices, int maxWeight = 100) {
        Graph<int> graph(vertices);
        for (int i = 1; i < vertices; ++i) {
            int parent = rand() % i; // Соединяем новую вершину с уже существующей
            int weight = rand() % maxWeight + 1;
            graph.addUndirectedEdge(i, parent, weight);
        }
        return graph;
    }

    /**
     * @brief Генерация случайного графа с заданной плотностью.
     *
     * @param vertices Количество вершин в графе.
     * @param density Плотность графа в диапазоне [0.0, 1.0].
     * @param maxWeight Максимальный вес ребра (по умолчанию 100).
     * @return Сгенерированный случайный граф типа Graph<int>.
     * @throws std::invalid_argument Если плотность не находится в диапазоне [0.0, 1.0].
     */
    static Graph<int> generateRandomGraph(int vertices, double density, int maxWeight = 100) {
        if (density < 0.0 || density > 1.0) {
            throw std::invalid_argument("Density must be between 0.0 and 1.0");
        }

        Graph<int> graph(vertices);
        int maxEdges = vertices * (vertices - 1) / 2; // Максимальное количество ребер
        int targetEdges = static_cast<int>(density * maxEdges); // Желаемое количество ребер

        // Создание последовательности всех возможных ребер
        ArraySequence<Pair<int, int>> allEdges;
        for (int i = 0; i < vertices; ++i) {
            for (int j = i + 1; j < vertices; ++j) {
                allEdges.append(Pair(i, j));
            }
        }

        // Перемешивание последовательности ребер с использованием алгоритма Фишера-Йейтса
        shuffleArraySequence(allEdges);

        // Добавление первых targetEdges ребер в граф
        for (int i = 0; i < targetEdges; ++i) {
            int u = allEdges[i].first;
            int v = allEdges[i].second;
            int weight = rand() % maxWeight + 1;
            graph.addUndirectedEdge(u, v, weight);
        }

        return graph;
    }

private:
    /**
     * @brief Перемешивает элементы в ArraySequence с использованием алгоритма Фишера-Йейтса.
     *
     * @tparam T Тип элементов в ArraySequence.
     * @param seq Ссылка на ArraySequence, который нужно перемешать.
     */
    template <typename T>
    static void shuffleArraySequence(ArraySequence<T>& seq) {
        std::random_device rd;
        std::mt19937 gen(rd());

        for (int i = seq.getLength() - 1; i > 0; --i) {
            std::uniform_int_distribution<> dis(0, i);
            int j = dis(gen);
            Swap(seq, i, j);
        }
    }

    /**
     * @brief Обменивает местами два элемента в ArraySequence.
     *
     * @tparam T Тип элементов в ArraySequence.
     * @param seq Ссылка на ArraySequence, содержащий элементы.
     * @param i Индекс первого элемента.
     * @param j Индекс второго элемента.
     */
    template <typename T>
    static void Swap(ArraySequence<T>& seq, int i, int j) {
        T temp = seq[i];
        seq[i] = seq[j];
        seq[j] = temp;
    }
};

#endif //LAB4_SEM3_GRAPHGENERATOR_H