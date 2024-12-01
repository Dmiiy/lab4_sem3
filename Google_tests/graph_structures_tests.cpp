#include <queue>
#include "gtest/gtest.h"
#include "../include/graph_structures/Graph.h"
#include "../include/graph_structures/ConnectedComponents.h"
#include "../include/graph_structures/GraphColoring.h"
#include "../include/graph_structures/Lattice.h"
#include "../include/graph_structures/MinimumSpanningTree.h"
#include "../include/graph_structures/ShortestPath.h"
#include "../include/graph_structures/StronglyConnectedComponents.h"
#include "../include/graph_structures/TravelingSalesman.h"
#include "../include/graph_structures/GraphGenerator.h"



// Тестирование конструктора
TEST(GraphTest, Constructor) {
    Graph<int> graph(5);
    EXPECT_EQ(graph.getVertexCount(), 5);
    EXPECT_EQ(graph.getEdges().getLength(), 0);
}

// Тестирование добавления рёбер
TEST(GraphTest, AddEdge) {
    Graph<int> graph(3);
    graph.addEdge(0, 1, 10);
    EXPECT_TRUE(graph.hasEdge(0, 1));
    EXPECT_EQ(graph.getEdges().getLength(), 1);
    EXPECT_EQ(graph.getEdgeWeight(0, 1), 10);
}

// Тестирование добавления невалидного ребра
TEST(GraphTest, AddEdgeInvalid) {
    Graph<int> graph(2);
    EXPECT_THROW(graph.addEdge(-1, 1, 5), std::out_of_range);
    EXPECT_THROW(graph.addEdge(0, 2, 5), std::out_of_range);
}

// Тестирование добавления неориентированного рёбра
TEST(GraphTest, AddUndirectedEdge) {
    Graph<int> graph(3);
    graph.addUndirectedEdge(0, 1, 15);
    EXPECT_TRUE(graph.hasEdge(0, 1));
    EXPECT_TRUE(graph.hasEdge(1, 0));
    EXPECT_EQ(graph.getEdges().getLength(), 2);
    EXPECT_EQ(graph.getEdgeWeight(0, 1), 15);
    EXPECT_EQ(graph.getEdgeWeight(1, 0), 15);
}

// Тестирование удаления рёбер
TEST(GraphTest, RemoveEdge) {
    Graph<int> graph(3);
    graph.addEdge(0, 1, 20);
    EXPECT_TRUE(graph.hasEdge(0, 1));
    graph.removeEdge(0, 1);
    EXPECT_FALSE(graph.hasEdge(0, 1));
    EXPECT_EQ(graph.getEdges().getLength(), 0);
}

// Тестирование удаления несуществующего ребра
TEST(GraphTest, RemoveEdgeNotFound) {
    Graph<int> graph(2);
    EXPECT_THROW(graph.removeEdge(0, 1), std::invalid_argument);
}

// Тестирование степени вершины
TEST(GraphTest, GetDegree) {
    Graph<int> graph(4);
    graph.addEdge(0, 1, 5);
    graph.addEdge(0, 2, 10);
    graph.addUndirectedEdge(0, 3, 15);
    EXPECT_EQ(graph.getDegree(0), 4); // Рёбра: 0->1, 0->2, 0->3, 3->0
    EXPECT_EQ(graph.getDegree(1), 1);
    EXPECT_EQ(graph.getDegree(3), 2);
}

// Тестирование получения соседей вершины
TEST(GraphTest, GetNeighbors) {
    Graph<int> graph(3);
    graph.addEdge(0, 1, 7);
    graph.addEdge(0, 2, 14);
    graph.addUndirectedEdge(1, 2, 21);

    ArraySequence<Pair<int, int>> neighbors = graph.getNeighbors(0);
    EXPECT_EQ(neighbors.getLength(), 2);
    EXPECT_EQ(neighbors[0].first, 1);
    EXPECT_EQ(neighbors[0].second, 7);
    EXPECT_EQ(neighbors[1].first, 2);
    EXPECT_EQ(neighbors[1].second, 14);
}

// Тестирование получения веса рёбер
TEST(GraphTest, GetEdgeWeight) {
    Graph<int> graph(2);
    graph.addEdge(0, 1, 30);
    EXPECT_EQ(graph.getEdgeWeight(0, 1), 30);
    EXPECT_THROW(graph.getEdgeWeight(1, 0), std::invalid_argument);
}

// Тестирование очистки рёбер
TEST(GraphTest, ClearEdges) {
    Graph<int> graph(3);
    graph.addEdge(0, 1, 5);
    graph.addEdge(1, 2, 10);
    EXPECT_EQ(graph.getEdges().getLength(), 2);
    graph.clearEdges();
    EXPECT_EQ(graph.getEdges().getLength(), 0);
    EXPECT_FALSE(graph.hasEdge(0, 1));
    EXPECT_FALSE(graph.hasEdge(1, 2));
}

// Тестирование наличия рёбер
TEST(GraphTest, HasEdge) {
    Graph<int> graph(2);
    graph.addEdge(0, 1, 50);
    EXPECT_TRUE(graph.hasEdge(0, 1));
    EXPECT_FALSE(graph.hasEdge(1, 0));
}

