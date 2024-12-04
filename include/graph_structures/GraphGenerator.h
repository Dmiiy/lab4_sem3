#ifndef LAB4_SEM3_GRAPHGENERATOR_H
#define LAB4_SEM3_GRAPHGENERATOR_H

#include "UndirectedGraph.h"
#include "DirectedGraph.h"
#include "Graph.h"
#include <functional>
#include <stdexcept>
#include <random>
#include "../sequence/ArraySequence.h"
#include "../sequence/Pair.h"

/**
 * @brief Класс для генерации различных типов графов.
 *
 * Класс `GraphGenerator` предоставляет статические методы для генерации
 * неориентированных и ориентированных графов различных типов, таких как полные, разреженные, случайные, циклические и деревья.
 */
class GraphGenerator {
public:
    /**
     * @brief Перечисление типов графов для генерации.
     */
    enum GraphType {
        COMPLETE, /**< Полный граф (каждая пара вершин соединена ребром). */
        SPARSE,   /**< Разрежённый граф (минимальный набор рёбер для связности). */
        RANDOM,   /**< Случайный граф с заданной плотностью. */
        CYCLE,    /**< Циклический граф (каждая вершина соединена с двумя другими, образуя цикл). */
        TREE      /**< Дерево (ациклический связный граф). */
    };

    /**
     * @brief Генерация неориентированного графа.
     *
     * Метод `generateUndirectedGraph` генерирует неориентированный граф заданного типа,
     * числа вершин и других параметров.
     *
     * @param type Тип графа, который необходимо сгенерировать.
     * @param vertices Число вершин в графе.
     * @param density Плотность графа (применяется для случайного графа).
     * @param maxWeight Максимальный вес рёбер.
     * @param k Параметр, используемый для некоторых типов графов (например, k-регулярный).
     * @return UndirectedGraph<int> Сгенерированный неориентированный граф.
     *
     * @throws std::invalid_argument Если заданное число вершин неположительно или тип графа неизвестен.
     */
    static UndirectedGraph<int> generateUndirectedGraph(GraphType type, int vertices, double density = 0.5, int maxWeight = 100, int k = 3) {
        if (vertices <= 0) {
            throw std::invalid_argument("Number of vertices must be positive.");
        }

        switch (type) {
            case COMPLETE:
                return generateCompleteGraph(vertices, maxWeight);
            case SPARSE:
                return generateSparseGraph(vertices, maxWeight);
            case RANDOM:
                return generateRandomGraph(vertices, density, maxWeight);
            case CYCLE:
                return generateCycleGraph(vertices, maxWeight);
            case TREE:
                return generateTree(vertices, maxWeight);
            default:
                throw std::invalid_argument("Unknown graph type.");
        }
    }

