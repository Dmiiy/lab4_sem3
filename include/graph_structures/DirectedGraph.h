#ifndef DIRECTEDGRAPH_H
#define DIRECTEDGRAPH_H

#include <stack>
#include "Graph.h"

/**
 * @brief Класс для представления ориентированного графа.
 *
 * Этот класс наследуется от абстрактного класса Graph и реализует типографию
 * ориентированного графа с использованием списка смежности на основе бинарных деревьев.
 *
 * @tparam T Тип данных, ассоциированный с рёбрами графа (например, вес рёбер).
 */
template<class T>
class DirectedGraph : public Graph<T> {
private:
    IDictionaryBinaryTree<int, IDictionaryBinaryTree<int, T>> adjacencyList; ///< Список смежности графа.
    int vertexCount; ///< Количество вершин в графе.

public:
    /**
     * @brief Конструктор для инициализации графа с заданным количеством вершин.
     *
     * Каждая вершина инициализируется пустым словарём для хранения исходящих ребер.
     *
     * @param vertices Количество вершин в графе.
     */
    explicit DirectedGraph(int vertices) : vertexCount(vertices) {
        // Инициализируем список смежности пустыми словарями для каждой вершины
        for (int i = 0; i < vertices; ++i) {
            adjacencyList.Add(i, IDictionaryBinaryTree<int, T>());
        }
    }

    /**
     * @brief Конструктор копирования.
     *
     * Создаёт новый граф как копию существующего графа.
     *
     * @param graph Граф, который необходимо скопировать.
     */
    DirectedGraph(const DirectedGraph<T> &graph) : vertexCount(graph.vertexCount), adjacencyList(graph.adjacencyList) {}

    /**
     * @brief Оператор присваивания.
     *
     * Присваивает текущему графу значения другого графа.
     *
     * @param graph Граф, значения которого необходимо присвоить.
     * @return Ссылка на текущий граф.
     */
    DirectedGraph<T> &operator=(const DirectedGraph<T> &graph) {
        if (this == &graph) {
            return *this; // Самоприсваивание, ничего не делаем
        }
        vertexCount = graph.vertexCount;
        adjacencyList = graph.adjacencyList;
        return *this;
    }

    /**
     * @brief Добавляет ориентированное ребро из одной вершины в другую с указанным весом.
     *
     * В ориентированном графе добавление ребра означает, что только одна вершина становится
     * соседом другой.
     *
     * @param from Начальная вершина.
     * @param to Конечная вершина.
     * @param weight Вес ребра или другой ассоциированный параметр.
     * @throws std::out_of_range Если индексы вершин выходят за допустимый диапазон.
     */
    void addEdge(int from, int to, T weight) override {
        if (from < 0 || from >= vertexCount || to < 0 || to >= vertexCount) {
            throw std::out_of_range("Invalid vertex index");
        }
        adjacencyList.GetReference(from).Add(to, weight); // Добавляем направление 'from' -> 'to'
    }

    /**
     * @brief Удаляет ориентированное ребро из одной вершины в другую.
     *
     * @param from Начальная вершина.
     * @param to Конечная вершина.
     * @throws std::invalid_argument Если ребро не найдено.
     */
    void removeEdge(int from, int to) override {
        if (adjacencyList.ContainsKey(from) && adjacencyList.Get(from).ContainsKey(to)) {
            adjacencyList.GetReference(from).Remove(to); // Удаляем направление 'from' -> 'to'
        } else {
            throw std::invalid_argument("Edge not found");
        }
    }

    /**
     * @brief Проверяет наличие ориентированного ребра из одной вершины в другую.
     *
     * @param from Начальная вершина.
     * @param to Конечная вершина.
     * @return true Если ребро существует.
     * @return false Если ребра нет.
     */
    bool hasEdge(int from, int to) const override {
        return adjacencyList.ContainsKey(from) && adjacencyList.Get(from).ContainsKey(to);
    }

    /**
     * @brief Возвращает степень вершины.
     *
     * В ориентированном графе степень вершины может быть разделена на
     * входящую и исходящую степень. Этот метод возвращает исходящую степень.
     *
     * @param vertex Вершина, для которой необходимо определить степень.
     * @return int Степень вершины.
     * @throws std::out_of_range Если вершина не существует в графе.
     */
    int getDegree(int vertex) const override {
        if (!adjacencyList.ContainsKey(vertex)) {
            throw std::out_of_range("Vertex not found");
        }
        return adjacencyList.Get(vertex).GetCount();
    }

    /**
     * @brief Возвращает список соседних вершин для заданной вершины.
     *
     * В ориентированном графе список соседей включает только вершины,
     * на которые идут исходящие рёбра из данной вершины.
     *
     * @param vertex Вершина, для которой необходимо получить список соседей.
     * @return ArraySequence<Pair<int, T>> Список пар (сосед, вес).
     */
    ArraySequence<Pair<int, T>> getNeighbors(int vertex) const override {
        ArraySequence<Pair<int, T>> neighbors;
        if (adjacencyList.ContainsKey(vertex)) {
            auto neighborDict = adjacencyList.Get(vertex);
            auto keys = neighborDict.GetKeys();
            for (int i = 0; i < keys.getLength(); ++i) {
                int neighbor = keys[i];
                T weight = neighborDict.Get(neighbor);
                neighbors.append(Pair<int, T>(neighbor, weight));
            }
        }
        return neighbors;
    }