// Тестирование копирования графа
TEST(GraphTest, CopyConstructor) {
    Graph<int> graph1(2);
    graph1.addEdge(0, 1, 100);

    Graph<int> graph2 = graph1; // Использование конструктора копирования
    EXPECT_EQ(graph2.getVertexCount(), 2);
    EXPECT_TRUE(graph2.hasEdge(0, 1));
    EXPECT_EQ(graph2.getEdgeWeight(0, 1), 100);
}

// Тестирование работы с комплексными типами
TEST(GraphTest, ComplexWeightType) {
    Graph<std::string> graph(2);
    graph.addEdge(0, 1, "EdgeA");
    EXPECT_TRUE(graph.hasEdge(0, 1));
    EXPECT_EQ(graph.getEdgeWeight(0, 1), "EdgeA");
}

// Тестирование добавления нескольких рёбер и их корректности
TEST(GraphTest, MultipleEdges) {
    Graph<int> graph(4);
    graph.addEdge(0, 1, 10);
    graph.addEdge(0, 2, 20);
    graph.addEdge(0, 3, 30);
    graph.addEdge(1, 2, 40);
    graph.addEdge(2, 3, 50);

    EXPECT_EQ(graph.getEdges().getLength(), 5);
    EXPECT_TRUE(graph.hasEdge(0, 1));
    EXPECT_TRUE(graph.hasEdge(0, 2));
    EXPECT_TRUE(graph.hasEdge(0, 3));
    EXPECT_TRUE(graph.hasEdge(1, 2));
    EXPECT_TRUE(graph.hasEdge(2, 3));

    EXPECT_EQ(graph.getEdgeWeight(0, 1), 10);
    EXPECT_EQ(graph.getEdgeWeight(0, 2), 20);
    EXPECT_EQ(graph.getEdgeWeight(0, 3), 30);
    EXPECT_EQ(graph.getEdgeWeight(1, 2), 40);
    EXPECT_EQ(graph.getEdgeWeight(2, 3), 50);
}


// Тестирование ConnectedComponents

// Тестирование поиска компонент связности в пустом графе
TEST(ConnectedComponentsTest, EmptyGraph) {
    Graph<int> graph(0);
    ArraySequence<ArraySequence<int>> components = ConnectedComponents<int>::findComponents(graph);
    EXPECT_EQ(components.getLength(), 0);
}

 //Тестирование поиска компонент связности в графе с одной вершиной
TEST(ConnectedComponentsTest, SingleVertex) {
    Graph<int> graph(1);
    auto components = ConnectedComponents<int>::findComponents(graph);
    EXPECT_EQ(components.getLength(), 1);
    ASSERT_EQ(components[0].getLength(), 1);
    //EXPECT_EQ((components[0])[0], 0);
}

// Тестирование поиска компонент связности в полностью связном графе
TEST(ConnectedComponentsTest, FullyConnectedGraph) {
    Graph<int> graph(4);
    graph.addEdge(0, 1, 1);
    graph.addEdge(1, 0, 1);
    graph.addEdge(0, 2, 1);
    graph.addEdge(2, 0, 1);
    graph.addEdge(0, 3, 1);
    graph.addEdge(3, 0, 1);
    graph.addEdge(1, 2, 1);
    graph.addEdge(2, 1, 1);
    graph.addEdge(1, 3, 1);
    graph.addEdge(3, 1, 1);
    graph.addEdge(2, 3, 1);
    graph.addEdge(3, 2, 1);

    ArraySequence<ArraySequence<int>> components = ConnectedComponents<int>::findComponents(graph);
    EXPECT_EQ(components.getLength(), 1);
    ASSERT_EQ(components[0].getLength(), 4);
    EXPECT_EQ(components[0][0], 0);
    EXPECT_EQ(components[0][1], 1);
    EXPECT_EQ(components[0][2], 2);
    EXPECT_EQ(components[0][3], 3);
}

// Тестирование поиска компонент связности в графе с несколькими компонентами
TEST(ConnectedComponentsTest, MultipleComponents) {
    Graph<int> graph(6);
    // Первая компонента: 0-1-2
    graph.addEdge(0, 1, 1);
    graph.addEdge(1, 0, 1);
    graph.addEdge(1, 2, 1);
    graph.addEdge(2, 1, 1);
    graph.addEdge(0, 2, 1);
    graph.addEdge(2, 0, 1);
    // Вторая компонента: 3-4
    graph.addEdge(3, 4, 1);
    graph.addEdge(4, 3, 1);
    // Третья компонента: 5

    ArraySequence<ArraySequence<int>> components = ConnectedComponents<int>::findComponents(graph);
    EXPECT_EQ(components.getLength(), 3);

    // Проверка первой компоненты
    ASSERT_EQ(components[0].getLength(), 3);
    EXPECT_EQ(components[0][0], 0);
    EXPECT_EQ(components[0][1], 1);
    EXPECT_EQ(components[0][2], 2);

    // Проверка второй компоненты
    ASSERT_EQ(components[1].getLength(), 2);
    EXPECT_EQ(components[1][0], 3);
    EXPECT_EQ(components[1][1], 4);

    // Проверка третьей компоненты
    ASSERT_EQ(components[2].getLength(), 1);
    EXPECT_EQ(components[2][0], 5);
}