    /**
     * @brief Генерация ориентированного графа.
     *
     * Метод `generateDirectedGraph` генерирует ориентированный граф заданного типа,
     * числа вершин и других параметров.
     *
     * @param type Тип графа, который необходимо сгенерировать.
     * @param vertices Число вершин в графе.
     * @param density Плотность графа (применяется для случайного графа).
     * @param maxWeight Максимальный вес рёбер.
     * @param k Параметр, используемый для некоторых типов графов (например, k-регулярный).
     * @return DirectedGraph<int> Сгенерированный ориентированный граф.
     *
     * @throws std::invalid_argument Если заданное число вершин неположительно или тип графа неизвестен.
     */
    static DirectedGraph<int> generateDirectedGraph(GraphType type, int vertices, double density = 0.5, int maxWeight = 100, int k = 3) {
        if (vertices <= 0) {
            throw std::invalid_argument("Number of vertices must be positive.");
        }

        switch (type) {
            case COMPLETE:
                return generateCompleteDirectedGraph(vertices, maxWeight);
            case SPARSE:
                return generateSparseDirectedGraph(vertices, maxWeight);
            case RANDOM:
                return generateRandomDirectedGraph(vertices, density, maxWeight);
            case CYCLE:
                return generateCycleDirectedGraph(vertices, maxWeight);
            case TREE:
                return generateTreeDirected(vertices, maxWeight);
            default:
                throw std::invalid_argument("Unknown graph type.");
        }
    }

private:
    /**
     * @brief Генерация полного неориентированного графа.
     *
     * В полном графе каждая пара различных вершин соединена ребром.
     *
     * @param vertices Число вершин в графе.
     * @param maxWeight Максимальный вес рёбер.
     * @return UndirectedGraph<int> Сгенерированный полный неориентированный граф.
     */
    static UndirectedGraph<int> generateCompleteGraph(int vertices, int maxWeight) {
        UndirectedGraph<int> graph(vertices); /**< Создаём неориентированный граф с заданным числом вершин. */
        std::mt19937 gen(std::random_device{}()); /**< Генератор случайных чисел. */
        std::uniform_int_distribution<> dis(1, maxWeight); /**< Равномерное распределение для весов рёбер. */

        // Проходим по всем парам вершин и добавляем ребра между ними
        for (int i = 0; i < vertices; ++i) {
            for (int j = i + 1; j < vertices; ++j) {
                int weight = dis(gen); /**< Генерируем случайный вес для ребра. */
                graph.addEdge(i, j, weight); /**< Добавляем ребро между вершинами i и j. */
            }
        }
        return graph; /**< Возвращаем сгенерированный граф. */
    }

    /**
     * @brief Генерация разрежённого неориентированного графа.
     *
     * Разрежённый граф гарантирует минимальный набор рёбер для связности,
     * обычно являясь деревом или близким к дереву.
     *
     * @param vertices Число вершин в графе.
     * @param maxWeight Максимальный вес рёбер.
     * @return UndirectedGraph<int> Сгенерированный разрежённый неориентированный граф.
     */
    static UndirectedGraph<int> generateSparseGraph(int vertices, int maxWeight) {
        UndirectedGraph<int> graph(vertices); /**< Создаём неориентированный граф с заданным числом вершин. */
        std::mt19937 gen(std::random_device{}()); /**< Генератор случайных чисел. */
        std::uniform_int_distribution<> dis(1, maxWeight); /**< Равномерное распределение для весов рёбер. */

        // Для обеспечения связности добавляем минимальное число рёбер (vertices - 1)
        for (int i = 1; i < vertices; ++i) {
            std::uniform_int_distribution<> parent_dis(0, i - 1); /**< Распределение для выбора родительской вершины. */
            int parent = parent_dis(gen); /**< Выбираем случайного родителя для текущей вершины. */
            int weight = dis(gen); /**< Генерируем случайный вес для ребра. */
            graph.addEdge(i, parent, weight); /**< Добавляем ребро между вершиной i и её родителем. */
        }

        return graph; /**< Возвращаем сгенерированный граф. */
    }

    /**
     * @brief Генерация случайного неориентированного графа с заданной плотностью.
     *
     * Плотность графа определяет вероятность наличия ребра между любой парой вершин.
     *
     * @param vertices Число вершин в графе.
     * @param density Плотность графа (значение от 0 до 1).
     * @param maxWeight Максимальный вес рёбер.
     * @return UndirectedGraph<int> Сгенерированный случайный неориентированный граф.
     */
    static UndirectedGraph<int> generateRandomGraph(int vertices, double density, int maxWeight) {
        UndirectedGraph<int> graph(vertices); /**< Создаём неориентированный граф. */
        std::mt19937 gen(std::random_device{}()); /**< Генератор случайных чисел. */
        std::uniform_int_distribution<> dis_weight(1, maxWeight); /**< Равномерное распределение для весов рёбер. */

        ArraySequence<Pair<int, int>> allEdges; /**< Список всех возможных рёбер. */

        // Генерируем список всех возможных рёбер без повторений
        for (int i = 0; i < vertices; ++i) {
            for (int j = i + 1; j < vertices; ++j) {
                allEdges.append({i, j});
            }
        }

        // Перемешиваем список рёбер для случайного выбора
        shuffleArraySequence(allEdges, gen);

        int maxEdges = vertices * (vertices - 1) / 2; /**< Максимальное возможное число рёбер в неориентированном графе. */
        int targetEdges = static_cast<int>(density * maxEdges); /**< Целевое число рёбер на основе плотности. */

        // Добавляем ребра до достижения целевого числа рёбер
        for (int i = 0; i < targetEdges && i < allEdges.getLength(); ++i) {
            int u = allEdges.get(i).first; /**< Первая вершина ребра. */
            int v = allEdges.get(i).second; /**< Вторая вершина ребра. */
            int weight = dis_weight(gen);   /**< Генерируем случайный вес для ребра. */
            graph.addEdge(u, v, weight);    /**< Добавляем ребро в граф. */
        }

        return graph; /**< Возвращаем сгенерированный граф. */
    }

