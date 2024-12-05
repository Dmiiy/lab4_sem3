#include "UndirectedWindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QInputDialog>
#include <QtMath>
#include <QGraphicsLineItem>

UndirectedWindow::UndirectedWindow(QWidget *parent)
        : QWidget(parent), originalGraph(0), resultGraph(0)
{
    setupUI();
}

UndirectedWindow::~UndirectedWindow()
{
    delete originalScene;
    delete originalView;
    delete resultScene;
    delete resultView;
}

void UndirectedWindow::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Панель управления
    QHBoxLayout *controlLayout = new QHBoxLayout();

    QLabel *graphTypeLabel = new QLabel("Тип графа:", this);
    graphTypeComboBox = new QComboBox(this);
    graphTypeComboBox->addItem("Полный граф", COMPLETE);
    graphTypeComboBox->addItem("Разреженный граф", SPARSE);
    graphTypeComboBox->addItem("Случайный граф", RANDOM);
    graphTypeComboBox->addItem("Цикл", CYCLE);
    graphTypeComboBox->addItem("Дерево", TREE);

    QLabel *vertexCountLabel = new QLabel("Количество вершин:", this);
    vertexCountEdit = new QLineEdit("5", this);

    QLabel *edgeProbabilityLabel = new QLabel("Вероятность ребра:", this);
    edgeProbabilityEdit = new QLineEdit("0.5", this);

    QLabel *algorithmLabel = new QLabel("Алгоритм:", this);
    algorithmComboBox = new QComboBox(this);
    algorithmComboBox->addItem("Раскраска графа", COLORING);
    algorithmComboBox->addItem("Минимальное остовное дерево", MST);
    algorithmComboBox->addItem("Компоненты связности", CONNECTED_COMPONENTS);

    generateButton = new QPushButton("Сгенерировать граф", this);
    runAlgorithmButton = new QPushButton("Запустить алгоритм", this);

    controlLayout->addWidget(graphTypeLabel);
    controlLayout->addWidget(graphTypeComboBox);
    controlLayout->addWidget(vertexCountLabel);
    controlLayout->addWidget(vertexCountEdit);
    controlLayout->addWidget(edgeProbabilityLabel);
    controlLayout->addWidget(edgeProbabilityEdit);
    controlLayout->addWidget(algorithmLabel);
    controlLayout->addWidget(algorithmComboBox);
    controlLayout->addWidget(generateButton);
    controlLayout->addWidget(runAlgorithmButton);

    connect(generateButton, &QPushButton::clicked, this, &UndirectedWindow::onGenerateGraph);
    connect(runAlgorithmButton, &QPushButton::clicked, this, &UndirectedWindow::onRunAlgorithm);

    mainLayout->addLayout(controlLayout);

    // Виджеты для отображения графов
    QHBoxLayout *graphsLayout = new QHBoxLayout();

    originalScene = new QGraphicsScene(this);
    originalView = new QGraphicsView(originalScene, this);
    originalView->setRenderHint(QPainter::Antialiasing);

    resultScene = new QGraphicsScene(this);
    resultView = new QGraphicsView(resultScene, this);
    resultView->setRenderHint(QPainter::Antialiasing);

    graphsLayout->addWidget(originalView);
    graphsLayout->addWidget(resultView);

    mainLayout->addLayout(graphsLayout);
}

void UndirectedWindow::onGenerateGraph()
{
    int vertexCount = vertexCountEdit->text().toInt();
    double edgeProbability = edgeProbabilityEdit->text().toDouble();

    GraphType graphType = static_cast<GraphType>(graphTypeComboBox->currentData().toInt());

    switch (graphType) {
        case COMPLETE:
            originalGraph = GraphGenerator::generateUndirectedGraph(GraphGenerator::COMPLETE, vertexCount, 1.0, 10);
            break;
        case SPARSE:
            originalGraph = GraphGenerator::generateUndirectedGraph(GraphGenerator::SPARSE, vertexCount, edgeProbability, 10);
            break;
        case RANDOM:
            originalGraph = GraphGenerator::generateUndirectedGraph(GraphGenerator::RANDOM, vertexCount, edgeProbability, 10);
            break;
        case CYCLE:
            originalGraph = GraphGenerator::generateUndirectedGraph(GraphGenerator::CYCLE, vertexCount, 1.0, 10);
            break;
        case TREE:
            originalGraph = GraphGenerator::generateUndirectedGraph(GraphGenerator::TREE, vertexCount, 1.0, 10);
            break;
        default:
            break;
    }

    // Генерация позиций вершин
    nodePositions.clear();
    for (int i = 0; i < vertexCount; ++i) {
        qreal angle = (2 * M_PI * i) / vertexCount;
        qreal x = qCos(angle) * 150;
        qreal y = qSin(angle) * 150;
        nodePositions.append(QPointF(x, y));
    }

    drawGraph(originalScene, originalGraph);
    resultScene->clear();
}

