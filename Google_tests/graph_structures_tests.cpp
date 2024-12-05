#include <queue>
#include "gtest/gtest.h"
#include "../include/graph_structures/DirectedGraph.h"
#include "../include/graph_structures/ConnectedComponents.h"
#include "../include/graph_structures/GraphColoring.h"
#include "../include/graph_structures/Lattice.h"
#include "../include/graph_structures/MinimumSpanningTree.h"
#include "../include/graph_structures/ShortestPath.h"
#include "../include/graph_structures/StronglyConnectedComponents.h"
#include "../include/graph_structures/GraphGenerator.h"
#include "../include/graph_structures/UndirectedGraph.h"
#include "../include/graph_structures/DynamicWeightShortestPath.h"


TEST(DirectedGraphTest, Constructor) {
    DirectedGraph<int> graph(5);
    EXPECT_EQ(graph.getVertexCount(), 5);
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(graph.getDegree(i), 0);
    }
}

TEST(DirectedGraphTest, AddEdge) {
    DirectedGraph<int> graph(3);
    graph.addEdge(0, 1, 10);
    EXPECT_TRUE(graph.hasEdge(0, 1));
    EXPECT_EQ(graph.getDegree(0), 1);
    EXPECT_EQ(graph.getEdgeWeight(0, 1), 10);
}

TEST(DirectedGraphTest, AddEdgeInvalid) {
    DirectedGraph<int> graph(2);
    EXPECT_THROW(graph.addEdge(-1, 1, 5), std::out_of_range);
    EXPECT_THROW(graph.addEdge(0, 2, 5), std::out_of_range);
}

TEST(DirectedGraphTest, RemoveEdge) {
    DirectedGraph<int> graph(3);
    graph.addEdge(0, 1, 20);
    EXPECT_TRUE(graph.hasEdge(0, 1));
    graph.removeEdge(0, 1);
    EXPECT_FALSE(graph.hasEdge(0, 1));
    EXPECT_EQ(graph.getDegree(0), 0);
}

TEST(DirectedGraphTest, RemoveEdgeNotFound) {
    DirectedGraph<int> graph(2);
    EXPECT_THROW(graph.removeEdge(0, 1), std::invalid_argument);
}

TEST(DirectedGraphTest, GetDegree) {
    DirectedGraph<int> graph(4);
    graph.addEdge(0, 1, 5);
    graph.addEdge(0, 2, 10);
    graph.addEdge(0, 3, 15);
    EXPECT_EQ(graph.getDegree(0), 3);
    EXPECT_EQ(graph.getDegree(1), 0);
    EXPECT_EQ(graph.getDegree(2), 0);
}

TEST(DirectedGraphTest, GetNeighbors) {
    DirectedGraph<int> graph(3);
    graph.addEdge(0, 1, 7);
    graph.addEdge(0, 2, 14);

    auto neighbors = graph.getNeighbors(0);
    EXPECT_EQ(neighbors.getLength(), 2);
    EXPECT_EQ(neighbors[0].first, 1);
    EXPECT_EQ(neighbors[0].second, 7);
    EXPECT_EQ(neighbors[1].first, 2);
    EXPECT_EQ(neighbors[1].second, 14);
}

TEST(DirectedGraphTest, GetEdgeWeight) {
    DirectedGraph<int> graph(2);
    graph.addEdge(0, 1, 30);
    EXPECT_EQ(graph.getEdgeWeight(0, 1), 30);
    EXPECT_THROW(graph.getEdgeWeight(1, 0), std::invalid_argument);
}

TEST(DirectedGraphTest, HasEdge) {
    DirectedGraph<int> graph(2);
    graph.addEdge(0, 1, 50);
    EXPECT_TRUE(graph.hasEdge(0, 1));
    EXPECT_FALSE(graph.hasEdge(1, 0));
}

// Тесты для UndirectedGraph
TEST(UndirectedGraphTest, Constructor) {
    UndirectedGraph<int> graph(5);
    EXPECT_EQ(graph.getVertexCount(), 5);
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(graph.getDegree(i), 0);
    }
}

