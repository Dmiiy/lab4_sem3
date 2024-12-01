// Lattice.h
#ifndef LATTICE_H
#define LATTICE_H

#include "Graph.h"
#include <functional>
#include <unordered_map>
#include <optional>
#include <vector>

template<typename T>
class Lattice {
private:
    Graph<T> hasseDiagram;
    bool isExplicit;
    std::function<bool(T, T)> relation; // Компаратор для неявной диаграммы Хассе
    ArraySequence<T> elements; // Элементы решётки
    std::unordered_map<T, int> elementToIndex; // Соответствие элемента индексу
    ArraySequence<T> indexToElement;

public:
    // Конструктор для явной диаграммы Хассе
    Lattice(const Graph<T>& diagram, const ArraySequence<T>& elems)
            : hasseDiagram(diagram), isExplicit(true), elements(elems) {
        int n = elements.getLength();
        for (int i = 0; i < n; ++i) {
            T elem = elements[i];
            elementToIndex[elem] = i;
            indexToElement.append(elem);
        }
    }

    // Конструктор для неявной диаграммы Хассе (построение по отношению)
    Lattice(const ArraySequence<T>& elems, std::function<bool(T, T)> rel)
            : hasseDiagram(elems.getLength()), isExplicit(false), relation(rel), elements(elems) {
        int n = elems.getLength();
        for (int i = 0; i < n; ++i) {
            T elem = elems[i];
            elementToIndex[elem] = i;
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
    bool lessEqual(T a, T b) const {
        if (isExplicit) {
            // Используем hasseDiagram для проверки пути от a к b
            auto itA = elementToIndex.find(a);
            auto itB = elementToIndex.find(b);
            if (itA == elementToIndex.end() || itB == elementToIndex.end()) {
                return false;
            }
            int indexA = itA->second;
            int indexB = itB->second;
            return hasPath(indexA, indexB);
        } else {
            // Используем заданное отношение
            return relation(a, b);
        }
    }

    // Метод для поиска пути от одного индекса к другому (DFS)
    bool hasPath(int fromIndex, int toIndex) const {
        if (fromIndex == toIndex) return true;
        int n = hasseDiagram.getVertexCount();
        std::vector<bool> visited(n, false);
        return dfs(fromIndex, toIndex, visited);
    }

    bool dfs(int current, int target, std::vector<bool>& visited) const {
        if (current == target) return true;
        visited[current] = true;
        auto neighbors = hasseDiagram.getNeighbors(current);
        for (int i = 0; i < neighbors.getLength(); ++i) {
            int neighbor = neighbors[i].first;
            if (!visited[neighbor]) {
                if (dfs(neighbor, target, visited)) {
                    return true;
                }
            }
        }
        return false;
    }

    // Метод для нахождения встречи (наибольшей нижней грани) двух элементов
    std::optional<T> meet(T a, T b) const {
        ArraySequence<T> candidates;
        int n = elements.getLength();
        // Поиск элементов, меньших или равных a и b
        for (int i = 0; i < n; ++i) {
            T c = elements[i];
            if (lessEqual(c, a) && lessEqual(c, b)) {
                candidates.append(c);
            }
        }
        if (candidates.getLength() == 0) {
            // Нет общей нижней грани
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

    // Метод для нахождения объединения (наименьшей верхней грани) двух элементов
    std::optional<T> join(T a, T b) const {
        ArraySequence<T> candidates;
        int n = elements.getLength();
        // Поиск элементов, больших или равных a и b
        for (int i = 0; i < n; ++i) {
            T c = elements[i];
            if (lessEqual(a, c) && lessEqual(b, c)) {
                candidates.append(c);
            }
        }
        if (candidates.getLength() == 0) {
            // Нет общей верхней грани
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
};

#endif // LATTICE_H