    /**
     * @brief Генерация циклического неориентированного графа.
     *
     * Циклический граф - это граф, в котором вершины соединены в цикл.
     *
     * @param vertices Число вершин в графе (должно быть не меньше 3).
     * @param maxWeight Максимальный вес рёбер.
     * @return UndirectedGraph<int> Сгенерированный циклический неориентированный граф.
     *
     * @throws std::invalid_argument Если число вершин меньше 3.
     */
    static UndirectedGraph<int> generateCycleGraph(int vertices, int maxWeight) {
        if (vertices < 3) {
            throw std::invalid_argument("Cycle graph must have at least 3 vertices.");
        }

        UndirectedGraph<int> graph(vertices); /**< Создаём неориентированный граф с заданным числом вершин. */
        std::mt19937 gen(std::random_device{}()); /**< Генератор случайных чисел. */
        std::uniform_int_distribution<> dis(1, maxWeight); /**< Равномерное распределение для весов рёбер. */

        // Добавляем ребра, формирующие цикл
        for (int i = 0; i < vertices; ++i) {
            int next = (i + 1) % vertices; /**< Вычисляем следующую вершину в цикле. */
            int weight = dis(gen);        /**< Генерируем случайный вес для ребра. */
            graph.addEdge(i, next, weight); /**< Добавляем ребро между текущей и следующей вершиной. */
        }

        return graph; /**< Возвращаем сгенерированный граф. */
    }

    /**
     * @brief Генерация дерева (разрежённого связного графа) с использованием минимального набора рёбер.
     *
     * Дерево - это ациклический связный граф, в котором есть ровно (vertices - 1) рёбер.
     *
     * @param vertices Число вершин в дереве.
     * @param maxWeight Максимальный вес рёбер.
     * @return UndirectedGraph<int> Сгенерированное дерево.
     */
    static UndirectedGraph<int> generateTree(int vertices, int maxWeight) {
        // Используем генерацию разрежённого графа, который гарантирует связность
        return generateSparseGraph(vertices, maxWeight);
    }

    /**
     * @brief Генерация полного ориентированного графа.
     *
     * В полном ориентированном графе каждая пара различных вершин соединена двумя ребрами в обоих направлениях.
     *
     * @param vertices Число вершин в графе.
     * @param maxWeight Максимальный вес рёбер.
     * @return DirectedGraph<int> Сгенерированный полный ориентированный граф.
     */
    static DirectedGraph<int> generateCompleteDirectedGraph(int vertices, int maxWeight) {
        DirectedGraph<int> graph(vertices); /**< Создаём ориентированный граф с заданным числом вершин. */
        std::mt19937 gen(std::random_device{}()); /**< Генератор случайных чисел. */
        std::uniform_int_distribution<> dis(1, maxWeight); /**< Равномерное распределение для весов рёбер. */

        // Добавляем ребра во всех возможных направлениях между разными вершинами
        for (int i = 0; i < vertices; ++i) {
            for (int j = 0; j < vertices; ++j) {
                if (i != j) {
                    int weight = dis(gen); /**< Генерируем случайный вес для ребра. */
                    graph.addEdge(i, j, weight); /**< Добавляем ребро из вершины i в вершину j. */
                }
            }
        }
        return graph; /**< Возвращаем сгенерированный граф. */
    }