TEST(UndirectedGraphTest, AddEdge) {
    UndirectedGraph<int> graph(3);
    graph.addEdge(0, 1, 15);
    EXPECT_TRUE(graph.hasEdge(0, 1));
    EXPECT_TRUE(graph.hasEdge(1, 0));
    EXPECT_EQ(graph.getDegree(0), 1);
    EXPECT_EQ(graph.getDegree(1), 1);
    EXPECT_EQ(graph.getEdgeWeight(0, 1), 15);
    EXPECT_EQ(graph.getEdgeWeight(1, 0), 15);
}

TEST(UndirectedGraphTest, AddEdgeInvalid) {
    UndirectedGraph<int> graph(2);
    EXPECT_THROW(graph.addEdge(-1, 1, 5), std::out_of_range);
    EXPECT_THROW(graph.addEdge(0, 2, 5), std::out_of_range);
}

TEST(UndirectedGraphTest, RemoveEdge) {
    UndirectedGraph<int> graph(3);
    graph.addEdge(0, 1, 20);
    EXPECT_TRUE(graph.hasEdge(0, 1));
    EXPECT_TRUE(graph.hasEdge(1, 0));
    graph.removeEdge(0, 1);
    EXPECT_FALSE(graph.hasEdge(0, 1));
    EXPECT_FALSE(graph.hasEdge(1, 0));
    EXPECT_EQ(graph.getDegree(0), 0);
    EXPECT_EQ(graph.getDegree(1), 0);
}

TEST(UndirectedGraphTest, RemoveEdgeNotFound) {
    UndirectedGraph<int> graph(2);
    EXPECT_THROW(graph.removeEdge(0, 1), std::invalid_argument);
}

TEST(UndirectedGraphTest, GetDegree) {
    UndirectedGraph<int> graph(4);
    graph.addEdge(0, 1, 5);
    graph.addEdge(0, 2, 10);
    graph.addEdge(0, 3, 15);
    EXPECT_EQ(graph.getDegree(0), 3);
    EXPECT_EQ(graph.getDegree(1), 1);
    EXPECT_EQ(graph.getDegree(2), 1);
    EXPECT_EQ(graph.getDegree(3), 1);
}

TEST(UndirectedGraphTest, GetNeighbors) {
    UndirectedGraph<int> graph(3);
    graph.addEdge(0, 1, 7);
    graph.addEdge(0, 2, 14);

    auto neighbors = graph.getNeighbors(0);
    EXPECT_EQ(neighbors.getLength(), 2);
    EXPECT_TRUE((neighbors[0].first == 1 && neighbors[0].second == 7) ||
                (neighbors[0].first == 2 && neighbors[0].second == 14));
    EXPECT_TRUE((neighbors[1].first == 1 && neighbors[1].second == 7) ||
                (neighbors[1].first == 2 && neighbors[1].second == 14));
}

TEST(UndirectedGraphTest, GetEdgeWeight) {
    UndirectedGraph<int> graph(2);
    graph.addEdge(0, 1, 30);
    EXPECT_EQ(graph.getEdgeWeight(0, 1), 30);
    EXPECT_EQ(graph.getEdgeWeight(1, 0), 30);
}

TEST(UndirectedGraphTest, HasEdge) {
    UndirectedGraph<int> graph(2);
    graph.addEdge(0, 1, 50);
    EXPECT_TRUE(graph.hasEdge(0, 1));
    EXPECT_TRUE(graph.hasEdge(1, 0));
}

// Тесты для ConnectedComponents
TEST(ConnectedComponentsTest, EmptyGraph) {
    UndirectedGraph<int> graph(0);
    auto components = ConnectedComponents::findComponents(graph);
    EXPECT_EQ(components.getLength(), 0);
}

TEST(ConnectedComponentsTest, SingleVertex) {
    UndirectedGraph<int> graph(1);
    auto components = ConnectedComponents::findComponents(graph);
    EXPECT_EQ(components.getLength(), 1);
    EXPECT_EQ(components[0].getLength(), 1);
    EXPECT_EQ(components[0][0], 0);
}

