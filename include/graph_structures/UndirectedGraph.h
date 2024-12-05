#ifndef UNDIRECTEDGRAPH_H
#define UNDIRECTEDGRAPH_H

#include "Graph.h"
#include <set>
#include "DirectedGraph.h"

/**
 * @brief Класс для представления неориентированного графа.
 *
 * Этот класс наследуется от абстрактного класса Graph и реализует типографию
 * неориентированного графа с использованием списка смежности на основе бинарных деревьев.
 *
 * @tparam T Тип данных, ассоциированный с рёбрами графа (например, вес рёбер).
 */
template<class T>
class UndirectedGraph : public Graph<T>{
private:
    DirectedGraph<T> directedGraph; /**< Список смежности для хранения рёбер графа. */

public:
    /**
     * @brief Конструктор для инициализации графа с заданным количеством вершин.
     *
     * Каждая вершина инициализируется пустым словарём для хранения соседних вершин.
     *
     * @param vertices Количество вершин в графе.
     */
    explicit UndirectedGraph(int vertices):directedGraph(vertices) {}


    /**
     * @brief Конструктор копирования.
     *
     * Создаёт новый граф как копию существующего графа.
     *
     * @param graph Граф, который необходимо скопировать.
     */
    UndirectedGraph(const UndirectedGraph<T> &graph) : directedGraph(graph.directedGraph) {}

    /**
     * @brief Оператор присваивания.
     *
     * Присваивает текущему графу значения другого графа.
     *
     * @param graph Граф, значения которого необходимо присвоить.
     * @return Ссылка на текущий граф.
     */
    UndirectedGraph<T> &operator=(const UndirectedGraph<T> &graph) {
        if (this == &graph) {
            return *this; // Самоприсваивание, ничего не делаем
        }

        directedGraph = graph.directedGraph;
        return *this;
    }

    /**
     * @brief Добавляет ребро между двумя вершинами с указанным весом.
     *
     * В неориентированном графе добавление ребра означает, что обе вершины становятся
     * соседними друг для друга.
     *
     * @param from Начальная вершина.
     * @param to Конечная вершина.
     * @param weight Вес ребра или другой ассоциированный параметр.
     * @throws std::out_of_range Если индексы вершин выходят за допустимый диапазон.
     */
    void addEdge(int from, int to, T weight) override {
        directedGraph.addEdge(from, to, weight);
        directedGraph.addEdge(to, from, weight);
    }

    /**
     * @brief Удаляет ребро между двумя вершинами.
     *
     * В неориентированном графе удаление ребра означает, что обе вершины перестают быть
     * соседними друг для друга.
     *
     * @param from Начальная вершина.
     * @param to Конечная вершина.
     * @throws std::invalid_argument Если ребро не найдено между вершинами.
     */
    void removeEdge(int from, int to) override {
        directedGraph.removeEdge(from, to);
        directedGraph.removeEdge(to, from);
    }

    /**
     * @brief Проверяет наличие ребра между двумя вершинами.
     *
     * @param from Начальная вершина.
     * @param to Конечная вершина.
     * @return true Если ребро существует.
     * @return false Если ребра нет.
     */
    bool hasEdge(int from, int to) const override {
        return directedGraph.hasEdge(from, to);
    }

    /**
     * @brief Возвращает степень вершины.
     *
     * Степень вершины в неориентированном графе — количество рёбер, инцидентных этой вершине.
     *
     * @param vertex Вершина, для которой необходимо определить степень.
     * @return int Степень вершины.
     * @throws std::out_of_range Если вершина не существует в графе.
     */
    int getDegree(int vertex) const override {
        return directedGraph.getDegree(vertex);
    }

    /**
     * @brief Возвращает список соседних вершин для заданной вершины.
     *
     * @param vertex Вершина, для которой необходимо получить список соседей.
     * @return ArraySequence<Pair<int, T>> Список пар (сосед, вес).
     */
    ArraySequence<Pair<int, T>> getNeighbors(int vertex) const override {
        return directedGraph.getNeighbors(vertex);
    }

    /**
     * @brief Возвращает вес ребра между двумя вершинами.
     *
     * @param from Начальная вершина.
     * @param to Конечная вершина.
     * @return T Вес ребра.
     * @throws std::invalid_argument Если ребро не найдено.
     */
    T getEdgeWeight(int from, int to) const override {
        return directedGraph.getEdgeWeight(from, to);
    }

    /**
     * @brief Выводит все рёбра графа в стандартный поток вывода.
     *
     * Каждое ребро выводится один раз с указанием вершин и веса.
     */
    void printGraph() const override {
        directedGraph.printGraph();
    }

    /**
     * @brief Возвращает количество вершин в графе.
     *
     * @return int Количество вершин.
     */
    int getVertexCount() const override {
        return directedGraph.getVertexCount();
    }

    /**
     * @brief Выполняет глубинный поиск (DFS) начиная с заданной вершины.
     *
     * Метод рекурсивно посещает все достижимые вершины из начальной вершины.
     *
     * @param startVertex Вершина, с которой начинается поиск.
     * @param visited Список посещённых вершин.
     * @param visit Функция, вызываемая при посещении вершины (может быть nullptr).
     * @throws std::out_of_range Если начальная вершина не существует в графе.
     */
    void dfs(int startVertex, ArraySequence<bool>& visited, std::function<void(int)> visit = nullptr) const {
        directedGraph.dfs(startVertex, visited, visit);
    }

    /**
     * @brief Возвращает все рёбра графа.
     *
     * Каждое ребро представлено в виде кортежа (from, to, weight).
     *
     * @return ArraySequence<std::tuple<int, int, T>> Список всех рёбер.
     */
    ArraySequence<std::tuple<int, int, T>> getEdges() const override {
        ArraySequence<std::tuple<int, int, T>> edges;
        std::set<std::pair<int, int>> seen; // Для избегания дублирования рёбер
        auto adjacencyList=directedGraph.getAdjacencyList();
        auto keys = adjacencyList.GetKeys();
        for (int i = 0; i < keys.getLength(); ++i) {
            int from = keys[i];
            auto neighborDict = adjacencyList.Get(from);
            auto neighbors = neighborDict.GetKeys();
            for (int j = 0; j < neighbors.getLength(); ++j) {
                int to = neighbors[j];
                if (from < to && seen.find(std::make_pair(from, to)) == seen.end()) {
                    T weight = neighborDict.Get(to);
                    edges.append(std::make_tuple(from, to, weight));
                    seen.emplace(from, to); // Помечаем ребро как обработанное
                }
            }
        }

        return edges;
    }

    /**
     * @brief Возвращает список смежности графа.
     *
     * @return IDictionaryBinaryTree<int, IDictionaryBinaryTree<int, T>> Список смежности.
     */
    IDictionaryBinaryTree<int, IDictionaryBinaryTree<int, T>> getAdjacencyList() const {
        return directedGraph.getAdjacencyList();
    }

    /**
     * @brief Деструктор по умолчанию.
     */
    ~UndirectedGraph() override = default;


};

#endif // UNDIRECTEDGRAPH_H