    /**
     * @brief Генерация разрежённого ориентированного графа.
     *
     * Разрежённый ориентированный граф гарантирует минимальный набор рёбер для связности,
     * обычно являясь деревом или близким к дереву.
     *
     * @param vertices Число вершин в графе.
     * @param maxWeight Максимальный вес рёбер.
     * @return DirectedGraph<int> Сгенерированный разрежённый ориентированный граф.
     */
    static DirectedGraph<int> generateSparseDirectedGraph(int vertices, int maxWeight) {
        DirectedGraph<int> graph(vertices); /**< Создаём ориентированный граф с заданным числом вершин. */
        std::mt19937 gen(std::random_device{}()); /**< Генератор случайных чисел. */
        std::uniform_int_distribution<> dis(1, maxWeight); /**< Равномерное распределение для весов рёбер. */

        // Добавляем минимальное число рёбер для обеспечения связности (vertices - 1)
        for (int i = 1; i < vertices; ++i) {
            std::uniform_int_distribution<> parent_dis(0, i - 1); /**< Распределение для выбора родительской вершины. */
            int parent = parent_dis(gen); /**< Выбираем случайного родителя для текущей вершины. */
            int weight = dis(gen); /**< Генерируем случайный вес для ребра. */
            graph.addEdge(parent, i, weight); /**< Добавляем ребро из родителя в текущую вершину. */
        }

        return graph; /**< Возвращаем сгенерированный граф. */
    }

    /**
     * @brief Генерация случайного ориентированного графа с заданной плотностью.
     *
     * Плотность графа определяет вероятность наличия ребра между любой парой вершин.
     *
     * @param vertices Число вершин в графе.
     * @param density Плотность графа (значение от 0 до 1).
     * @param maxWeight Максимальный вес рёбер.
     * @return DirectedGraph<int> Сгенерированный случайный ориентированный граф.
     */
    static DirectedGraph<int> generateRandomDirectedGraph(int vertices, double density, int maxWeight) {
        DirectedGraph<int> graph(vertices); /**< Создаём ориентированный граф. */
        std::mt19937 gen(std::random_device{}()); /**< Генератор случайных чисел. */
        std::uniform_int_distribution<> dis_weight(1, maxWeight); /**< Равномерное распределение для весов рёбер. */

        ArraySequence<Pair<int, int>> allEdges; /**< Список всех возможных направленных рёбер. */

        // Генерируем список всех возможных направленных рёбер без повторений, исключая петли
        for (int i = 0; i < vertices; ++i) {
            for (int j = 0; j < vertices; ++j) {
                if (i != j) {
                    allEdges.append({i, j});
                }
            }
        }

        // Перемешиваем список рёбер для случайного выбора
        shuffleArraySequence(allEdges, gen);

        int maxEdges = vertices * (vertices - 1); /**< Максимальное возможное число ориентированных рёбер. */
        int targetEdges = static_cast<int>(density * maxEdges); /**< Целевое число рёбер на основе плотности. */

        // Добавляем ребра до достижения целевого числа рёбер
        for (int i = 0; i < targetEdges && i < allEdges.getLength(); ++i) {
            int u = allEdges.get(i).first; /**< Исходная вершина ребра. */
            int v = allEdges.get(i).second; /**< Конечная вершина ребра. */
            int weight = dis_weight(gen);   /**< Генерируем случайный вес для ребра. */
            graph.addEdge(u, v, weight);    /**< Добавляем ребро в граф. */
        }

        return graph; /**< Возвращаем сгенерированный граф. */
    }