// Тестирование ConnectedComponents с различными типами весов графа
TEST(ConnectedComponentsTest, ComplexWeightTypes) {
    Graph<std::string> graph(3);
    graph.addEdge(0, 1, "A");
    graph.addEdge(1, 0, "A");
    graph.addEdge(1, 2, "B");
    graph.addEdge(2, 1, "B");

    ArraySequence<ArraySequence<int>> components = ConnectedComponents<std::string>::findComponents(graph);
    EXPECT_EQ(components.getLength(), 1);
    ASSERT_EQ(components[0].getLength(), 3);
    EXPECT_EQ(components[0][0], 0);
    EXPECT_EQ(components[0][1], 1);
    EXPECT_EQ(components[0][2], 2);
}

// Тестирование GraphColoring

// Тестирование раскраски пустого графа
TEST(GraphColoringTest, EmptyGraph) {
    Graph<int> graph(0);
    ArraySequence<int> colors = GraphColoring::greedyColoring(graph);
    EXPECT_EQ(colors.getLength(), 0);
}

// Тестирование раскраски графа с одной вершиной
TEST(GraphColoringTest, SingleVertex) {
    Graph<int> graph(1);
    ArraySequence<int> colors = GraphColoring::greedyColoring(graph);
    ASSERT_EQ(colors.getLength(), 1);
    EXPECT_EQ(colors[0], 0);
}

// Тестирование раскраски линейного графа (цепочки)
TEST(GraphColoringTest, LinearGraph) {
    Graph<int> graph(4);
    graph.addEdge(0, 1, 1);
    graph.addEdge(1, 0, 1);
    graph.addEdge(1, 2, 1);
    graph.addEdge(2, 1, 1);
    graph.addEdge(2, 3, 1);
    graph.addEdge(3, 2, 1);

    ArraySequence<int> colors = GraphColoring::greedyColoring(graph);
    ASSERT_EQ(colors.getLength(), 4);
    EXPECT_EQ(colors[0], 0);
    EXPECT_EQ(colors[1], 1);
    EXPECT_EQ(colors[2], 0);
    EXPECT_EQ(colors[3], 1);
}

// Тестирование раскраски полного графа
TEST(GraphColoringTest, CompleteGraph) {
    Graph<int> graph(3);
    graph.addUndirectedEdge(0, 1, 1);
    graph.addUndirectedEdge(0, 2, 1);
    graph.addUndirectedEdge(1, 2, 1);

    ArraySequence<int> colors = GraphColoring::greedyColoring(graph);
    ASSERT_EQ(colors.getLength(), 3);
    // Каждый узел должен иметь уникальный цвет
    EXPECT_NE(colors[0], colors[1]);
    EXPECT_NE(colors[0], colors[2]);
    EXPECT_NE(colors[1], colors[2]);
}

// Тестирование раскраски графа с несколькими компонентами
TEST(GraphColoringTest, MultipleComponents) {
    Graph<int> graph(5);
    // Первая компонента: 0-1
    graph.addUndirectedEdge(0, 1, 1);
    // Вторая компонента: 2-3-4
    graph.addUndirectedEdge(2, 3, 1);
    graph.addUndirectedEdge(3, 4, 1);

    ArraySequence<int> colors = GraphColoring::greedyColoring(graph);
    ASSERT_EQ(colors.getLength(), 5);
    // Проверка, что соседние вершины имеют разные цвета
    EXPECT_NE(colors[0], colors[1]);
    EXPECT_NE(colors[2], colors[3]);
    EXPECT_NE(colors[3], colors[4]);
}

// Тестирование раскраски графа с циклом нечетной длины
TEST(GraphColoringTest, OddCycle) {
    Graph<int> graph(3);
    graph.addUndirectedEdge(0, 1, 1);
    graph.addUndirectedEdge(1, 2, 1);
    graph.addUndirectedEdge(2, 0, 1);

    ArraySequence<int> colors = GraphColoring::greedyColoring(graph);
    ASSERT_EQ(colors.getLength(), 3);
    // В треугольнике потребуются 3 цвета
    EXPECT_EQ(colors[0], 0);
    EXPECT_EQ(colors[1], 1);
    EXPECT_EQ(colors[2], 2);
}

// Тестирование раскраски графа с несколькими вершинами и различными связями
TEST(GraphColoringTest, ComplexGraph) {
    Graph<int> graph(6);
    graph.addUndirectedEdge(0, 1, 1);
    graph.addUndirectedEdge(0, 2, 1);
    graph.addUndirectedEdge(1, 2, 1);
    graph.addUndirectedEdge(1, 3, 1);
    graph.addUndirectedEdge(2, 3, 1);
    graph.addUndirectedEdge(3, 4, 1);
    graph.addUndirectedEdge(4, 5, 1);

    ArraySequence<int> colors = GraphColoring::greedyColoring(graph);
    ASSERT_EQ(colors.getLength(), 6);
    // Проверка, что соседние вершины имеют разные цвета
    EXPECT_NE(colors[0], colors[1]);
    EXPECT_NE(colors[0], colors[2]);
    EXPECT_NE(colors[1], colors[2]);
    EXPECT_NE(colors[1], colors[3]);
    EXPECT_NE(colors[2], colors[3]);
    EXPECT_NE(colors[3], colors[4]);
    EXPECT_NE(colors[4], colors[5]);
}

