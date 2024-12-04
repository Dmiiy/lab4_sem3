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
    EXPECT_TRUE(scc[0].find(0));
    EXPECT_TRUE(scc[0].find(1));
}

TEST(StronglyConnectedComponentsTest, TwoDisconnectedVertices) {
    DirectedGraph<int> graph(2);
    graph.addEdge(0, 1, 1);
    auto scc = StronglyConnectedComponents<int>::findSCC(graph);
    EXPECT_EQ(scc.getLength(), 2);
    EXPECT_EQ(scc[0].getLength(), 1);
    EXPECT_EQ(scc[1].getLength(), 1);
    EXPECT_TRUE(scc[0].find(0));
    EXPECT_TRUE(scc[1].find(1));
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

    // Проверяем наличие ожидаемых компонент
    bool foundSCC0 = false, foundSCC1 = false, foundSCC2 = false;
    for(int i = 0; i < scc.getLength(); ++i){
        if(scc[i].getLength() == 3){
            EXPECT_TRUE(scc[i].find(0));
            EXPECT_TRUE(scc[i].find(1));
            EXPECT_TRUE(scc[i].find(2));
            foundSCC0 = true;
        }
        if(scc[i].getLength() == 1){
            EXPECT_TRUE(scc[i].find(3) || scc[i].find(4));
            foundSCC1 = true;
            foundSCC2 = true;
        }
    }
    EXPECT_TRUE(foundSCC0);
    EXPECT_TRUE(foundSCC1);
    EXPECT_TRUE(foundSCC2);
}

TEST(StronglyConnectedComponentsTest, FullyConnectedGraph) {
    DirectedGraph<int> graph(3);
    graph.addEdge(0, 1, 1);
    graph.addEdge(1, 2, 1);
    graph.addEdge(2, 0, 1);
    auto scc = StronglyConnectedComponents<int>::findSCC(graph);
    EXPECT_EQ(scc.getLength(), 1);
    EXPECT_EQ(scc[0].getLength(), 3);
    EXPECT_TRUE(scc[0].find(0));
    EXPECT_TRUE(scc[0].find(1));
    EXPECT_TRUE(scc[0].find(2));
}

// Тестирование явной диаграммы Хассе на пустом графе
TEST(LatticeTest, ExplicitHasseDiagramEmpty) {
    DirectedGraph<int> diagram(0);
    ArraySequence<int> elements;
    Lattice<int> lattice(diagram, elements);
    EXPECT_EQ(lattice.getHasseDiagram().getVertexCount(), 0);
}

// Тестирование явной диаграммы Хассе с одной вершиной
TEST(LatticeTest, ExplicitHasseDiagramSingleElement) {
    DirectedGraph<int> diagram(1);
    ArraySequence<int> elements;
    elements.append(1);
    Lattice<int> lattice(diagram, elements);
    EXPECT_EQ(lattice.getHasseDiagram().getVertexCount(), 1);
    EXPECT_FALSE(lattice.getHasseDiagram().hasEdge(0, 0)); // Нет циклов

    // Проверка отношения
    EXPECT_TRUE(lattice.lessEqual(1,1));
}

// Тестирование явной диаграммы Хассе с двумя связанными вершинами
TEST(LatticeTest, ExplicitHasseDiagramTwoElements) {
    DirectedGraph<int> diagram(2);
    diagram.addEdge(0,1,1); // 1 -> 2
    ArraySequence<int> elements = {1, 2};
    Lattice<int> lattice(diagram, elements);

    EXPECT_EQ(lattice.getHasseDiagram().getVertexCount(),2);
    EXPECT_TRUE(lattice.getHasseDiagram().hasEdge(0,1));

    // Проверка отношений
    EXPECT_TRUE(lattice.lessEqual(1,2));
    EXPECT_FALSE(lattice.lessEqual(2,1));
}

// Тестирование неявной диаграммы Хассе на пустом графе
TEST(LatticeTest, ImplicitHasseDiagramEmpty) {
    ArraySequence<int> elements;
    auto relation = [](const int& a, const int& b) -> bool { return a <= b; };
    Lattice<int> lattice(elements, relation);
    EXPECT_EQ(lattice.getHasseDiagram().getVertexCount(), 0);
}

// Тестирование неявной диаграммы Хассе с одной вершиной
TEST(LatticeTest, ImplicitHasseDiagramSingleElement) {
    ArraySequence<int> elements;
    elements.append(1);
    auto relation = [](const int& a, const int& b) -> bool { return a <= b; };
    Lattice<int> lattice(elements, relation);
    EXPECT_EQ(lattice.getHasseDiagram().getVertexCount(),1);

    // Проверка отношения
    EXPECT_TRUE(lattice.lessEqual(1,1));
}

