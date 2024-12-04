// Lattice.h
#ifndef LATTICE_H
#define LATTICE_H

#include "DirectedGraph.h"
#include "../sequence/ArraySequence.h"
#include "../sequence/Pair.h"
#include "../data_structures/IDictionaryBinaryTree.h"
#include <functional>
#include <optional>
#include <iostream>

template<typename T>
class Lattice {
private:
    DirectedGraph<int> hasseDiagram; // Использование DirectedGraph с индексами
    bool isExplicit;
    std::function<bool(const T&, const T&)> relation; // Отношение для неявной диаграммы Хассе
    ArraySequence<T> elements; // Элементы решётки
    IDictionaryBinaryTree<T, int> elementToIndex; // Отображение элемента в индекс
    ArraySequence<T> indexToElement;

public:
    // Конструктор для явной диаграммы Хассе
    Lattice(const DirectedGraph<int>& diagram, const ArraySequence<T>& elems)
            : hasseDiagram(diagram), isExplicit(true), elements(elems) {
        int n = elements.getLength();
        for (int i = 0; i < n; ++i) {
            T elem = elements[i];
            elementToIndex.Add(elem, i);
            indexToElement.append(elem);
        }
    }

    // Конструктор для неявной диаграммы Хассе (построение по отношению)
    Lattice(const ArraySequence<T>& elems, std::function<bool(const T&, const T&)> rel)
            : hasseDiagram(elems.getLength()), isExplicit(false), relation(rel), elements(elems) {
        int n = elems.getLength();
        for (int i = 0; i < n; ++i) {
            T elem = elements[i];
            elementToIndex.Add(elem, i);
            indexToElement.append(elem);
        }

        // Построение диаграммы Хассе на основе отношения
        for (int i = 0; i < n; ++i) {
            T a = elements[i];
            for (int j = 0; j < n; ++j) {
                T b = elements[j];
                if (i != j && relation(a, b)) {
                    // Проверка на отсутствие промежуточных элементов
                    bool directRelation = true;
                    for (int k = 0; k < n; ++k) {
                        if (k != i && k != j) {
                            T c = elements[k];
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

    // Метод для проверки отношения a <= b
    bool lessEqual(const T& a, const T& b) const {
        if (!elementToIndex.ContainsKey(a) || !elementToIndex.ContainsKey(b)) {
            return false;
        }
        int indexA = elementToIndex.Get(a);
        int indexB = elementToIndex.Get(b);
        return hasseDiagram.hasPath(indexA, indexB);
    }

    // Нахождение наибольшей нижней грани (meet)
    std::optional<T> meet(const T& a, const T& b) const {
        ArraySequence<T> candidates;
        int n = elements.getLength();
        // Поиск общих нижних границ
        for (int i = 0; i < n; ++i) {
            T c = elements[i];
            if (lessEqual(c, a) && lessEqual(c, b)) {
                candidates.append(c);
            }
        }
        if (candidates.getLength() == 0) {
            return std::nullopt;
        }
        // Нахождение наибольшего среди кандидатов
        T glb = candidates[0];
        for (int i = 1; i < candidates.getLength(); ++i) {
            T c = candidates[i];
            if (lessEqual(glb, c)) {
                glb = c;
            }
        }
        return glb;
    }

    // Нахождение наименьшей верхней грани (join)
    std::optional<T> join(const T& a, const T& b) const {
        ArraySequence<T> candidates;
        int n = elements.getLength();
        // Поиск общих верхних границ
        for (int i = 0; i < n; ++i) {
            T c = elements[i];
            if (lessEqual(a, c) && lessEqual(b, c)) {
                candidates.append(c);
            }
        }
        if (candidates.getLength() == 0) {
            return std::nullopt;
        }
        // Нахождение наименьшего среди кандидатов
        T lub = candidates[0];
        for (int i = 1; i < candidates.getLength(); ++i) {
            T c = candidates[i];
            if (lessEqual(c, lub)) {
                lub = c;
            }
        }
        return lub;
    }

    DirectedGraph<int> getHasseDiagram() const {
        return hasseDiagram;
    }

    void printHasseDiagram() const {
        std::cout << "Hasse Diagram:" << std::endl;
        for (int i = 0; i < hasseDiagram.getVertexCount(); ++i) {
            auto neighbors = hasseDiagram.getNeighbors(i);
            for (int j = 0; j < neighbors.getLength(); ++j) {
                int from = i;
                int to = neighbors[j].first;
                std::cout << indexToElement[from] << " -> " << indexToElement[to] << std::endl;
            }
        }
    }
};

#endif // LATTICE_H