TEST(ConnectedComponentsTest, DisconnectedGraph) {
    UndirectedGraph<int> graph(4);
    graph.addEdge(0, 1, 1);
    graph.addEdge(2, 3, 1);
    auto components = ConnectedComponents::findComponents(graph);
    EXPECT_EQ(components.getLength(), 2);

    ArraySequence<int> component1 = components[0];
    ArraySequence<int> component2 = components[1];

    // Проверяем наличие вершин в компонентах
    EXPECT_TRUE((component1.find(0) && component1.find(1)) || (component1.find(2) && component1.find(3)));
    EXPECT_TRUE((component2.find(0) && component2.find(1)) || (component2.find(2) && component2.find(3)));
}

TEST(ConnectedComponentsTest, FullyConnectedGraph) {
    UndirectedGraph<int> graph(3);
    graph.addEdge(0, 1, 1);
    graph.addEdge(1, 2, 1);
    graph.addEdge(0, 2, 1);
    auto components = ConnectedComponents::findComponents(graph);
    EXPECT_EQ(components.getLength(), 1);
    EXPECT_EQ(components[0].getLength(), 3);
    EXPECT_TRUE(components[0].find(0));
    EXPECT_TRUE(components[0].find(1));
    EXPECT_TRUE(components[0].find(2));
}

// Тесты для GraphColoring
TEST(GraphColoringTest, EmptyGraph) {
    UndirectedGraph<int> graph(0);
    auto colors = GraphColoring::greedyColoring(graph);
    EXPECT_EQ(colors.getLength(), 0);
}

TEST(GraphColoringTest, SingleVertex) {
    UndirectedGraph<int> graph(1);
    auto colors = GraphColoring::greedyColoring(graph);
    EXPECT_EQ(colors.getLength(), 1);
    EXPECT_EQ(colors[0], 0);
}

TEST(GraphColoringTest, TwoConnectedVertices) {
    UndirectedGraph<int> graph(2);
    graph.addEdge(0, 1, 1);
    auto colors = GraphColoring::greedyColoring(graph);
    EXPECT_EQ(colors.getLength(), 2);
    EXPECT_NE(colors[0], colors[1]);
}

TEST(GraphColoringTest, TriangleGraph) {
    UndirectedGraph<int> graph(3);
    graph.addEdge(0, 1, 1);
    graph.addEdge(1, 2, 1);
    graph.addEdge(0, 2, 1);
    auto colors = GraphColoring::greedyColoring(graph);
    EXPECT_EQ(colors.getLength(), 3);
    // В треугольнике требуется 3 цвета
    int uniqueColors = 0;
    std::set<int> colorSet;
    for(int i = 0; i < colors.getLength(); ++i) {
        colorSet.insert(colors[i]);
    }
    uniqueColors = colorSet.size();
    EXPECT_EQ(uniqueColors, 3);
}

TEST(GraphColoringTest, BipartiteGraph) {
    UndirectedGraph<int> graph(4);
    graph.addEdge(0, 1, 1);
    graph.addEdge(0, 3, 1);
    graph.addEdge(2, 1, 1);
    graph.addEdge(2, 3, 1);
    auto colors = GraphColoring::greedyColoring(graph);
    EXPECT_EQ(colors.getLength(), 4);
    // В двудольном графе достаточно 2 цветов
    std::set<int> colorSet;
    for(int i = 0; i < colors.getLength(); ++i) {
        colorSet.insert(colors[i]);
    }
    EXPECT_LE(colorSet.size(), 2);
}

TEST(MinimumSpanningTreeTest, EmptyGraph) {
    UndirectedGraph<int> graph(0);
    auto mst = MinimumSpanningTree<int>::kruskal(graph);
    EXPECT_EQ(mst.getLength(), 0);
}

TEST(MinimumSpanningTreeTest, SingleVertex) {
    UndirectedGraph<int> graph(1);
    auto mst = MinimumSpanningTree<int>::kruskal(graph);
    EXPECT_EQ(mst.getLength(), 0);
}

TEST(MinimumSpanningTreeTest, TwoConnectedVertices) {
    UndirectedGraph<int> graph(2);
    graph.addEdge(0, 1, 5);
    auto mst = MinimumSpanningTree<int>::kruskal(graph);
    EXPECT_EQ(mst.getLength(), 1);
    EXPECT_EQ(std::get<0>(mst[0]), 0);
    EXPECT_EQ(std::get<1>(mst[0]), 1);
    EXPECT_EQ(std::get<2>(mst[0]), 5);
}