// Тестирование неявной диаграммы Хассе с несколькими элементами
TEST(LatticeTest, ImplicitHasseDiagramMultipleElements) {
    ArraySequence<int> elements;
    elements.append(1);
    elements.append(2);
    elements.append(3);
    elements.append(4);
    auto relation = [](const int& a, const int& b) -> bool { return a <= b; };
    Lattice<int> lattice(elements, relation);
    EXPECT_EQ(lattice.getHasseDiagram().getVertexCount(),4);

    // Ожидаемые ребра диаграммы Хассе для отношения <= на {1,2,3,4}: 1->2, 1->3, 2->4, 3->4
    EXPECT_TRUE(lattice.getHasseDiagram().hasEdge(0,1)); // 1->2
    EXPECT_TRUE(lattice.getHasseDiagram().hasEdge(0,2)); // 1->3
    EXPECT_TRUE(lattice.getHasseDiagram().hasEdge(1,3)); // 2->4
    EXPECT_TRUE(lattice.getHasseDiagram().hasEdge(2,3)); // 3->4
}

// Тестирование метода lessEqual
TEST(LatticeTest, LessEqualRelation) {
    ArraySequence<int> elements;
    elements.append(1);
    elements.append(2);
    elements.append(3);
    elements.append(4);    auto relation = [](const int& a, const int& b) -> bool { return a <= b; };
    Lattice<int> lattice(elements, relation);

    EXPECT_TRUE(lattice.lessEqual(1,2));
    EXPECT_TRUE(lattice.lessEqual(1,3));
    EXPECT_TRUE(lattice.lessEqual(1,4));
    EXPECT_TRUE(lattice.lessEqual(2,4));
    EXPECT_TRUE(lattice.lessEqual(3,4));
    EXPECT_TRUE(lattice.lessEqual(1,1));
    EXPECT_FALSE(lattice.lessEqual(2,1));
    EXPECT_FALSE(lattice.lessEqual(3,2));
}

// Тестирование операции meet (наибольшая нижняя грани)
TEST(LatticeTest, MeetOperation) {
    ArraySequence<int> elements;
    elements.append(1);
    elements.append(2);
    elements.append(3);
    elements.append(4);    auto relation = [](const int& a, const int& b) -> bool { return a <= b; };
    Lattice<int> lattice(elements, relation);

    // meet(2,3) =1
    auto meetResult = lattice.meet(2,3);
    ASSERT_TRUE(meetResult.has_value());
    EXPECT_EQ(meetResult.value(), 1);

    // meet(2,4) =2
    meetResult = lattice.meet(2,4);
    ASSERT_TRUE(meetResult.has_value());
    EXPECT_EQ(meetResult.value(), 2);

    // meet(3,4) =3
    meetResult = lattice.meet(3,4);
    ASSERT_TRUE(meetResult.has_value());
    EXPECT_EQ(meetResult.value(), 3);
}

// Тестирование операции join (наименьшая верхняя грани)
TEST(LatticeTest, JoinOperation) {
    ArraySequence<int> elements;
    elements.append(1);
    elements.append(2);
    elements.append(3);
    elements.append(4);    auto relation = [](const int& a, const int& b) -> bool { return a <= b; };
    Lattice<int> lattice(elements, relation);

    // join(1,2) =2
    auto joinResult = lattice.join(1,2);
    ASSERT_TRUE(joinResult.has_value());
    EXPECT_EQ(joinResult.value(), 2);

    // join(1,3) =3
    joinResult = lattice.join(1,3);
    ASSERT_TRUE(joinResult.has_value());
    EXPECT_EQ(joinResult.value(), 3);

    // join(2,3) =4
    joinResult = lattice.join(2,3);
    ASSERT_TRUE(joinResult.has_value());
    EXPECT_EQ(joinResult.value(),4);
}

// Тестирование Dijkstra на пустом графе
TEST(ShortestPathTest, EmptyGraph) {
    UndirectedGraph<int> graph(0);
    EXPECT_THROW(ShortestPath::dijkstra(graph, 0), std::out_of_range);
}

// Тестирование Dijkstra на графе с одной вершиной
TEST(ShortestPathTest, SingleVertex) {
    UndirectedGraph<int> graph(1);
    auto result = ShortestPath::dijkstra(graph, 0);
    EXPECT_EQ(result.first.getLength(),1);
    EXPECT_EQ(result.first[0], 0.0);
    EXPECT_EQ(result.second.getLength(),1);
    EXPECT_EQ(result.second[0], -1);
}