// Тестирование раскраски графа с комплексными связями
TEST(GraphColoringTest, GraphWithMultipleEdges) {
    Graph<int> graph(4);
    graph.addUndirectedEdge(0, 1, 1);
    graph.addUndirectedEdge(0, 1, 2); // Множественные ребра
    graph.addUndirectedEdge(1, 2, 1);
    graph.addUndirectedEdge(2, 3, 1);
    graph.addUndirectedEdge(3, 0, 1);

    ArraySequence<int> colors = GraphColoring::greedyColoring(graph);
    ASSERT_EQ(colors.getLength(), 4);
    // Проверка, что соседние вершины имеют разные цвета
    EXPECT_NE(colors[0], colors[1]);
    EXPECT_NE(colors[1], colors[2]);
    EXPECT_NE(colors[2], colors[3]);
    EXPECT_NE(colors[3], colors[0]);
}
TEST(LatticeTest, MeetOperationExplicit) {
    // Создаем явную диаграмму Хассе
    Graph<int> graph(5);
    graph.addEdge(0, 2, 1); // 0 < 2
    graph.addEdge(1, 2, 1); // 1 < 2
    graph.addEdge(2, 3, 1); // 2 < 3
    graph.addEdge(2, 4, 1); // 2 < 4

    ArraySequence<int> elements;
    elements.append(0);
    elements.append(1);
    elements.append(2);
    elements.append(3);
    elements.append(4);

    Lattice<int> lattice(graph, elements);

    // Тестируем операцию meet
    auto meet01 = lattice.meet(0, 1);
    EXPECT_FALSE(meet01.has_value()); // Нет общей нижней грани

    auto meet02 = lattice.meet(0, 2);
    ASSERT_TRUE(meet02.has_value());
    EXPECT_EQ(meet02.value(), 0);

    auto meet13 = lattice.meet(1, 3);
    ASSERT_TRUE(meet13.has_value());
    EXPECT_EQ(meet13.value(), 1);  // Исправлено ожидаемое значение
}


TEST(LatticeTest, JoinOperationExplicit) {
    // Создаем явную диаграмму Хассе
    Graph<int> graph(5);
    graph.addEdge(0, 2, 1); // 0 < 2
    graph.addEdge(1, 2, 1); // 1 < 2
    graph.addEdge(2, 3, 1); // 2 < 3
    graph.addEdge(2, 4, 1); // 2 < 4

    ArraySequence<int> elements;
    elements.append(0);
    elements.append(1);
    elements.append(2);
    elements.append(3);
    elements.append(4);

    Lattice<int> lattice(graph, elements);

    // Тестируем операцию join
    auto join34 = lattice.join(3, 4);
    EXPECT_FALSE(join34.has_value()); // Нет общей верхней грани

    auto join23 = lattice.join(2, 3);
    ASSERT_TRUE(join23.has_value());
    EXPECT_EQ(join23.value(), 3);

    auto join01 = lattice.join(0, 1);
    ASSERT_TRUE(join01.has_value());
    EXPECT_EQ(join01.value(), 2);
}

TEST(LatticeTest, MeetOperationImplicit) {
    // Создаем неявную диаграмму Хассе с отношением делимости
    ArraySequence<int> elements;
    elements.append(1);
    elements.append(2);
    elements.append(3);
    elements.append(6);

    auto relation = [](int a, int b) { return (a % b == 0); };

    Lattice<int> lattice(elements, relation);

    // Тестируем операцию meet
    auto meet23 = lattice.meet(2, 3);
    ASSERT_TRUE(meet23.has_value());
    EXPECT_EQ(meet23.value(), 6);

    auto meet26 = lattice.meet(2, 6);
    ASSERT_TRUE(meet26.has_value());
    EXPECT_EQ(meet26.value(), 6);

    auto meet36 = lattice.meet(3, 6);
    ASSERT_TRUE(meet36.has_value());
    EXPECT_EQ(meet36.value(), 6);
}

TEST(LatticeTest, JoinOperationImplicit) {
    // Создаем неявную диаграмму Хассе с отношением делимости
    ArraySequence<int> elements;
    elements.append(1);
    elements.append(2);
    elements.append(3);
    elements.append(6);

    // Правильное отношение: a делит b
    auto relation = [](int a, int b) { return (b % a == 0); };

    Lattice<int> lattice(elements, relation);

    // Тестируем операцию join
    auto join12 = lattice.join(1, 2);
    ASSERT_TRUE(join12.has_value());
    EXPECT_EQ(join12.value(), 2);

    auto join23 = lattice.join(2, 3);
    ASSERT_TRUE(join23.has_value());
    EXPECT_EQ(join23.value(), 6);

    auto join16 = lattice.join(1, 6);
    ASSERT_TRUE(join16.has_value());
    EXPECT_EQ(join16.value(), 6);
}
// TEST for MinimumSpanningTree
TEST(MinimumSpanningTreeTest, KruskalBasic) {
    Graph<int> graph(4);
    graph.addEdge(0, 1, 1);
    graph.addEdge(0, 2, 4);
    graph.addEdge(1, 2, 2);
    graph.addEdge(1, 3, 5);
    graph.addEdge(2, 3, 3);

    ArraySequence<std::tuple<int, int, int>> mst = MinimumSpanningTree<int>::kruskal(4, graph);

    EXPECT_EQ(mst.getLength(), 3);
    // Check edges in MST
    EXPECT_EQ(std::get<0>(mst[0]), 0);
    EXPECT_EQ(std::get<1>(mst[0]), 1);
    EXPECT_EQ(std::get<2>(mst[0]), 1);

    EXPECT_EQ(std::get<0>(mst[1]), 1);
    EXPECT_EQ(std::get<1>(mst[1]), 2);
    EXPECT_EQ(std::get<2>(mst[1]), 2);

    EXPECT_EQ(std::get<0>(mst[2]), 2);
    EXPECT_EQ(std::get<1>(mst[2]), 3);
    EXPECT_EQ(std::get<2>(mst[2]), 3);
}