TEST(MinimumSpanningTreeTest, DisconnectedGraph) {
    UndirectedGraph<int> graph(4);
    graph.addEdge(0, 1, 1);
    graph.addEdge(2, 3, 2);
    auto mst = MinimumSpanningTree<int>::kruskal(graph);
    // Остов каждого связного компонента
    EXPECT_EQ(mst.getLength(), 2);
}

TEST(MinimumSpanningTreeTest, FullyConnectedGraph) {
    UndirectedGraph<int> graph(4);
    graph.addEdge(0, 1, 1);
    graph.addEdge(0, 2, 3);
    graph.addEdge(0, 3, 4);
    graph.addEdge(1, 2, 2);
    graph.addEdge(1, 3, 5);
    graph.addEdge(2, 3, 6);
    auto mst = MinimumSpanningTree<int>::kruskal(graph);
    EXPECT_EQ(mst.getLength(), 3);
    // Проверяем суммарный вес минимального остова
    int totalWeight = 0;
    for(int i = 0; i < mst.getLength(); ++i) {
        totalWeight += std::get<2>(mst[i]);
    }
    EXPECT_EQ(totalWeight, 1 + 2 + 4); // Итого 7
}

TEST(MinimumSpanningTreeTest, GraphWithMultipleMinimumSpanningTrees) {
    UndirectedGraph<int> graph(4);
    graph.addEdge(0, 1, 1);
    graph.addEdge(0, 2, 1);
    graph.addEdge(0, 3, 1);
    graph.addEdge(1, 2, 1);
    graph.addEdge(1, 3, 1);
    graph.addEdge(2, 3, 1);
    auto mst = MinimumSpanningTree<int>::kruskal(graph);
    EXPECT_EQ(mst.getLength(), 3);
    // Все веса должны быть 1
    for(int i = 0; i < mst.getLength(); ++i) {
        EXPECT_EQ(std::get<2>(mst[i]), 1);
    }
}

// Вспомогательная функция для сортировки компонентов
template<typename T>
void sortSCC(ArraySequence<ArraySequence<T>>& sccList) {
    // Сортируем вершины внутри каждой компоненты
    for(int i = 0; i < sccList.getLength(); ++i){
        // Копируем компоненты в std::vector для сортировки
        std::vector<T> temp;
        for(int j = 0; j < sccList[i].getLength(); ++j){
            temp.push_back(sccList[i][j]);
        }
        std::sort(temp.begin(), temp.end());
        // Обновляем компоненты
        for(int j = 0; j < temp.size(); ++j){
            sccList[i][j] = temp[j];
        }
    }

    // Сортируем список компонент по первому элементу каждой компоненты
    std::sort(sccList.begin(), sccList.end(), [&](const ArraySequence<T>& a, const ArraySequence<T>& b) -> bool {
        if(a.getLength() == 0) return true;
        if(b.getLength() == 0) return false;
        return a[0] < b[0];
    });
}

TEST(StronglyConnectedComponentsTest, EmptyGraph) {
    DirectedGraph<int> graph(0);
    auto scc = StronglyConnectedComponents<int>::findSCC(graph);
    EXPECT_EQ(scc.getLength(), 0);
}

TEST(StronglyConnectedComponentsTest, SingleVertex) {
    DirectedGraph<int> graph(1);
    auto scc = StronglyConnectedComponents<int>::findSCC(graph);
    EXPECT_EQ(scc.getLength(), 1);
    EXPECT_EQ(scc[0].getLength(), 1);
    EXPECT_EQ(scc[0][0], 0);
}

TEST(StronglyConnectedComponentsTest, TwoStronglyConnectedVertices) {
    DirectedGraph<int> graph(2);
    graph.addEdge(0, 1, 1);
    graph.addEdge(1, 0, 1);
    auto scc = StronglyConnectedComponents<int>::findSCC(graph);
    EXPECT_EQ(scc.getLength(), 1);
    EXPECT_EQ(scc[0].getLength(), 2);
    std::vector<int> expected = {0, 1};
    std::vector<int> actual = {scc[0][0], scc[0][1]};
    std::sort(actual.begin(), actual.end());
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(actual, expected);
}

