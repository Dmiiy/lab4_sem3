// TravelingSalesman.h
#ifndef TRAVELINGSALESMAN_H
#define TRAVELINGSALESMAN_H

#include "Graph.h"
#include "../data_structures/IDictionaryBinaryTree.h"
#include <limits>
#include <stdexcept>

// Предполагается, что Pair.h, Graph.h, ArraySequence.h, IDictionaryBinaryTree.h
// и другие необходимые заголовочные файлы правильно реализованы.

template<typename T>
class TravelingSalesman {
private:
    Graph<T> graph;
    ArraySequence<int> mandatoryVertices;
    int startVertex;
    int m; // Количество обязательных вершин
    IDictionaryBinaryTree<int, int> vertexToBit; // Соответствие вершины биту

    // Вспомогательная функция: рекурсивный DP для вычисления минимальной стоимости
    T tspUtil(int pos, int mask, ArraySequence<ArraySequence<T>>& dp, ArraySequence<ArraySequence<int>>& parent) const {
        // Базовый случай: все обязательные вершины посещены, возвращаемся в начальную точку
        if (mask == (1 << m) - 1) {
            if (graph.hasEdge(pos, startVertex)) {
                return graph.getEdgeWeight(pos, startVertex);
            } else {
                return std::numeric_limits<T>::max();
            }
        }

        // Если состояние уже вычислено, возвращаем сохраненное значение
        T cachedValue = dp[pos][mask];
        if (cachedValue != -1) {
            return cachedValue;
        }

        T minCost = std::numeric_limits<T>::max();
        int nextVertex = -1;

        int n = graph.getVertexCount();
        for (int vertex = 0; vertex < n; ++vertex) {
            // Если вершина обязательная и еще не посещена
            bool isMandatory = vertexToBit.ContainsKey(vertex);
            if (isMandatory) {
                int bit;
                try {
                    bit = vertexToBit.Get(vertex);
                } catch (const std::out_of_range&) {
                    // Вершина обязательная, но бит не найден — пропускаем
                    continue;
                }

                if (!(mask & (1 << bit)) && graph.hasEdge(pos, vertex)) {
                    T edgeWeight = graph.getEdgeWeight(pos, vertex);
                    T cost = tspUtil(vertex, mask | (1 << bit), dp, parent);
                    if (cost != std::numeric_limits<T>::max()) {
                        T totalCost = edgeWeight + cost;
                        if (totalCost < minCost) {
                            minCost = totalCost;
                            nextVertex = vertex;
                        }
                    }
                }
            }
                // Если вершина не обязательная, можно её использовать как транзитную
            else {
                if (graph.hasEdge(pos, vertex)) {
                    T edgeWeight = graph.getEdgeWeight(pos, vertex);
                    T cost = tspUtil(vertex, mask, dp, parent);
                    if (cost != std::numeric_limits<T>::max()) {
                        T totalCost = edgeWeight + cost;
                        if (totalCost < minCost) {
                            minCost = totalCost;
                            nextVertex = vertex;
                        }
                    }
                }
            }
        }

        dp[pos][mask] = minCost;
        parent[pos][mask] = nextVertex;
        return minCost;
    }

public:
    // Конструктор
    TravelingSalesman(const Graph<T>& g, const ArraySequence<int>& mandatory)
            : graph(g), mandatoryVertices(mandatory) {
        if (mandatoryVertices.getLength() == 0) {
            throw std::invalid_argument("No mandatory vertices provided.");
        }
        startVertex = mandatoryVertices.get(0); // Начальная точка — первая обязательная вершина
        m = mandatoryVertices.getLength();
        // Создаем соответствие для битовой маски
        for (int i = 0; i < m; ++i) {
            int vertex = mandatoryVertices.get(i);
            vertexToBit.Add(vertex, i);
        }
    }

    // Метод для решения задачи коммивояжера и возврата оптимального пути и его стоимости
    Pair<ArraySequence<int>, T> solve() const {
        int n = graph.getVertexCount();

        // Инициализируем таблицы dp и parent
        ArraySequence<ArraySequence<T>> dp;
        ArraySequence<ArraySequence<int>> parent;

        // Инициализация dp и parent с использованием appending
        for (int i = 0; i < n; ++i) {
            ArraySequence<T> dpRow;
            ArraySequence<int> parentRow;

            for (int j = 0; j < (1 << m); ++j) {
                dpRow.append(-1);      // Инициализируем всеми возможными масками как невычисленными
                parentRow.append(-1);  // Инициализируем parent как -1
            }

            dp.append(dpRow);
            parent.append(parentRow);
        }

        // Начальное состояние: находимся в startVertex, если он обязательный — помечаем как посещенный
        int initialMask = 0;
        if (vertexToBit.ContainsKey(startVertex)) {
            try {
                int bit = vertexToBit.Get(startVertex);
                initialMask |= (1 << bit);
            } catch (const std::out_of_range&) {
                // Вершина обязательная, но бит не найден — ничего не делаем
            }
        }

        T minCost = tspUtil(startVertex, initialMask, dp, parent);

        if (minCost == std::numeric_limits<T>::max()) {
            // Нет доступного пути
            ArraySequence<int> path;
            path.append(startVertex);
            return {path, minCost};
        }

        // Восстановление пути
        ArraySequence<int> path;
        int mask = initialMask;
        int pos = startVertex;
        path.append(pos);

        while (mask != (1 << m) - 1) {
            int nextPos = parent[pos][mask];
            if (nextPos == -1) break;
            path.append(nextPos);
            // Обновляем маску, если это обязательная вершина
            if (vertexToBit.ContainsKey(nextPos)) {
                try {
                    int bit = vertexToBit.Get(nextPos);
                    mask |= (1 << bit);
                } catch (const std::out_of_range&) {
                    // Вершина обязательная, но бит не найден — пропускаем
                }
            }
            pos = nextPos;
        }

        // Возвращаемся в начальную точку, если это возможно
        if (graph.hasEdge(pos, startVertex)) {
            path.append(startVertex);
        }

        return {path, minCost};
    }
};

#endif // TRAVELINGSALESMAN_H