TEST(MinimumSpanningTreeTest, KruskalDisconnectedGraph) {
    Graph<int> graph(5);
    graph.addEdge(0, 1, 1);
    graph.addEdge(2, 3, 2);

    ArraySequence<std::tuple<int, int, int>> mst = MinimumSpanningTree<int>::kruskal(5, graph);

    EXPECT_EQ(mst.getLength(), 2);
    // Check edges in MST
    EXPECT_EQ(std::get<0>(mst[0]), 0);
    EXPECT_EQ(std::get<1>(mst[0]), 1);
    EXPECT_EQ(std::get<2>(mst[0]), 1);

    EXPECT_EQ(std::get<0>(mst[1]), 2);
    EXPECT_EQ(std::get<1>(mst[1]), 3);
    EXPECT_EQ(std::get<2>(mst[1]), 2);
}

TEST(MinimumSpanningTreeTest, KruskalSingleVertex) {
    Graph<int> graph(1);
    ArraySequence<std::tuple<int, int, int>> mst = MinimumSpanningTree<int>::kruskal(1, graph);
    EXPECT_EQ(mst.getLength(), 0);
}

TEST(MinimumSpanningTreeTest, KruskalEmptyGraph) {
    Graph<int> graph(0);
    ArraySequence<std::tuple<int, int, int>> mst = MinimumSpanningTree<int>::kruskal(0, graph);
    EXPECT_EQ(mst.getLength(), 0);
}

// TEST for ShortestPath
TEST(ShortestPathTest, DijkstraBasic) {
    Graph<double> graph(5);
    graph.addEdge(0, 1, 10);
    graph.addEdge(0, 3, 5);
    graph.addEdge(1, 2, 1);
    graph.addEdge(3, 1, 3);
    graph.addEdge(3, 2, 9);
    graph.addEdge(3, 4, 2);
    graph.addEdge(4, 2, 2);
    graph.addEdge(4, 1, 4);

    auto result = ShortestPath::dijkstra(graph, 0);
    ArraySequence<double> distances = result.first;
    ArraySequence<int> predecessors = result.second;

    EXPECT_EQ(distances.getLength(), 5);
    EXPECT_EQ(distances[0], 0);
    EXPECT_EQ(distances[1], 8);
    EXPECT_EQ(distances[2], 9);
    EXPECT_EQ(distances[3], 5);
    EXPECT_EQ(distances[4], 7);

    EXPECT_EQ(predecessors[0], -1);
    EXPECT_EQ(predecessors[1], 3);
    EXPECT_EQ(predecessors[2], 1);
    EXPECT_EQ(predecessors[3], 0);
    EXPECT_EQ(predecessors[4], 3);
}

TEST(ShortestPathTest, DijkstraDisconnectedGraph) {
    Graph<double> graph(4);
    graph.addEdge(0, 1, 1);
    graph.addEdge(2, 3, 1);

    auto result = ShortestPath::dijkstra(graph, 0);
    ArraySequence<double> distances = result.first;

    EXPECT_EQ(distances.getLength(), 4);
    EXPECT_EQ(distances[0], 0);
    EXPECT_EQ(distances[1], 1);
    EXPECT_EQ(distances[2], std::numeric_limits<double>::infinity());
    EXPECT_EQ(distances[3], std::numeric_limits<double>::infinity());
}

TEST(ShortestPathTest, DijkstraSingleVertex) {
    Graph<double> graph(1);
    auto result = ShortestPath::dijkstra(graph, 0);
    ArraySequence<double> distances = result.first;

    EXPECT_EQ(distances.getLength(), 1);
    EXPECT_EQ(distances[0], 0);
}

TEST(ShortestPathTest, DijkstraEmptyGraph) {
    Graph<double> graph(0);
    // Expect no exception, but distances should be empty
    auto result = ShortestPath::dijkstra(graph, 0);
    ArraySequence<double> distances = result.first;

    EXPECT_EQ(distances.getLength(), 0);
}

// TEST for StronglyConnectedComponents
TEST(StronglyConnectedComponentsTest, FindSCCBasic) {
    // Creating a graph with 5 vertices
    Graph<int> graph(5);
    graph.addEdge(0, 2, 1);
    graph.addEdge(2, 1, 1);
    graph.addEdge(1, 0, 1);
    graph.addEdge(0, 3, 1);
    graph.addEdge(3, 4, 1);

    ArraySequence<ArraySequence<int>> scc = StronglyConnectedComponents<int>::findSCC(graph);

    EXPECT_EQ(scc.getLength(), 3);

    // First SCC
    EXPECT_EQ(scc[0].getLength(), 3);
    EXPECT_TRUE(scc[0].find(0));
    EXPECT_TRUE(scc[0].find(1));
    EXPECT_TRUE(scc[0].find(2));

    // Second SCC
    EXPECT_EQ(scc[1].getLength(), 1);
    EXPECT_TRUE(scc[1].find(3));

    // Third SCC
    EXPECT_EQ(scc[2].getLength(), 1);
    EXPECT_TRUE(scc[2].find(4));
}

