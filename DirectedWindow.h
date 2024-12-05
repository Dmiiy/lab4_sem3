#ifndef DIRECTEDWINDOW_H
#define DIRECTEDWINDOW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVector>
#include <QPointF>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include "graph_structures/DirectedGraph.h"
#include "graph_structures/GraphGenerator.h"
#include "graph_structures/StronglyConnectedComponents.h"
#include "graph_structures/Lattice.h"
#include "graph_structures/DynamicWeightShortestPath.h"
#include "graph_structures/ShortestPath.h"
#include "GraphTypes.h"

class DirectedWindow : public QWidget {


public:
    explicit DirectedWindow(QWidget *parent = nullptr);
    ~DirectedWindow();

private slots:
    void onGenerateGraph();
    void onRunAlgorithm();

private:
    void setupUI();
    void drawGraph(QGraphicsScene *scene, DirectedGraph<int> &graph);
    void drawDirectedEdge(QGraphicsScene *scene, int from, int to, int weight, const QPen &pen = QPen(Qt::black));
//    void drawShortestPath(const ArraySequence<int>& path, const QColor& pathColor);
//    bool checkPathExists(int source, int target);
    void drawPath(const ArraySequence<int>& path, const QColor& color);
    void drawComponents(const ArraySequence<ArraySequence<int>>& components);
    void drawHighlightedEdge(QGraphicsScene *scene, int from, int to, int weight, const QColor& color);
    void drawHighlightedEdge(QGraphicsScene *scene, int from, int to, const QString& weightLabel, const QColor& color);
    bool checkPathExists(int source, int target);

    QGraphicsScene *originalScene;
    QGraphicsView *originalView;
    QGraphicsScene *resultScene;
    QGraphicsView *resultView;

    DirectedGraph<int> originalGraph;
    DirectedGraph<int> resultGraph;
    QVector<QPointF> nodePositions;

    QComboBox *graphTypeComboBox;
    QComboBox *algorithmComboBox;
    QPushButton *generateButton;
    QPushButton *runAlgorithmButton;
    QLineEdit *vertexCountEdit;
    QLineEdit *edgeProbabilityEdit;
};
#endif // DIRECTEDWINDOW_H