// Тестирование Dijkstra на простом графе без циклов
TEST(ShortestPathTest, SimpleGraph) {
    UndirectedGraph<int> graph(3);
    graph.addEdge(0,1,1);
    graph.addEdge(1,2,2);
    auto result = ShortestPath::dijkstra(graph, 0);

    ArraySequence<double> distances = result.first;
    ArraySequence<int> predecessors = result.second;

    EXPECT_EQ(distances.getLength(), 3);
    EXPECT_EQ(distances[0],0.0);
    EXPECT_EQ(distances[1],1.0);
    EXPECT_EQ(distances[2],3.0);

    EXPECT_EQ(predecessors[0], -1);
    EXPECT_EQ(predecessors[1],0);
    EXPECT_EQ(predecessors[2],1);
}

// Тестирование Dijkstra на графе с несколькими путями
TEST(ShortestPathTest, MultiplePaths) {
    UndirectedGraph<int> graph(4);
    graph.addEdge(0,1,1);
    graph.addEdge(0,2,4);
    graph.addEdge(1,2,2);
    graph.addEdge(1,3,5);
    graph.addEdge(2,3,1);

    auto result = ShortestPath::dijkstra(graph,0);

    ArraySequence<double> distances = result.first;
    ArraySequence<int> predecessors = result.second;

    EXPECT_EQ(distances.getLength(),4);
    EXPECT_EQ(distances[0],0.0);
    EXPECT_EQ(distances[1],1.0);
    EXPECT_EQ(distances[2],3.0); // 0->1->2
    EXPECT_EQ(distances[3],4.0); // 0->1->2->3

    EXPECT_EQ(predecessors[0], -1);
    EXPECT_EQ(predecessors[1],0);
    EXPECT_EQ(predecessors[2],1);
    EXPECT_EQ(predecessors[3],2);
}

// Тестирование Dijkstra с неверным источником
TEST(ShortestPathTest, SourceOutOfRange) {
    UndirectedGraph<int> graph(3);
    graph.addEdge(0,1,1);
    graph.addEdge(1,2,2);

    EXPECT_THROW(ShortestPath::dijkstra(graph, 3), std::out_of_range);
}

// Тестирование Dijkstra с динамическими весами на пустом графе
TEST(DynamicWeightShortestPathTest, EmptyGraph) {
    UndirectedGraph<double> graph(0);
    auto updateFunc = [](double weight, double time) -> double { return weight; };
    DynamicWeightShortestPath dsp(updateFunc);
    EXPECT_THROW(dsp.dijkstra(graph, 0, 0.0), std::out_of_range);
}

// Тестирование Dijkstra с динамическими весами на графе с одной вершиной
TEST(DynamicWeightShortestPathTest, SingleVertex) {
    UndirectedGraph<double> graph(1);
    auto updateFunc = [](double weight, double time) -> double { return weight; };
    DynamicWeightShortestPath dsp(updateFunc);
    auto result = dsp.dijkstra(graph, 0, 0.0);
    EXPECT_EQ(result.first.getLength(),1);
    EXPECT_EQ(result.first[0],0.0);
    EXPECT_EQ(result.second.getLength(),1);
    EXPECT_EQ(result.second[0], -1);
}

// Тестирование Dijkstra с динамическими весами на простом графе
TEST(DynamicWeightShortestPathTest, SimpleGraph) {
    UndirectedGraph<double> graph(3);
    graph.addEdge(0,1,1.0);
    graph.addEdge(1,2,2.0);

    auto updateFunc = [](double weight, double time) -> double { return weight + time; };
    DynamicWeightShortestPath dsp(updateFunc);

    auto result = dsp.dijkstra(graph,0, 0.0);
    ArraySequence<double> distances = result.first;
    ArraySequence<int> predecessors = result.second;

    EXPECT_EQ(distances.getLength(),3);
    EXPECT_EQ(distances[0],0.0);
    EXPECT_EQ(distances[1],1.0);
    EXPECT_EQ(distances[2],3.0); // 0->1->2

    EXPECT_EQ(predecessors[0], -1);
    EXPECT_EQ(predecessors[1],0);
    EXPECT_EQ(predecessors[2],1);
}