TEST(StronglyConnectedComponentsTest, FindSCCSingleVertex) {
    Graph<int> graph(1);
    ArraySequence<ArraySequence<int>> scc = StronglyConnectedComponents<int>::findSCC(graph);

    EXPECT_EQ(scc.getLength(), 1);
    EXPECT_EQ(scc[0].getLength(), 1);
    EXPECT_TRUE(scc[0].find(0));
}

TEST(StronglyConnectedComponentsTest, FindSCCCycle) {
    Graph<int> graph(3);
    graph.addEdge(0, 1, 1);
    graph.addEdge(1, 2, 1);
    graph.addEdge(2, 0, 1);

    ArraySequence<ArraySequence<int>> scc = StronglyConnectedComponents<int>::findSCC(graph);

    EXPECT_EQ(scc.getLength(), 1);
    EXPECT_EQ(scc[0].getLength(), 3);
    EXPECT_TRUE(scc[0].find(0));
    EXPECT_TRUE(scc[0].find(1));
    EXPECT_TRUE(scc[0].find(2));
}



TEST(TravelingSalesmanTest, BasicTest) {
    // Создаем граф с 4 вершинами
    Graph<int> graph(4);

    // Добавляем ребра между вершинами с указанными весами (неориентированный граф)
    graph.addEdge(0, 1, 10);
    graph.addEdge(1, 0, 10); // Обратное ребро

    graph.addEdge(1, 2, 15);
    graph.addEdge(2, 1, 15); // Обратное ребро

    graph.addEdge(2, 3, 20);
    graph.addEdge(3, 2, 20); // Обратное ребро

    graph.addEdge(3, 0, 25);
    graph.addEdge(0, 3, 25); // Обратное ребро

    graph.addEdge(0, 2, 35);
    graph.addEdge(2, 0, 35); // Обратное ребро

    graph.addEdge(1, 3, 30);
    graph.addEdge(3, 1, 30); // Обратное ребро

    // Обязательные для посещения вершины
    ArraySequence<int> mandatoryVertices;
    mandatoryVertices.append(0);
    mandatoryVertices.append(1);
    mandatoryVertices.append(2);
    mandatoryVertices.append(3);

    // Создаем объект TravelingSalesman
    TravelingSalesman<int> tsp(graph, mandatoryVertices);

    // Вызываем метод solve()
    auto result = tsp.solve();

    // Ожидаемый путь и стоимость
    ArraySequence<int> expectedPath;
    expectedPath.append(0);
    expectedPath.append(1);
    expectedPath.append(2);
    expectedPath.append(3);
    expectedPath.append(0);

    int expectedCost = 10 + 15 + 20 + 25; // 70

    // Проверяем полученный путь и стоимость
    ASSERT_EQ(result.first.getLength(), expectedPath.getLength());
    for (int i = 0; i < expectedPath.getLength(); ++i) {
        EXPECT_EQ(result.first.get(i), expectedPath.get(i));
    }
    EXPECT_EQ(result.second, expectedCost);
}

TEST(TravelingSalesmanTest, AsymmetricGraphTest) {
    // Создаем граф с 5 вершинами (асимметричный)
    Graph<int> graph(5);

    // Добавляем ребра между вершинами с разными весами (асимметричный граф)
    graph.addEdge(0, 1, 2);
    // Не добавляем обратное ребро 1 -> 0

    graph.addEdge(1, 2, 4);
    // Не добавляем обратное ребро 2 -> 1

    graph.addEdge(2, 3, 5);
    // Не добавляем обратное ребро 3 -> 2

    graph.addEdge(3, 4, 7);
    // Не добавляем обратное ребро 4 -> 3

    graph.addEdge(4, 0, 6);
    // Не добавляем обратное ребро 0 -> 4

    // Обязательные для посещения вершины
    ArraySequence<int> mandatoryVertices;
    mandatoryVertices.append(0);
    mandatoryVertices.append(2);
    mandatoryVertices.append(4);

    // Создаем объект TravelingSalesman
    TravelingSalesman<int> tsp(graph, mandatoryVertices);

    // Вызываем метод solve()
    auto result = tsp.solve();

    // Ожидаемая стоимость:
    // Путь: 0 -> 1 -> 2 -> 3 -> 4 -> 0
    // Стоимость: 2 + 4 + 5 + 7 + 6 = 24
    int expectedCost = 24;

    // Проверяем стоимость
    EXPECT_EQ(result.second, expectedCost);
}