TEST(StronglyConnectedComponentsTest, TwoDisconnectedVertices) {
    DirectedGraph<int> graph(2);
    graph.addEdge(0, 1, 1);
    auto scc = StronglyConnectedComponents<int>::findSCC(graph);
    EXPECT_EQ(scc.getLength(), 2);

    // Ожидаемые компоненты: {0}, {1}
    std::vector<std::vector<int>> expected = { {0}, {1} };
    std::vector<std::vector<int>> actual;

    for(int i = 0; i < scc.getLength(); ++i){
        std::vector<int> component = { scc[i][0] };
        actual.push_back(component);
    }

    // Сортируем для корректного сравнения
    std::sort(actual.begin(), actual.end(), [](const std::vector<int>& a, const std::vector<int>& b)-> bool{
        return a[0] < b[0];
    });

    EXPECT_EQ(actual, expected);
}

TEST(StronglyConnectedComponentsTest, ComplexGraph) {
    DirectedGraph<int> graph(5);
    graph.addEdge(0, 2, 1);
    graph.addEdge(2, 1, 1);
    graph.addEdge(1, 0, 1);
    graph.addEdge(0, 3, 1);
    graph.addEdge(3, 4, 1);
    auto scc = StronglyConnectedComponents<int>::findSCC(graph);
    EXPECT_EQ(scc.getLength(), 3);

    // Ожидаемые компоненты: {0,1,2}, {3}, {4}
    std::vector<std::vector<int>> expected = { {0,1,2}, {3}, {4} };
    std::vector<std::vector<int>> actual;

    for(int i = 0; i < scc.getLength(); ++i){
        std::vector<int> component;
        for(int j = 0; j < scc[i].getLength(); ++j){
            component.push_back(scc[i][j]);
        }
        std::sort(component.begin(), component.end());
        actual.push_back(component);
    }

    // Сортируем список компонент по первому элементу каждой компоненты
    std::sort(actual.begin(), actual.end(), [&](const std::vector<int>& a, const std::vector<int>& b)-> bool{
        return a[0] < b[0];
    });

    // Сортируем ожидаемые компоненты по аналогичному критерию
    std::sort(expected.begin(), expected.end(), [&](const std::vector<int>& a, const std::vector<int>& b)-> bool{
        return a[0] < b[0];
    });

    EXPECT_EQ(actual, expected);
}

TEST(StronglyConnectedComponentsTest, FullyConnectedGraph) {
    DirectedGraph<int> graph(3);
    graph.addEdge(0, 1, 1);
    graph.addEdge(1, 2, 1);
    graph.addEdge(2, 0, 1);
    auto scc = StronglyConnectedComponents<int>::findSCC(graph);
    EXPECT_EQ(scc.getLength(), 1);
    EXPECT_EQ(scc[0].getLength(), 3);
    std::vector<int> expected = {0, 1, 2};
    std::vector<int> actual = {scc[0][0], scc[0][1], scc[0][2]};
    std::sort(actual.begin(), actual.end());
    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(actual, expected);
}

// Вспомогательная функция для разделения ArraySequence<Pair<Weight, Vertex>> на отдельные массивы
template<typename Weight, typename Vertex>
std::pair<ArraySequence<Weight>, ArraySequence<Vertex>> splitResult(const ArraySequence<Pair<Weight, Vertex>>& result) {
    ArraySequence<Weight> distances;
    ArraySequence<Vertex> predecessors;

    for(int i = 0; i < result.getLength(); ++i){
        distances.append(result[i].first);
        predecessors.append(result[i].second);
    }

    return {distances, predecessors};
}
// Helper function to split results
template<typename T>
std::pair<ArraySequence<T>, ArraySequence<int>> splitResult(const ArraySequence<Pair<T, int>>& result) {
    ArraySequence<T> distances;
    ArraySequence<int> predecessors;
    for (int i = 0; i < result.getLength(); i++) {
        distances.append(result[i].first);
        predecessors.append(result[i].second);
    }
    return {distances, predecessors};
}

