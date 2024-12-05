// HasseWindow.h
#ifndef HASSEWINDOW_H
#define HASSEWINDOW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QRadioButton>
#include <QMessageBox>
#include "graph_structures/DirectedGraph.h"
#include "graph_structures/Lattice.h"


class HasseWindow : public QWidget {


public:
    explicit HasseWindow(QWidget *parent = nullptr);
    ~HasseWindow();

private slots:
    void onGenerateElements();
    void onCreateHasseDiagram();
    void onModeChanged();

private:
    void setupUI();
    void drawHasseDiagram(QGraphicsScene *scene, DirectedGraph<int> &graph);
    void calculateHasseLevels(const DirectedGraph<int>& graph, QMap<int, int>& levels);
    void drawElements();

    QGraphicsScene *elementScene;
    QGraphicsView *elementView;
    QGraphicsScene *hasseScene;
    QGraphicsView *hasseView;

    bool createExplicitDiagram();
    bool createImplicitDiagram();
    bool positionNodes(const QMap<int, int>& levels);

    QLineEdit *elementCountEdit;
    QLineEdit *minRangeEdit;
    QLineEdit *maxRangeEdit;
    QTextEdit *elementInput;
    QTextEdit *relationInput;
    QRadioButton *explicitMode;
    QRadioButton *implicitMode;
    QPushButton *generateButton;
    QPushButton *createButton;
    QComboBox *relationTypeComboBox;

    ArraySequence<int> elements;
    QVector<QPointF> nodePositions;
    DirectedGraph<int> hasseDiagram;
};

#endif // HASSEWINDOW_H