TEST(TravelingSalesmanTest, NoPathTest) {
    // Создаем граф с 3 вершинами
    Graph<int> graph(3);

    // Добавляем ребра, граф несвязный
    graph.addEdge(0, 1, 5);
    graph.addEdge(1, 0, 5);
    // Вершина 2 не соединена

    // Обязательные для посещения вершины
    ArraySequence<int> mandatoryVertices;
    mandatoryVertices.append(0);
    mandatoryVertices.append(1);
    mandatoryVertices.append(2); // Вершина 2 несвязана

    // Создаем объект TravelingSalesman
    TravelingSalesman<int> tsp(graph, mandatoryVertices);

    // Вызываем метод solve()
    auto result = tsp.solve();

    // Ожидаемое значение стоимости (нет пути, поэтому максимальное значение)
    int expectedCost = std::numeric_limits<int>::max();

    // Проверяем стоимость
    EXPECT_EQ(result.second, expectedCost);
    // Проверяем, что путь содержит только начальную вершину (нет полного обхода)
    ASSERT_EQ(result.first.getLength(), 1);
    EXPECT_EQ(result.first.get(0), 0);
}

TEST(TravelingSalesmanTest, LargeGraphTest) {
    // Создаем граф с 5 вершинами
    Graph<int> graph(5);

    // Добавляем ребра между вершинами (неориентированный граф)
    graph.addEdge(0, 1, 2);
    graph.addEdge(1, 0, 2); // Обратное ребро

    graph.addEdge(0, 2, 9);
    graph.addEdge(2, 0, 9); // Обратное ребро

    graph.addEdge(0, 3, 10);
    graph.addEdge(3, 0, 10); // Обратное ребро

    graph.addEdge(0, 4, 15);
    graph.addEdge(4, 0, 15); // Обратное ребро

    graph.addEdge(1, 2, 6);
    graph.addEdge(2, 1, 6); // Обратное ребро

    graph.addEdge(1, 3, 4);
    graph.addEdge(3, 1, 4); // Обратное ребро

    graph.addEdge(1, 4, 8);
    graph.addEdge(4, 1, 8); // Обратное ребро

    graph.addEdge(2, 3, 3);
    graph.addEdge(3, 2, 3); // Обратное ребро

    graph.addEdge(2, 4, 12);
    graph.addEdge(4, 2, 12); // Обратное ребро

    graph.addEdge(3, 4, 14);
    graph.addEdge(4, 3, 14); // Обратное ребро

    // Обязательные для посещения вершины
    ArraySequence<int> mandatoryVertices;
    mandatoryVertices.append(0);
    mandatoryVertices.append(1);
    mandatoryVertices.append(2);
    mandatoryVertices.append(3);
    mandatoryVertices.append(4);

    // Создаем объект TravelingSalesman
    TravelingSalesman<int> tsp(graph, mandatoryVertices);

    // Вызываем метод solve()
    auto result = tsp.solve();

    int expectedCost = 35;

    // Проверяем стоимость
    EXPECT_EQ(result.second, expectedCost);
}

TEST(GraphGeneratorTest, GenerateCompleteGraph) {
    int vertices = 5;
    int maxWeight = 100;

    // Генерация полного графа
    Graph<int> graph = GraphGenerator::generateCompleteGraph(vertices, maxWeight);

    // Проверка количества вершин
    EXPECT_EQ(graph.getVertexCount(), vertices);

    // В полном неориентированном графе количество рёбер должно быть n*(n-1)/2
    int expectedEdges = vertices * (vertices - 1) / 2;
    // Поскольку addUndirectedEdge добавляет оба направления, общее количество рёбер в списке должно быть 2 * expectedEdges
    EXPECT_EQ(graph.getEdges().getLength(), expectedEdges * 2) << "Complete graph should have " << expectedEdges * 2 << " edges.";

    // Проверка наличия всех возможных рёбер
    for (int i = 0; i < vertices; ++i) {
        for (int j = i + 1; j < vertices; ++j) {
            EXPECT_TRUE(graph.hasEdge(i, j)) << "Edge (" << i << ", " << j << ") should exist.";
            EXPECT_TRUE(graph.hasEdge(j, i)) << "Edge (" << j << ", " << i << ") should exist.";
        }
    }

    // Проверка диапазона весов рёбер
    for (int i = 0; i < graph.getEdges().getLength(); ++i) {
        const std::tuple<int, int, int>& edge = graph.getEdges().get(i);
        EXPECT_GE(std::get<2>(edge), 1) << "Edge weight should be at least 1.";
        EXPECT_LE(std::get<2>(edge), maxWeight) << "Edge weight should not exceed maxWeight.";
    }
}

/**
 * @brief Тестирование генерации разреженного графа (дерева).
 */
