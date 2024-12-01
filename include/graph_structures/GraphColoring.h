#ifndef LAB4_SEM3_GRAPHCOLORING_H
#define LAB4_SEM3_GRAPHCOLORING_H

#include "Graph.h"
#include <algorithm>
#include <stdexcept>

class GraphColoring {
public:
    static ArraySequence<int> greedyColoring(const Graph<int>& graph) {
        int n = graph.getVertexCount();
        ArraySequence<int> result;   // Цвета вершин

        if(n == 0){
            return result; // Возвращаем пустую последовательность для пустого графа
        }

        // Инициализация массивов
        for (int i = 0; i < n; ++i) {
            result.append(-1); // Все вершины изначально не раскрашены
        }

        result[0] = 0; // Первой вершине присваиваем первый цвет

        for (int u = 1; u < n; ++u) {
            // Создаем массив доступных цветов и инициализируем все как доступные
            ArraySequence<bool> available;
            for(int i = 0; i < n; ++i){
                available.append(true);
            }

            ArraySequence<Pair<int, int>> neighbors = graph.getNeighbors(u);

            // Помечаем занятые цвета соседями
            for (int i = 0; i < neighbors.getLength(); ++i) {
                int neighbor = neighbors[i].first;

                // Проверка корректности индекса соседа
                if(neighbor < 0 || neighbor >= n){
                    throw std::out_of_range("Neighbor index out of range");
                }

                if (result[neighbor] != -1) {
                    if(result[neighbor] < available.getLength()){
                        available[result[neighbor]] = false;
                    }
                }
            }

            // Выбираем первый доступный цвет
            int color = 0;
            while (color < available.getLength() && !available[color]) {
                ++color;
            }

            // Если все существующие цвета заняты, добавляем новый цвет
            if(color >= available.getLength()){
                color = available.getLength();
                available.append(false); // Добавляем новый цвет как занятый
            }

            result[u] = color;
        }

        return result;
    }
};

#endif //LAB4_SEM3_GRAPHCOLORING_H