// ConnectedComponents.h
#ifndef CONNECTED_COMPONENTS_H
#define CONNECTED_COMPONENTS_H

#include "UndirectedGraph.h"
#include "../sequence/ArraySequence.h"
#include <functional>

/**
 * @brief Класс для поиска компонент связности в неориентированном графе.
 */
class ConnectedComponents {
public:
    ConnectedComponents() = default;

    /**
     * @brief Находит все компоненты связности в графе.
     *
     * @tparam T Тип веса рёбер.
     * @param graph Неориентированный граф.
     * @return ArraySequence<ArraySequence<int>> Список компонент связности.
     */
    template<typename T>
    static ArraySequence<ArraySequence<int>> findComponents(const UndirectedGraph<T>& graph) {
        int vertexCount = graph.getVertexCount();
        ArraySequence<bool> visited;
        for (int i = 0; i < vertexCount; ++i) {
            visited.append(false);
        }

        ArraySequence<ArraySequence<int>> components;

        for (int v = 0; v < vertexCount; ++v) {
            if (!visited[v]) {
                ArraySequence<int> component;
                // Lambda-функция для добавления вершины в текущую компоненту
                std::function<void(int)> visit = [&component](int vertex) {
                    component.append(vertex);
                };
                graph.dfs(v, visited, visit);
                components.append(component);
            }
        }

        return components;
    }

    ~ConnectedComponents() = default;
};

#endif // CONNECTED_COMPONENTS_H