void UndirectedWindow::onRunAlgorithm()
{
    AlgorithmType algorithmType = static_cast<AlgorithmType>(algorithmComboBox->currentData().toInt());

    switch (algorithmType) {
        case COLORING: {
            auto colors = GraphColoring::greedyColoring(originalGraph);
            resultGraph = originalGraph; // Копируем оригинальный граф для отображения
            // Раскрашиваем вершины в resultScene
            resultScene->clear();
            drawGraph(resultScene, resultGraph);
            for (int i = 0; i < nodePositions.size(); ++i) {
                qreal x = nodePositions.at(i).x();
                qreal y = nodePositions.at(i).y();
                QColor color = QColor::fromHsv((colors[i] * 36) % 360, 255, 255);
                resultScene->addEllipse(x - 10, y - 10, 20, 20, QPen(Qt::black), QBrush(color));
                resultScene->addText(QString::number(i))->setPos(x - 5, y - 10);
            }
            break;
        }
        case MST: {
            auto mstEdges = MinimumSpanningTree<int>::kruskal(originalGraph);
            resultGraph = UndirectedGraph<int>(originalGraph.getVertexCount());
            for (int i = 0; i < mstEdges.getLength(); ++i) {
                int from = std::get<0>(mstEdges[i]);
                int to = std::get<1>(mstEdges[i]);
                int weight = std::get<2>(mstEdges[i]);
                resultGraph.addEdge(from, to, weight);
            }
            drawGraph(resultScene, resultGraph);
            break;
        }
        case CONNECTED_COMPONENTS: {
            auto components = ConnectedComponents::findComponents(originalGraph);
            resultGraph = originalGraph; // Копируем оригинальный граф для отображения
            resultScene->clear();
            drawGraph(resultScene, resultGraph);
            QColor colors[] = {Qt::red, Qt::green, Qt::blue, Qt::yellow, Qt::cyan, Qt::magenta};
            int colorCount = sizeof(colors) / sizeof(colors[0]);
            for (int c = 0; c < components.getLength(); ++c) {
                auto component = components.get(c);
                QColor color = colors[c % colorCount];
                for (int i = 0; i < component.getLength(); ++i) {
                    int index = component.get(i);
                    qreal x = nodePositions.at(index).x();
                    qreal y = nodePositions.at(index).y();
                    resultScene->addEllipse(x - 10, y - 10, 20, 20, QPen(Qt::black), QBrush(color));
                    resultScene->addText(QString::number(index))->setPos(x - 5, y - 10);
                }
            }
            break;
        }
        default:
            break;
    }
}

void UndirectedWindow::drawGraph(QGraphicsScene *scene, UndirectedGraph<int> &graph)
{
    scene->clear();
    const int vertexCount = graph.getVertexCount();

    // Вычисляем радиус графа в зависимости от количества вершин
    const qreal radius = std::max(150.0, vertexCount * 30.0);

    // Обновляем позиции вершин
    nodePositions.clear();
    for (int i = 0; i < vertexCount; ++i) {
        qreal angle = (2 * M_PI * i) / vertexCount;
        qreal x = qCos(angle) * radius;
        qreal y = qSin(angle) * radius;
        nodePositions.append(QPointF(x, y));
    }

    // Отрисовка рёбер
    auto edges = graph.getEdges();
    for (int i = 0; i < edges.getLength(); ++i) {
        int from = std::get<0>(edges[i]);
        int to = std::get<1>(edges[i]);
        int weight = std::get<2>(edges[i]);
        drawUndirectedEdge(scene, from, to, weight);
    }

    // Отрисовка вершин поверх рёбер
    const int vertexRadius = 15;
    for (int i = 0; i < nodePositions.size(); ++i) {
        qreal x = nodePositions.at(i).x();
        qreal y = nodePositions.at(i).y();

        QBrush brush(Qt::white);
        QPen pen(Qt::black, 2);
        scene->addEllipse(x - vertexRadius, y - vertexRadius,
                          2 * vertexRadius, 2 * vertexRadius,
                          pen, brush);

        auto text = scene->addText(QString::number(i));
        text->setDefaultTextColor(Qt::black);
        text->setPos(x - text->boundingRect().width()/2,
                     y - text->boundingRect().height()/2);
    }

    // Масштабируем вид
    originalView->setSceneRect(scene->itemsBoundingRect());
    originalView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void UndirectedWindow::drawUndirectedEdge(QGraphicsScene *scene, int from, int to, int weight)
{
//    QPointF fromPos = nodePositions.at(from);
//    QPointF toPos = nodePositions.at(to);
//
//    QLineF line(fromPos, toPos);
//
//    // Рисуем линию
//    QGraphicsLineItem *lineItem = scene->addLine(line, QPen(Qt::black));
//
//    // Отображаем вес ребра
//    QPointF textPos = (fromPos + toPos) / 2;
//    scene->addText(QString::number(weight))->setPos(textPos);
    const int vertexRadius = 15;
    QPointF fromPos = nodePositions.at(from);
    QPointF toPos = nodePositions.at(to);

    // Вычисляем направляющий вектор
    QPointF direction = toPos - fromPos;
    qreal length = QLineF(fromPos, toPos).length();

    // Нормализуем вектор
    direction /= length;

    // Корректируем начальную и конечную точки с учетом размера вершин
    QPointF adjustedFromPos = fromPos;
    QPointF adjustedToPos = toPos ;

    // Рисуем линию
    QPen edgePen(Qt::black, 1.5);
    scene->addLine(QLineF(adjustedFromPos, adjustedToPos), edgePen);


    // Отображаем вес ребра
    QPointF midPoint = (fromPos + toPos) / 2;
    QPointF normal(-direction.y(), direction.x());
    QPointF weightPos = midPoint + normal * 15;

    auto textItem = scene->addText(QString::number(weight));
    textItem->setDefaultTextColor(Qt::black);
    textItem->setPos(weightPos.x() - textItem->boundingRect().width()/2,
                     weightPos.y() - textItem->boundingRect().height()/2);
}