TEST(GraphGeneratorTest, GenerateSparseGraph) {
    int vertices = 10;
    int maxWeight = 50;

    // Генерация разреженного графа
    Graph<int> graph = GraphGenerator::generateSparseGraph(vertices, maxWeight);

    // Проверка количества вершин
    EXPECT_EQ(graph.getVertexCount(), vertices);

    // В дереве количество рёбер должно быть n-1, а addUndirectedEdge добавляет два направления
    int expectedEdges = vertices - 1;
    EXPECT_EQ(graph.getEdges().getLength(), expectedEdges * 2) << "Sparse graph should have " << expectedEdges * 2 << " edges.";

    // Проверка связности графа с помощью обхода в ширину (BFS)
    ArraySequence<bool> visited;
    for(int i = 0; i < vertices; ++i) {
        visited.append(false);
    }

    std::queue<int> q;
    q.push(0);
    visited[0] = true;

    while(!q.empty()) {
        int current = q.front();
        q.pop();

        ArraySequence<Pair<int, int>> neighbors = graph.getNeighbors(current);
        for(int i = 0; i < neighbors.getLength(); ++i) {
            int neighbor = neighbors.get(i).first;
            if(!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }

    // Проверяем, что все вершины посещены
    for(int i = 0; i < vertices; ++i) {
        EXPECT_TRUE(visited[i]) << "Vertex " << i << " should be connected.";
    }
}

/**
 * @brief Тестирование генерации случайного графа с заданной плотностью.
 */
TEST(GraphGeneratorTest, GenerateRandomGraph) {
    int vertices = 6;
    double density = 0.5; // Для n=6, maxEdges=15; ожидаемое количество рёбер = 7
    int maxWeight = 100;

    // Генерация случайного графа
    Graph<int> graph = GraphGenerator::generateRandomGraph(vertices, density, maxWeight);

    // Проверка количества вершин
    EXPECT_EQ(graph.getVertexCount(), vertices);

    // Вычисление ожидаемого количества рёбер
    int maxEdges = vertices * (vertices - 1) / 2;
    int expectedEdges = static_cast<int>(density * maxEdges);

    // В addUndirectedEdge добавляются два направления
    EXPECT_EQ(graph.getEdges().getLength(), expectedEdges * 2) << "Random graph should have " << expectedEdges * 2 << " edges.";

    // Проверка диапазона весов рёбер
    for (int i = 0; i < graph.getEdges().getLength(); ++i) {
        const std::tuple<int, int, int>& edge = graph.getEdges().get(i);
        EXPECT_GE(std::get<2>(edge), 1) << "Edge weight should be at least 1.";
        EXPECT_LE(std::get<2>(edge), maxWeight) << "Edge weight should not exceed maxWeight.";
    }
}

/**
 * @brief Тестирование генерации случайного графа с некорректной плотностью.
 */
TEST(GraphGeneratorTest, GenerateRandomGraphInvalidDensity) {
    int vertices = 5;
    int maxWeight = 100;

    // Плотность меньше 0.0
    double invalidDensityLow = -0.1;
    EXPECT_THROW(GraphGenerator::generateRandomGraph(vertices, invalidDensityLow, maxWeight), std::invalid_argument);

    // Плотность больше 1.0
    double invalidDensityHigh = 1.1;
    EXPECT_THROW(GraphGenerator::generateRandomGraph(vertices, invalidDensityHigh, maxWeight), std::invalid_argument);
}

/**
 * @brief Тестирование генерации полного графа с некорректным количеством вершин.
 */
TEST(GraphGeneratorTest, GenerateCompleteGraphInvalidVertices) {
    int invalidVertices = -3;
    int maxWeight = 100;

    EXPECT_THROW(GraphGenerator::generateCompleteGraph(invalidVertices, maxWeight), std::invalid_argument);
}

/**
 * @brief Тестирование генерации разреженного графа с некорректным количеством вершин.
 */
TEST(GraphGeneratorTest, GenerateSparseGraphInvalidVertices) {
    int invalidVertices = -1;
    int maxWeight = 100;

    EXPECT_THROW(GraphGenerator::generateSparseGraph(invalidVertices, maxWeight), std::invalid_argument);
}

/**
 * @brief Тестирование генерации случайного графа с нулевым количеством вершин.
 */
TEST(GraphGeneratorTest, GenerateRandomGraphZeroVertices) {
    int vertices = 0;
    double density = 0.5;
    int maxWeight = 100;

    EXPECT_THROW(GraphGenerator::generateRandomGraph(vertices, density, maxWeight), std::invalid_argument);
}

/**
 * @brief Тестирование, что сгенерированный разреженный граф содержит именно n-1 рёбер.
 */
TEST(GraphGeneratorTest, GenerateSparseGraphEdgeCount) {
    int vertices = 10;
    int maxWeight = 50;

    // Генерация разреженного графа
    Graph<int> graph = GraphGenerator::generateSparseGraph(vertices, maxWeight);

    // В дереве количество рёбер должно быть n-1, а addUndirectedEdge добавляет два направления
    int expectedEdges = vertices - 1;
    EXPECT_EQ(graph.getEdges().getLength(), expectedEdges * 2) << "Sparse graph should have " << expectedEdges * 2 << " edges.";
}

/**
 * @brief Тестирование генерации случайного графа с плотностью 1.0 (должно быть полным графом).
 */
TEST(GraphGeneratorTest, GenerateRandomGraphFullDensity) {
    int vertices = 4;
    double density = 1.0;
    int maxWeight = 100;

    // Генерация случайного графа с плотностью 1.0
    Graph<int> graph = GraphGenerator::generateRandomGraph(vertices, density, maxWeight);

    // Проверка, что граф является полным
    int expectedEdges = vertices * (vertices - 1) / 2;
    EXPECT_EQ(graph.getEdges().getLength(), expectedEdges * 2) << "Random graph with full density should have " << expectedEdges * 2 << " edges.";

    // Проверка наличия всех возможных рёбер
    for (int i = 0; i < vertices; ++i) {
        for (int j = i + 1; j < vertices; ++j) {
            EXPECT_TRUE(graph.hasEdge(i, j)) << "Edge (" << i << ", " << j << ") should exist.";
            EXPECT_TRUE(graph.hasEdge(j, i)) << "Edge (" << j << ", " << i << ") should exist.";
        }
    }
}