    /**
     * @brief Возвращает вес ориентированного ребра между двумя вершинами.
     *
     * @param from Начальная вершина.
     * @param to Конечная вершина.
     * @return T Вес ребра.
     * @throws std::invalid_argument Если ребро не найдено.
     */
    T getEdgeWeight(int from, int to) const override {
        if (hasEdge(from, to)) {
            return adjacencyList.Get(from).Get(to);
        }
        throw std::invalid_argument("Edge not found");
    }

    /**
     * @brief Выводит все ориентированные рёбра графа в стандартный поток вывода.
     *
     * Каждое ребро выводится с указанием направления и веса.
     */
    void printGraph() const override {
        auto keys = adjacencyList.GetKeys();
        for (int i = 0; i < keys.getLength(); ++i) {
            int from = keys[i];
            auto neighborDict = adjacencyList.Get(from);
            auto neighbors = neighborDict.GetKeys();
            for (int j = 0; j < neighbors.getLength(); ++j) {
                int to = neighbors[j];
                T weight = neighborDict.Get(to);
                std::cout << "Edge (" << from << " -> " << to << ") with weight: " << weight << std::endl;
            }
        }
    }

    /**
     * @brief Возвращает количество вершин в графе.
     *
     * @return int Количество вершин.
     */
    int getVertexCount() const override {
        return vertexCount;
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
        if (startVertex < 0 || startVertex >= vertexCount) {
            throw std::out_of_range("Invalid start vertex");
        }
        if (visited.getLength() != vertexCount) {
            ArraySequence<bool> falseVisited;
            for (int i = 0; i < vertexCount; ++i) {
                falseVisited.append(false);
            }
            visited = falseVisited;
        }
        dfsUtil(startVertex, visited, visit);
    }

    /**
     * @brief Возвращает все ориентированные рёбра графа.
     *
     * Каждое ребро представлено в виде кортежа (from, to, weight).
     *
     * @return ArraySequence<std::tuple<int, int, T>> Список всех ориетированных рёбер.
     */
    ArraySequence<std::tuple<int, int, T>> getEdges() const override {
        ArraySequence<std::tuple<int, int, T>> edges;
        auto keys = adjacencyList.GetKeys();
        for (int i = 0; i < keys.getLength(); ++i) {
            int from = keys[i];
            auto neighborDict = adjacencyList.Get(from);
            auto neighbors = neighborDict.GetKeys();
            for (int j = 0; j < neighbors.getLength(); ++j) {
                int to = neighbors[j];
                T weight = neighborDict.Get(to);
                edges.append(std::make_tuple(from, to, weight));
            }
        }

        return edges;
    }

    /**
     * @brief Возвращает транспонированный граф (граф с инвертированными направлениями всех рёбер).
     *
     * Транспонированный граф используется в алгоритме Косарайю для поиска сильно связанных компонент.
     *
     * @param graph Оригинальный ориентированный граф.
     * @return DirectedGraph<T> Транспонированный граф.
     */
    DirectedGraph<T> getTranspose(const DirectedGraph<T>& graph) const {
        int vertexCount = graph.getVertexCount();
        DirectedGraph<T> transposedGraph(vertexCount);

        // Инвертируем направления всех рёбер
        for (int u = 0; u < vertexCount; ++u) {
            auto neighbors = graph.getNeighbors(u);
            for (int i = 0; i < neighbors.getLength(); ++i) {
                int v = neighbors[i].first;
                T weight = neighbors[i].second;
                transposedGraph.addEdge(v, u, weight); // Инвертируем ребро
            }
        }

        return transposedGraph;
    }

    /**
     * @brief Возвращает список смежности графа.
     *
     * @return IDictionaryBinaryTree<int, IDictionaryBinaryTree<int, T>> Список смежности.
     */
    IDictionaryBinaryTree<int, IDictionaryBinaryTree<int, T>> getAdjacencyList() const {
        return adjacencyList;
    }

    /**
     * @brief Деструктор по умолчанию.
     */
    ~DirectedGraph() override = default;

private:
    /**
     * @brief Рекурсивная вспомогательная функция для выполнения DFS.
     *
     * Отмечает текущую вершину как посещённую и рекурсивно посещает всех
     * непосещённых соседей.
     *
     * @param vertex Текущая вершина.
     * @param visited Список посещённых вершин.
     * @param visit Функция, вызываемая при посещении вершины (может быть nullptr).
     */
    void dfsUtil(int vertex, ArraySequence<bool>& visited, std::function<void(int)> visit) const {
        visited[vertex] = true;
        if (visit) {
            visit(vertex);
        }

        auto neighbors = getNeighbors(vertex);
        for (int i = 0; i < neighbors.getLength(); ++i) {
            int neighbor = neighbors[i].first;
            if (!visited[neighbor]) {
                dfsUtil(neighbor, visited, visit);
            }
        }
    }
};

#endif // DIRECTEDGRAPH_H