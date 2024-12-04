#ifndef CONNECTED_COMPONENTS_H
#define CONNECTED_COMPONENTS_H

#include "UndirectedGraph.h"
#include "../sequence/ArraySequence.h"
#include <functional>

/**
 * @brief Класс для поиска компонент связности в неориентированном графе.
 *
 * Класс `ConnectedComponents` предоставляет метод для поиска всех компонент связности
 * в заданном неориентированном графе. Он использует алгоритм обхода в глубину (DFS)
 * для идентификации связанных групп вершин.
 */
class ConnectedComponents {
public:

    ConnectedComponents() = default;

    /**
     * @brief Находит все компоненты связности в графе.
     *
     * Метод `findComponents` выполняет поиск всех компонент связности в заданном
     * неориентированном графе, используя алгоритм обхода в глубину (DFS).
     * Каждая компонента представлена как отдельная последовательность вершин.
     *
     * @tparam T Тип веса рёбер графа.
     * @param graph Неориентированный граф, в котором выполняется поиск компонент связности.
     * @return ArraySequence<ArraySequence<int>> Список компонент связности, каждая из которых представлена
     *                                      последовательностью вершин.
     *
     * @throws std::invalid_argument Если граф содержит недопустимые вершины.
     */
    template<typename T>
    static ArraySequence<ArraySequence<int>> findComponents(const UndirectedGraph<T>& graph) {
        int vertexCount = graph.getVertexCount(); /**< Общее количество вершин в графе. */
        ArraySequence<bool> visited;               /**< Массив для отслеживания посещённых вершин. */

        // Инициализация массива посещённых вершин как false
        for (int i = 0; i < vertexCount; ++i) {
            visited.append(false);
        }

        ArraySequence<ArraySequence<int>> components; /**< Список найденных компонент связности. */

        // Итерация по всем вершинам графа
        for (int v = 0; v < vertexCount; ++v) {
            // Если вершина ещё не посещена, запускаем DFS для её компоненты связности
            if (!visited[v]) {
                ArraySequence<int> component; /**< Текущая компонента связности. */

                /**
                 * @brief Lambda-функция для добавления вершины в текущую компоненту.
                 *
                 * Эта функция вызывается для каждой посещённой вершины во время DFS.
                 *
                 * @param vertex Индекс текущей вершины.
                 */
                std::function<void(int)> visit = [&component](int vertex) {
                    component.append(vertex); /**< Добавляем вершину в компоненту. */
                };

                // Выполнение DFS начиная с вершины v
                graph.dfs(v, visited, visit);

                // Добавление найденной компоненты в список компонент
                components.append(component);
            }
        }

        return components; /**< Возвращаем список всех компонент связности. */
    }

    ~ConnectedComponents() = default;
};

#endif // CONNECTED_COMPONENTS_H