    /**
     * @brief Генерация циклического ориентированного графа.
     *
     * Циклический ориентированный граф - это ориентированный граф, в котором вершины соединены
     * в цикл.
     *
     * @param vertices Число вершин в графе (должно быть не меньше 3).
     * @param maxWeight Максимальный вес рёбер.
     * @return DirectedGraph<int> Сгенерированный циклический ориентированный граф.
     *
     * @throws std::invalid_argument Если число вершин меньше 3.
     */
    static DirectedGraph<int> generateCycleDirectedGraph(int vertices, int maxWeight) {
        if (vertices < 3) {
            throw std::invalid_argument("Cycle graph must have at least 3 vertices.");
        }

        DirectedGraph<int> graph(vertices); /**< Создаём ориентированный граф с заданным числом вершин. */
        std::mt19937 gen(std::random_device{}()); /**< Генератор случайных чисел. */
        std::uniform_int_distribution<> dis(1, maxWeight); /**< Равномерное распределение для весов рёбер. */

        // Добавляем ребра, формирующие ориентированный цикл
        for (int i = 0; i < vertices; ++i) {
            int next = (i + 1) % vertices; /**< Вычисляем следующую вершину в цикле. */
            int weight = dis(gen);        /**< Генерируем случайный вес для ребра. */
            graph.addEdge(i, next, weight); /**< Добавляем ребро из текущей вершины в следующую. */
        }

        return graph; /**< Возвращаем сгенерированный граф. */
    }

    /**
     * @brief Генерация ориентированного дерева.
     *
     * Ориентированное дерево - это ациклический связный ориентированный граф,
     * где каждая вершина, кроме корня, имеет ровно одного предшественника.
     *
     * @param vertices Число вершин в дереве.
     * @param maxWeight Максимальный вес рёбер.
     * @return DirectedGraph<int> Сгенерированное ориентированное дерево.
     */
    static DirectedGraph<int> generateTreeDirected(int vertices, int maxWeight) {
        DirectedGraph<int> graph(vertices); /**< Создаём ориентированный граф с заданным числом вершин. */
        std::mt19937 gen(std::random_device{}()); /**< Генератор случайных чисел. */
        std::uniform_int_distribution<> dis(1, maxWeight); /**< Равномерное распределение для весов рёбер. */

        // Добавляем минимальное число рёбер для обеспечения связности (vertices - 1)
        for (int i = 1; i < vertices; ++i) {
            std::uniform_int_distribution<> parent_dis(0, i - 1); /**< Распределение для выбора родительской вершины. */
            int parent = parent_dis(gen); /**< Выбираем случайного родителя для текущей вершины. */
            int weight = dis(gen); /**< Генерируем случайный вес для ребра. */
            graph.addEdge(parent, i, weight); /**< Добавляем ребро из родителя в текущую вершину. */
        }

        return graph; /**< Возвращаем сгенерированный граф. */
    }