// Basic tests for ShortestPath
TEST(ShortestPathTest, SingleVertex) {
    DirectedGraph<int> graph(1);
    auto result = ShortestPath<int>::dijkstra(graph, 0);
    auto [distances, predecessors] = splitResult(result);

    EXPECT_EQ(distances.getLength(), 1);
    EXPECT_EQ(distances[0], 0);
    EXPECT_EQ(predecessors.getLength(), 1);
    EXPECT_EQ(predecessors[0], -1);
}

TEST(ShortestPathTest, SimpleGraph) {
    DirectedGraph<int> graph(3);
    graph.addEdge(0, 1, 1);
    graph.addEdge(1, 2, 2);
    auto result = ShortestPath<int>::dijkstra(graph, 0);
    auto [distances, predecessors] = splitResult(result);

    EXPECT_EQ(distances.getLength(), 3);
    EXPECT_EQ(distances[0], 0);
    EXPECT_EQ(distances[1], 1);
    EXPECT_EQ(distances[2], 3);

    EXPECT_EQ(predecessors.getLength(), 3);
    EXPECT_EQ(predecessors[0], -1);
    EXPECT_EQ(predecessors[1], 0);
    EXPECT_EQ(predecessors[2], 1);
}

TEST(ShortestPathTest, UnreachableVertex) {
    DirectedGraph<int> graph(3);
    graph.addEdge(0, 1, 1);
    auto result = ShortestPath<int>::dijkstra(graph, 0);
    auto [distances, predecessors] = splitResult(result);

    EXPECT_EQ(distances[2], std::numeric_limits<int>::max());
    EXPECT_EQ(predecessors[2], -1);
    EXPECT_THROW(ShortestPath<int>::getPath(result, 2), std::runtime_error);
}



TEST(DynamicWeightShortestPathTest, InvalidSource) {
    DirectedGraph<int> graph(3);
    DynamicWeightShortestPath<int> dsp(0.1);
    EXPECT_THROW(dsp.dijkstra(graph, 3), std::out_of_range);
}

TEST(DynamicWeightShortestPathTest, PathValidation) {
    DirectedGraph<int> graph(4);
    graph.addEdge(0, 1, 1);
    graph.addEdge(1, 2, 2);
    graph.addEdge(2, 3, 3);

    DynamicWeightShortestPath<int> dsp(0.1);
    auto result = dsp.dijkstra(graph, 0);

    EXPECT_NO_THROW(DynamicWeightShortestPath<int>::getPath(result, 3));
    EXPECT_THROW(DynamicWeightShortestPath<int>::getPath(result, 4), std::out_of_range);
}
// Тесты для GraphGenerator
TEST(GraphGenerator, GenerateCompleteGraph) {
    int vertices = 5;
    int maxWeight = 10;
    UndirectedGraph<int> completeGraph = GraphGenerator::generateUndirectedGraph(GraphGenerator::COMPLETE, vertices, 1.0, maxWeight);

    ASSERT_EQ(completeGraph.getVertexCount(), vertices);
    for (int i = 0; i < vertices; ++i) {
        ASSERT_EQ(completeGraph.getDegree(i), vertices - 1);
        for (int j = 0; j < vertices; ++j) {
            if (i != j) {
                ASSERT_TRUE(completeGraph.hasEdge(i, j));
                ASSERT_GE(completeGraph.getEdgeWeight(i, j), 1);
                ASSERT_LE(completeGraph.getEdgeWeight(i, j), maxWeight);
            }
        }
    }
}

TEST(GraphGenerator, GenerateSparseGraph) {
    int vertices = 10;
    int maxWeight = 20;
    UndirectedGraph<int> sparseGraph = GraphGenerator::generateUndirectedGraph(GraphGenerator::SPARSE, vertices, 0.1, maxWeight);

    ASSERT_EQ(sparseGraph.getVertexCount(), vertices);
    // В разрежённом графе степень каждой вершины должна быть минимальной (1)
    for (int i = 0; i < vertices; ++i) {
        ASSERT_GE(sparseGraph.getDegree(i), 1);
        ASSERT_LE(sparseGraph.getDegree(i), vertices - 1);
    }
}

