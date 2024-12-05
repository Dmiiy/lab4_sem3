#ifndef UNDIRECTEDWINDOW_H
#define UNDIRECTEDWINDOW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVector>
#include <QPointF>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include "graph_structures/UndirectedGraph.h"
#include "graph_structures/GraphGenerator.h"
#include "graph_structures/ConnectedComponents.h"
#include "graph_structures/GraphColoring.h"
#include "graph_structures/MinimumSpanningTree.h"
#include "GraphTypes.h"


class UndirectedWindow : public QWidget {


public:
    explicit UndirectedWindow(QWidget *parent = nullptr);
    ~UndirectedWindow();

private slots:
    void onGenerateGraph();
    void onRunAlgorithm();

private:
    void setupUI();
    void drawGraph(QGraphicsScene *scene, UndirectedGraph<int> &graph);
    void drawUndirectedEdge(QGraphicsScene *scene, int from, int to, int weight);

    QGraphicsScene *originalScene;
    QGraphicsView *originalView;
    QGraphicsScene *resultScene;
    QGraphicsView *resultView;

    UndirectedGraph<int> originalGraph;
    UndirectedGraph<int> resultGraph;
    QVector<QPointF> nodePositions;

    QComboBox *graphTypeComboBox;
    QComboBox *algorithmComboBox;
    QPushButton *generateButton;
    QPushButton *runAlgorithmButton;
    QLineEdit *vertexCountEdit;
    QLineEdit *edgeProbabilityEdit;
};

#endif // UNDIRECTEDWINDOW_H