    /**
     * @brief Генерация k-регулярного ориентированного графа.
     *
     * k-регулярный ориентированный граф - это граф, в котором каждая вершина имеет
     * ровно k исходящих и k входящих рёбер.
     *
     * @param vertices Число вершин в графе.
     * @param k Степень регулярности (количество исходящих и входящих рёбер).
     * @param maxWeight Максимальный вес рёбер.
     * @return DirectedGraph<int> Сгенерированный k-регулярный ориентированный граф.
     *
     * @throws std::invalid_argument Если k не меньше числа вершин или vertices * k нечётно.
     */
    static DirectedGraph<int> generateKRegularDirectedGraph(int vertices, int k, int maxWeight) {
        if (k >= vertices) {
            throw std::invalid_argument("k must be less than the number of vertices.");
        }
        if ((vertices * k) % 2 != 0) {
            throw std::invalid_argument("vertices * k must be even for a k-regular graph.");
        }

        DirectedGraph<int> graph(vertices); /**< Создаём ориентированный граф с заданным числом вершин. */
        ArraySequence<int> outDegrees(vertices, 0); /**< Массив для отслеживания исходящих степеней вершин. */
        ArraySequence<int> inDegrees(vertices, 0); /**< Массив для отслеживания входящих степеней вершин. */

        std::mt19937 gen(std::random_device{}()); /**< Генератор случайных чисел. */
        std::uniform_int_distribution<> dis(0, vertices - 1); /**< Равномерное распределение для выбора соседей. */
        std::uniform_int_distribution<> dis_weight(1, maxWeight); /**< Равномерное распределение для весов рёбер. */

        /**
         * @brief Основной цикл генерации k-регулярного графа.
         *
         * Повторяет процесс добавления рёбер до тех пор, пока все вершины
         * не достигнут необходимой степени.
         */
        while (true) {
            // Сбрасываем граф и степени вершин
            graph = DirectedGraph<int>(vertices);
            outDegrees = ArraySequence<int>(vertices, 0);
            inDegrees = ArraySequence<int>(vertices, 0);

            bool success = true; /**< Флаг успешной генерации графа. */

            // Проходим по каждой вершине и добавляем необходимые рёбра
            for (int i = 0; i < vertices; ++i) {
                for (int j = 0; j < k; ++j) {
                    int neighbor = dis(gen); /**< Выбираем случайного соседа. */

                    // Проверка, что не создаём петлю и не добавляем повторяющиеся ребра
                    if (neighbor != i && !graph.hasEdge(i, neighbor) && outDegrees.get(i) < k && inDegrees.get(neighbor) < k) {
                        int weight = dis_weight(gen); /**< Генерируем случайный вес для ребра. */
                        graph.addEdge(i, neighbor, weight); /**< Добавляем ребро в граф. */
                        outDegrees[i] = outDegrees.get(i) + 1; /**< Увеличиваем исходящую степень вершины. */
                        inDegrees[neighbor] = inDegrees.get(neighbor) + 1; /**< Увеличиваем входящую степень соседа. */
                    }
                }
            }

            // Проверяем, достигли ли все вершин необходимой степени
            for (int i = 0; i < vertices; ++i) {
                if (outDegrees.get(i) != k || inDegrees.get(i) != k) {
                    success = false; /**< Если хотя бы одна вершина не достигла необходимой степени, отмечаем неудачу. */
                    break;
                }
            }

            if (success) {
                break; /**< Если все вершины достигли необходимой степени, завершаем цикл. */
            }
        }

        return graph; /**< Возвращаем сгенерированный граф. */
    }

    /**
     * @brief Алгоритм Фишера-Йейтса для перемешивания элементов в `ArraySequence`.
     *
     * Метод `shuffleArraySequence` перемешивает элементы последовательности в случайном порядке,
     * используя алгоритм Фишера-Йейтса.
     *
     * @tparam T Тип элементов в последовательности.
     * @param seq Ссылка на последовательность, которую необходимо перемешать.
     * @param gen Ссылка на генератор случайных чисел.
     */
    template <typename T>
    static void shuffleArraySequence(ArraySequence<T>& seq, std::mt19937& gen) {
        for (int i = seq.getLength() - 1; i > 0; --i) {
            std::uniform_int_distribution<> dis(0, i); /**< Распределение для выбора индекса для обмена. */
            int j = dis(gen); /**< Выбираем случайный индекс для обмена. */
            Swap(seq, i, j);   /**< Обмениваем элементы на позициях i и j. */
        }
    }

    /**
     * @brief Обменивает элементы в `ArraySequence` по заданным индексам.
     *
     * Метод `Swap` выполняет обмен значениями элементов на позициях `i` и `j` в последовательности.
     *
     * @tparam T Тип элементов в последовательности.
     * @param seq Ссылка на последовательность.
     * @param i Первый индекс.
     * @param j Второй индекс.
     */
    template <typename T>
    static void Swap(ArraySequence<T>& seq, int i, int j) {
        T temp = seq.get(i); /**< Временное хранение элемента на позиции i. */
        seq[i] = seq[j];     /**< Копирование элемента с позиции j на позицию i. */
        seq[j] = temp;       /**< Копирование сохранённого элемента на позицию j. */
    }
};

#endif // LAB4_SEM3_GRAPHGENERATOR_H