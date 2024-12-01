#ifndef LAB4_SEM3_GRAPH_H
#define LAB4_SEM3_GRAPH_H
#include <stdexcept>
#include "../sequence/ArraySequence.h"
#include <tuple>  // Для работы с std::tuple
#include "../sequence/Pair.h"
template<class T>
class Graph {
private:
    ArraySequence<std::tuple<int, int, T>> edges;  // Список рёбер с весами (from, to, weight)
    int vertexCount; // Количество вершин

public:
    // Конструктор
    explicit Graph(int vertices) : vertexCount(vertices), edges() {}

    // Добавление ребра
    void addEdge(int from, int to, T weight) {
        if (from < 0 || from >= vertexCount || to < 0 || to >= vertexCount) {
            throw std::out_of_range("Invalid vertex index");
        }
        edges.append(std::make_tuple(from, to, weight));
    }

    // Добавление неориентированного ребра
    void addUndirectedEdge(int from, int to, T weight) {
        addEdge(from, to, weight);
        addEdge(to, from, weight);  // Для неориентированного графа добавляем ребро в обе стороны
    }

    // Удаление ребра
    void removeEdge(int from, int to) {
        for (int i = 0; i < edges.getLength(); ++i) {
            auto edge = edges[i];
            if (std::get<0>(edge) == from && std::get<1>(edge) == to) {
                edges.removeAt(i);
                return;
            }
        }
        throw std::invalid_argument("Edge not found");
    }

    // Проверка наличия ребра
    bool hasEdge(int from, int to) const {
        for (int i = 0; i < edges.getLength(); ++i) {
            if (std::get<0>(edges[i]) == from && std::get<1>(edges[i]) == to) {
                return true;
            }
        }
        return false;
    }

    // Получение степени вершины
    int getDegree(int vertex) const {
        int degree = 0;
        for (int i = 0; i < edges.getLength(); ++i) {
            auto edge = edges[i];
            if (std::get<0>(edge) == vertex || std::get<1>(edge) == vertex) {
                degree++;
            }
        }
        return degree;
    }

    // Получение соседей вершины
    ArraySequence<Pair<int, T>> getNeighbors(int vertex) const {
        ArraySequence<Pair<int, T>> neighbors;
        for (int i = 0; i < edges.getLength(); ++i) {
            auto edge = edges[i];
            int u = std::get<0>(edge);
            int v = std::get<1>(edge);
            T weight = std::get<2>(edge);

            if (u == vertex) {
                neighbors.append({v, weight});
            }
        }
        return neighbors;
    }

    // Получение всех рёбер графа
    const ArraySequence<std::tuple<int, int, T>>& getEdges() const {
        return edges;
    }

    // Получение количества вершин
    int getVertexCount() const {
        return vertexCount;
    }

    // Очистка всех рёбер
    void clearEdges() {
        edges.clear();
    }

    // Вывод графа (все рёбра)
    void printGraph() const {
        for (int i = 0; i < edges.getLength(); ++i) {
            auto edge = edges[i];
            std::cout << "Edge (" << std::get<0>(edge) << ", " << std::get<1>(edge)
                      << ") with weight: " << std::get<2>(edge) << std::endl;
        }
    }

    // Деструктор
    ~Graph() = default;

    // Конструктор копирования и оператор присваивания
    Graph(const Graph& other) = default;
    Graph& operator=(const Graph& other) = default;

    // Получение веса ребра
    T getEdgeWeight(int from, int to) const {
        for (int i = 0; i < edges.getLength(); ++i) {
            if (std::get<0>(edges[i]) == from && std::get<1>(edges[i]) == to) {
                return std::get<2>(edges[i]);
            }
        }
        throw std::invalid_argument("Edge not found");
    }
};

#endif