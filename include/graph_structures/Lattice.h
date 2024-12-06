#ifndef LATTICE_H
#define LATTICE_H

#include "DirectedGraph.h"
#include "../sequence/ArraySequence.h"
#include "../sequence/Pair.h"
#include "../data_structures/IDictionaryBinaryTree.h"
#include <functional>
#include <optional>
#include <iostream>
#include <stdexcept>

/**
 * @brief Класс для представления решётки и построения диаграммы Хассе.
 *
 * Класс `Lattice` предоставляет методы для построения и работы с диаграммой Хассе,
 * как в явной, так и в неявной форме.
 */
template<typename T>
class Lattice {
private:
    DirectedGraph<T> hasseDiagram; /**< Диаграмма Хассе, представленная ориентированным графом. */
    bool isExplicit; /**< Флаг, указывающий на явную или неявную форму диаграммы Хассе. */
    std::function<bool(const T&, const T&)> relation; /**< Отношение для неявной диаграммы Хассе. */
    ArraySequence<T> elements; /**< Элементы решётки. */
    IDictionaryBinaryTree<T, int> elementToIndex; /**< Отображение элемента в индекс. */
    ArraySequence<T> indexToElement; /**< Отображение индекса в элемент. */

    /**
     * @brief Реализация метода hasPath с использованием DFS.
     *
     * Метод `hasPath` проверяет наличие пути между двумя вершинами в графе с использованием
     * алгоритма поиска в глубину (DFS).
     *
     * @param src Исходная вершина.
     * @param dest Целевая вершина.
     * @param visited Массив посещённых вершин.
     * @return true, если путь существует, иначе false.
     */
    bool hasPath(int src, int dest, ArraySequence<bool>& visited) const {
        if (src == dest) {
            return true;
        }

        visited[src] = true;

        ArraySequence<Pair<int, int>> neighbors = hasseDiagram.getNeighbors(src);
        for (int i = 0; i < neighbors.getLength(); ++i) {
            int neighbor = neighbors.get(i).first;
            if (!visited.get(neighbor)) {
                if (hasPath(neighbor, dest, visited)) {
                    return true;
                }
            }
        }
        return false;
    }

public:
    /**
     * @brief Конструктор для явной диаграммы Хассе.
     *
     * @param diagram Ориентированный граф, представляющий диаграмму Хассе.
     * @param elems Последовательность элементов решётки.
     */
    Lattice(const DirectedGraph<int>& diagram, const ArraySequence<T>& elems)
            : hasseDiagram(diagram), isExplicit(true), elements(elems) {
        int n = elements.getLength();
        for (int i = 0; i < n; ++i) {
            T elem = elements.get(i);
            if (elementToIndex.ContainsKey(elem)) {
                throw std::invalid_argument("Duplicate element in elements array.");
            }
            elementToIndex.Add(elem, i);
            indexToElement.append(elem);
        }
    }

    /**
     * @brief Конструктор для неявной диаграммы Хассе (построение по отношению).
     *
     * @param elems Последовательность элементов решётки.
     * @param rel Отношение для построения диаграммы Хассе.
     */
    Lattice(const ArraySequence<T>& elems, std::function<bool(const T&, const T&)> rel)
            : hasseDiagram(elems.getLength()), isExplicit(false), relation(rel), elements(elems) {
        int n = elems.getLength();
        for (int i = 0; i < n; ++i) {
            T elem = elements.get(i);
            if (elementToIndex.ContainsKey(elem)) {
                throw std::invalid_argument("Duplicate element in elements array.");
            }
            elementToIndex.Add(elem, i);
            indexToElement.append(elem);
        }

        // Построение диаграммы Хассе на основе отношения
        for (int i = 0; i < n; ++i) {
            T a = elements.get(i);
            for (int j = 0; j < n; ++j) {
                T b = elements.get(j);
                if (i != j && relation(a, b)) {
                    // Проверка на отсутствие промежуточных элементов
                    bool directRelation = true;
                    for (int k = 0; k < n; ++k) {
                        if (k != i && k != j) {
                            T c = elements.get(k);
                            if (relation(a, c) && relation(c, b)) {
                                directRelation = false;
                                break;
                            }
                        }
                    }
                    if (directRelation) {
                        hasseDiagram.addEdge(i, j, 1); // Ребро от a к b
                    }
                }
            }
        }
    }

    /**
     * @brief Метод для проверки отношения a <= b.
     *
     * @param a Первый элемент.
     * @param b Второй элемент.
     * @return true, если a <= b, иначе false.
     *
     * @throws std::invalid_argument Если элемент не найден в решётке.
     */
    bool lessEqual(const T& a, const T& b) const {
        if (!elementToIndex.ContainsKey(a) || !elementToIndex.ContainsKey(b)) {
            throw std::invalid_argument("Element not found in lattice.");
        }
        int indexA = elementToIndex.Get(a);
        int indexB = elementToIndex.Get(b);

        ArraySequence<bool> visited;
        for(int i = 0; i < elements.getLength(); ++i){
            visited.append(false);
        }

        return hasPath(indexA, indexB, visited);
    }

    /**
     * @brief Возвращает диаграмму Хассе.
     *
     * @return DirectedGraph<T> Диаграмма Хассе.
     */
    DirectedGraph<T> getHasseDiagram() const {
        return hasseDiagram;
    }

    /**
     * @brief Печатает диаграмму Хассе.
     */
    void printHasseDiagram() const {
        std::cout << "Hasse Diagram:" << std::endl;
        for (int i = 0; i < hasseDiagram.getVertexCount(); ++i) {
            ArraySequence<Pair<int, int>> neighbors = hasseDiagram.getNeighbors(i);
            for (int j = 0; j < neighbors.getLength(); ++j) {
                int from = i;
                int to = neighbors.get(j).first;
                std::cout << indexToElement.get(from) << " -> " << indexToElement.get(to) << std::endl;
            }
        }
    }
};

#endif // LATTICE_H