TEST(GraphGenerator, GenerateCycleGraph) {
    int vertices = 4;
    int maxWeight = 15;
    UndirectedGraph<int> cycleGraph = GraphGenerator::generateUndirectedGraph(GraphGenerator::CYCLE, vertices, 1.0, maxWeight);

    ASSERT_EQ(cycleGraph.getVertexCount(), vertices);
    for (int i = 0; i < vertices; ++i) {
        ASSERT_EQ(cycleGraph.getDegree(i), 2);
        int next = (i + 1) % vertices;
        int prev = (i - 1 + vertices) % vertices;
        ASSERT_TRUE(cycleGraph.hasEdge(i, next));
        ASSERT_TRUE(cycleGraph.hasEdge(i, prev));
    }
}

TEST(GraphGenerator, GenerateDirectedGraph) {
    int vertices = 5;
    int maxWeight = 30;
    DirectedGraph<int> directedGraph = GraphGenerator::generateDirectedGraph(GraphGenerator::RANDOM, vertices, 0.5, maxWeight);

    ASSERT_EQ(directedGraph.getVertexCount(), vertices);
    for (int i = 0; i < vertices; ++i) {
        // Проверка диапазона веса рёбер
        ArraySequence<Pair<int, int>> neighbors = directedGraph.getNeighbors(i);
        for (int j = 0; j < neighbors.getLength(); ++j) {
            int weight = neighbors[j].second;
            ASSERT_GE(weight, 1);
            ASSERT_LE(weight, maxWeight);
        }
    }
}

// Тесты для Lattice
TEST(Lattice, ConstructorExplicit) {
    // Создание явного графа Хассе
    DirectedGraph<int> diagram(3);
    diagram.addEdge(0, 1, 1);
    diagram.addEdge(1, 2, 1);

    ArraySequence<int> elements;
    elements.append(1);
    elements.append(2);
    elements.append(3);

    Lattice<int> lattice(diagram, elements);

    ASSERT_TRUE(lattice.lessEqual(1, 2));
    ASSERT_TRUE(lattice.lessEqual(2, 3));
    ASSERT_TRUE(lattice.lessEqual(1, 3));
    ASSERT_FALSE(lattice.lessEqual(3, 1));
}

TEST(Lattice, ConstructorImplicit) {
    ArraySequence<int> elements;
    elements.append(1);
    elements.append(2);
    elements.append(3);
    elements.append(4);

    Lattice<int> lattice(elements,[](const int& a, const int& b) {
        return a <= b;
    });

    ASSERT_TRUE(lattice.lessEqual(1, 2));
    ASSERT_TRUE(lattice.lessEqual(2, 3));
    ASSERT_TRUE(lattice.lessEqual(1, 3));
    ASSERT_TRUE(lattice.lessEqual(3, 4));
    ASSERT_TRUE(lattice.lessEqual(1, 4));
    ASSERT_FALSE(lattice.lessEqual(4, 1));
}

TEST(Lattice, InvalidElements) {
    ArraySequence<int> elements;
    elements.append(1);
    elements.append(2);

    // Конструктор с неявным графом
    Lattice<int> lattice(elements, [](const int& a, const int& b) {
        return a <= b;
    });

    // Проверка исключений для несуществующих элементов
    ASSERT_THROW(lattice.lessEqual(1, 3), std::invalid_argument);

}

TEST(GraphGenerator, GenerateTree) {
    int vertices = 7;
    int maxWeight = 50;
    UndirectedGraph<int> treeGraph = GraphGenerator::generateUndirectedGraph(GraphGenerator::TREE, vertices, 0.0, maxWeight);

    ASSERT_EQ(treeGraph.getVertexCount(), vertices);
    // В дереве должно быть ровно vertices - 1 рёбер
    int edgeCount = 0;
    for (int i = 0; i < vertices; ++i) {
        edgeCount += treeGraph.getDegree(i);
    }
    ASSERT_EQ(edgeCount, 2 * (vertices - 1));

    // Проверка связности через DFS
    ArraySequence<bool> visited(vertices, false);
    treeGraph.dfs(0, visited, nullptr);
    for (int i = 0; i < vertices; ++i) {
        ASSERT_TRUE(visited.get(i));
    }
}