// Тестирование Dijkstra с изменением весов во времени
TEST(DynamicWeightShortestPathTest, TimeDependentWeights) {
    UndirectedGraph<double> graph(3);
    graph.addEdge(0,1,1.0);
    graph.addEdge(1,2,2.0);
    graph.addEdge(0,2,4.0);

    // Вес увеличивается на 0.5 за единицу времени
    auto updateFunc = [](double weight, double time) -> double { return weight + 0.5 * time; };
    DynamicWeightShortestPath dsp(updateFunc);

    double time = 2.0;
    auto result = dsp.dijkstra(graph,0, time);
    ArraySequence<double> distances = result.first;
    ArraySequence<int> predecessors = result.second;

    // Обновлённые веса: (0-1)=2.0, (1-2)=3.0, (0-2)=5.0
    EXPECT_EQ(distances.getLength(),3);
    EXPECT_EQ(distances[0],0.0);
    EXPECT_EQ(distances[1],2.0);
    EXPECT_EQ(distances[2],5.0); // 0->1->2

    EXPECT_EQ(predecessors[0], -1);
    EXPECT_EQ(predecessors[1],0);
    EXPECT_EQ(predecessors[2],1);
}

// Тестирование Dijkstra с динамическими весами на графе с пересекающимися путями
TEST(DynamicWeightShortestPathTest, ComplexGraph) {
    UndirectedGraph<double> graph(4);
    graph.addEdge(0,1,1.0);
    graph.addEdge(0,2,5.0);
    graph.addEdge(1,2,2.0);
    graph.addEdge(1,3,4.0);
    graph.addEdge(2,3,1.0);

    // Вес уменьшается на 0.2 за единицу времени
    auto updateFunc = [](double weight, double time) -> double { return weight - 0.2 * time; };
    DynamicWeightShortestPath dsp(updateFunc);

    double time = 3.0;
    auto result = dsp.dijkstra(graph,0, time);
    ArraySequence<double> distances = result.first;
    ArraySequence<int> predecessors = result.second;

    // Обновлённые веса: (0-1)=0.4, (0-2)=4.4, (1-2)=1.4, (1-3)=3.4, (2-3)=0.8
    EXPECT_EQ(distances.getLength(),4);
    EXPECT_DOUBLE_EQ(distances[0],0.0);
    EXPECT_DOUBLE_EQ(distances[1],0.4);
    EXPECT_DOUBLE_EQ(distances[2],1.8); // 0->1->2
    EXPECT_DOUBLE_EQ(distances[3],2.2); // 0->1->2->3

    EXPECT_EQ(predecessors[0], -1);
    EXPECT_EQ(predecessors[1],0);
    EXPECT_EQ(predecessors[2],1);
    EXPECT_EQ(predecessors[3],2);
}

// Тестирование hasPath на простом графе
TEST(DirectedGraphTest, HasPathSimple) {
    DirectedGraph<int> graph(3);
    graph.addEdge(0,1,1);
    graph.addEdge(1,2,1);

    EXPECT_TRUE(graph.hasPath(0,2));
    EXPECT_FALSE(graph.hasPath(2,0));
    EXPECT_TRUE(graph.hasPath(0,1));
    EXPECT_TRUE(graph.hasPath(1,2));
}

// Тестирование hasPath на графе без пути
TEST(DirectedGraphTest, HasPathNoPath) {
    DirectedGraph<int> graph(3);
    graph.addEdge(0,1,1);

    EXPECT_FALSE(graph.hasPath(1,0));
    EXPECT_FALSE(graph.hasPath(0,2));
    EXPECT_FALSE(graph.hasPath(2,0));
}

// Тестирование hasPath на полном графе
TEST(DirectedGraphTest, HasPathCompleteGraph) {
    DirectedGraph<int> graph(3);
    graph.addEdge(0,1,1);
    graph.addEdge(1,0,1);
    graph.addEdge(0,2,1);
    graph.addEdge(2,0,1);
    graph.addEdge(1,2,1);
    graph.addEdge(2,1,1);

    EXPECT_TRUE(graph.hasPath(0,1));
    EXPECT_TRUE(graph.hasPath(1,0));
    EXPECT_TRUE(graph.hasPath(0,2));
    EXPECT_TRUE(graph.hasPath(2,0));
    EXPECT_TRUE(graph.hasPath(1,2));
    EXPECT_TRUE(graph.hasPath(2,1));
}

// Тестирование hasPath на пустом графе
TEST(DirectedGraphTest, HasPathEmptyGraph) {
    DirectedGraph<int> graph(0);
    EXPECT_THROW(graph.hasPath(0,0), std::out_of_range);
}