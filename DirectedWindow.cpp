#include "DirectedWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QInputDialog>
#include <QtMath>
#include <QMessageBox>
#include <QGraphicsTextItem>

DirectedWindow::DirectedWindow(QWidget *parent)
        : QWidget(parent), originalGraph(0), resultGraph(0) {
    setupUI();
}

DirectedWindow::~DirectedWindow() {
    delete originalScene;
    delete originalView;
    delete resultScene;
    delete resultView;
}

void DirectedWindow::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Control Panel
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
    algorithmComboBox->addItem("Кратчайший путь", SHORTEST_PATH);
    algorithmComboBox->addItem("Динамический кратчайший путь", DYNAMIC_SHORTEST_PATH);
    algorithmComboBox->addItem("Сильно связанные компоненты", STRONGLY_CONNECTED_COMPONENTS);

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

    mainLayout->addLayout(controlLayout);

    // Graph Views
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

    connect(generateButton, &QPushButton::clicked, this, &DirectedWindow::onGenerateGraph);
    connect(runAlgorithmButton, &QPushButton::clicked, this, &DirectedWindow::onRunAlgorithm);
}

void DirectedWindow::onGenerateGraph() {
    bool ok;
    int vertexCount = vertexCountEdit->text().toInt(&ok);
    if (!ok || vertexCount <= 0) {
        QMessageBox::warning(this, "Ошибка", "Неверное количество вершин");
        return;
    }

    double edgeProbability = edgeProbabilityEdit->text().toDouble(&ok);
    if (!ok || edgeProbability < 0 || edgeProbability > 1) {
        QMessageBox::warning(this, "Ошибка", "Вероятность должна быть от 0 до 1");
        return;
    }

    try {
        GraphType graphType = static_cast<GraphType>(graphTypeComboBox->currentData().toInt());
        originalGraph = DirectedGraph<int>(vertexCount);

        switch (graphType) {
            case COMPLETE:
                originalGraph = GraphGenerator::generateDirectedGraph(GraphGenerator::COMPLETE, vertexCount, 1.0, 10);
                break;
            case SPARSE:
                originalGraph = GraphGenerator::generateDirectedGraph(GraphGenerator::SPARSE, vertexCount,
                                                                      edgeProbability, 10);
                break;
            case RANDOM:
                originalGraph = GraphGenerator::generateDirectedGraph(GraphGenerator::RANDOM, vertexCount,
                                                                      edgeProbability, 10);
                break;
            case CYCLE:
                originalGraph = GraphGenerator::generateDirectedGraph(GraphGenerator::CYCLE, vertexCount, 1.0, 10);
                break;
            case TREE:
                originalGraph = GraphGenerator::generateDirectedGraph(GraphGenerator::TREE, vertexCount, 1.0, 10);
                break;
        }

        // Generate vertex positions
        nodePositions.clear();
        const double radius = std::max(150.0, vertexCount * 30.0);
        for (int i = 0; i < vertexCount; ++i) {
            double angle = (2 * M_PI * i) / vertexCount;
            double x = std::cos(angle) * radius;
            double y = std::sin(angle) * radius;
            nodePositions.append(QPointF(x, y));
        }

        drawGraph(originalScene, originalGraph);
        resultScene->clear();

    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Ошибка", QString("Ошибка при генерации графа: %1").arg(e.what()));
    }
}

void DirectedWindow::onRunAlgorithm() {
    if (originalGraph.getVertexCount() == 0) {
        QMessageBox::warning(this, "Ошибка", "Сначала сгенерируйте граф");
        return;
    }

    AlgorithmType algorithmType = static_cast<AlgorithmType>(algorithmComboBox->currentData().toInt());

    try {
        switch (algorithmType) {
            case SHORTEST_PATH: {
                bool ok;
                int source = QInputDialog::getInt(this, "Исходная вершина",
                                                  "Введите номер исходной вершины:", 0, 0,
                                                  originalGraph.getVertexCount() - 1, 1, &ok);
                if (!ok) return;

                int target = QInputDialog::getInt(this, "Целевая вершина",
                                                  "Введите номер целевой вершины:", 0, 0,
                                                  originalGraph.getVertexCount() - 1, 1, &ok);
                if (!ok) return;

                if (!checkPathExists(source, target)) {
                    QMessageBox::warning(this, "Ошибка", "Между вершинами нет пути");
                    return;
                }

                try {
                    auto data = ShortestPath<int>::dijkstra(originalGraph, source);
                    auto path = ShortestPath<int>::getPath(data, target);

                    resultScene->clear();
                    drawGraph(resultScene, originalGraph);

                    int totalWeight = 0;
                    for (int i = 0; i < path.getLength() - 1; ++i) {
                        int from = path.get(i);
                        int to = path.get(i + 1);
                        int weight = originalGraph.getEdgeWeight(from, to);
                        totalWeight += weight;
                        drawHighlightedEdge(resultScene, from, to, weight, Qt::red);
                    }

                    QString pathStr = QString("Длина пути: %1\nПуть: ").arg(totalWeight);
                    for (int i = 0; i < path.getLength(); ++i) {
                        pathStr += QString::number(path.get(i));
                        if (i < path.getLength() - 1) pathStr += " → ";
                    }
                    QMessageBox::information(this, "Результат", pathStr);
                }
                catch (const std::exception &e) {
                    QMessageBox::critical(this, "Ошибка",
                                          QString("Ошибка при поиске пути: %1").arg(e.what()));
                }
                break;
            }
            case DYNAMIC_SHORTEST_PATH: {
                bool ok;
                int source = QInputDialog::getInt(this, "Исходная вершина",
                                                  "Введите номер исходной вершины:", 0, 0,
                                                  originalGraph.getVertexCount() - 1, 1, &ok);
                if (!ok) return;

                int target = QInputDialog::getInt(this, "Целевая вершина",
                                                  "Введите номер целевой вершины:", 0, 0,
                                                  originalGraph.getVertexCount() - 1, 1, &ok);
                if (!ok) return;

                if (!checkPathExists(source, target)) {
                    QMessageBox::warning(this, "Ошибка", "Между вершинами нет пути");
                    return;
                }

                try {
                    // Create DynamicWeightShortestPath with time influence factor
                    DynamicWeightShortestPath<int> dynamicSP(0.1); // 10% increase per second
                    auto data = dynamicSP.dijkstra(originalGraph, source);

                    if (data[target].first == std::numeric_limits<int>::max()) {
                        QMessageBox::warning(this, "Ошибка", "Путь не существует");
                        return;
                    }

                    auto path = DynamicWeightShortestPath<int>::getPath(data, target);
                    resultScene->clear();
                    drawGraph(resultScene, originalGraph);

                    // Show path with dynamic weights
                    int totalOriginalWeight = 0;
                    int totalDynamicWeight = 0;

                    for (int i = 0; i < path.getLength() - 1; ++i) {
                        int from = path.get(i);
                        int to = path.get(i + 1);
                        int originalWeight = originalGraph.getEdgeWeight(from, to);
                        int dynamicWeight = data[to].first - data[from].first;

                        totalOriginalWeight += originalWeight;
                        totalDynamicWeight += dynamicWeight;

                        QString weightLabel = QString("%1→%2")
                                .arg(originalWeight)
                                .arg(dynamicWeight);
                        drawHighlightedEdge(resultScene, from, to, weightLabel, Qt::blue);
                    }

                    QString pathStr = QString(
                            "Исходная длина пути: %1\n"
                            "Динамическая длина пути: %2\n"
                            "Время выполнения повлияло на веса\n"
                            "Путь: "
                    ).arg(totalOriginalWeight).arg(totalDynamicWeight);

                    for (int i = 0; i < path.getLength(); ++i) {
                        pathStr += QString::number(path.get(i));
                        if (i < path.getLength() - 1) pathStr += " → ";
                    }
                    QMessageBox::information(this, "Результат", pathStr);
                }
                catch (const std::exception& e) {
                    QMessageBox::critical(this, "Ошибка",
                                          QString("Ошибка при поиске пути: %1").arg(e.what()));
                }
                break;
            }
            case STRONGLY_CONNECTED_COMPONENTS: {
                auto components = StronglyConnectedComponents<int>::findSCC(originalGraph);
                drawComponents(components);
                break;
            }
        }
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Ошибка",
                              QString("Ошибка при выполнении алгоритма: %1").arg(e.what()));
    }
}

// Add implementation to DirectedWindow.cpp:
bool DirectedWindow::checkPathExists(int source, int target) {
    ArraySequence<bool> visited;
    for (int i = 0; i < originalGraph.getVertexCount(); i++) {
        visited.append(false);
    }

    std::function<bool(int)> dfs = [&](int v) -> bool {
        if (v == target) return true;
        visited[v] = true;

        auto neighbors = originalGraph.getNeighbors(v);
        for (int i = 0; i < neighbors.getLength(); i++) {
            int next = neighbors[i].first;
            if (!visited[next] && dfs(next)) {
                return true;
            }
        }
        return false;
    };

    return dfs(source);
}


void DirectedWindow::drawHighlightedEdge(QGraphicsScene *scene, int from, int to,
                                         const QString &weightLabel, const QColor &color) {
    QPointF fromPos = nodePositions.at(from);
    QPointF toPos = nodePositions.at(to);

    QPointF direction = toPos - fromPos;
    qreal length = QLineF(fromPos, toPos).length();
    direction /= length;

    const int vertexRadius = 15;
    QPointF adjustedFromPos = fromPos + direction * vertexRadius;
    QPointF adjustedToPos = toPos - direction * (vertexRadius + 10);

    QPen pen(color, 2.5);
    scene->addLine(QLineF(adjustedFromPos, adjustedToPos), pen);

    const qreal arrowSize = 10;
    QPointF arrowP1 = adjustedToPos + QPointF(
            direction.y() * arrowSize - direction.x() * arrowSize,
            -direction.x() * arrowSize - direction.y() * arrowSize
    );
    QPointF arrowP2 = adjustedToPos + QPointF(
            -direction.y() * arrowSize - direction.x() * arrowSize,
            direction.x() * arrowSize - direction.y() * arrowSize
    );

    QPolygonF arrowHead;
    arrowHead << adjustedToPos << arrowP1 << arrowP2;
    scene->addPolygon(arrowHead, pen, QBrush(color));

    QPointF midPoint = (fromPos + toPos) / 2;
    QPointF normal(-direction.y(), direction.x());
    QPointF weightPos = midPoint + normal * 20;

    auto text = scene->addText(weightLabel);
    text->setDefaultTextColor(color);
    text->setPos(weightPos.x() - text->boundingRect().width() / 2,
                 weightPos.y() - text->boundingRect().height() / 2);
}

void DirectedWindow::drawPath(const ArraySequence<int> &path, const QColor &color) {
    resultScene->clear();
    drawGraph(resultScene, originalGraph);

    double totalWeight = 0;
    for (int i = 0; i < path.getLength() - 1; ++i) {
        int from = path.get(i);
        int to = path.get(i + 1);
        int weight = originalGraph.getEdgeWeight(from, to);
        totalWeight += weight;

        drawHighlightedEdge(resultScene, from, to, weight, color);
    }

    QString pathStr = QString("Длина пути: %1\nПуть: ").arg(totalWeight);
    for (int i = 0; i < path.getLength(); ++i) {
        pathStr += QString::number(path.get(i));
        if (i < path.getLength() - 1) pathStr += " → ";
    }

    QMessageBox::information(this, "Результат", pathStr);
}

void DirectedWindow::drawComponents(const ArraySequence<ArraySequence<int>> &components) {
    resultScene->clear();
    drawGraph(resultScene, originalGraph);

    QColor colors[] = {Qt::red, Qt::green, Qt::blue, Qt::yellow, Qt::cyan, Qt::magenta};
    int colorCount = sizeof(colors) / sizeof(colors[0]);

    for (int c = 0; c < components.getLength(); ++c) {
        auto component = components.get(c);
        QColor color = colors[c % colorCount];

        for (int i = 0; i < component.getLength(); ++i) {
            int vertex = component.get(i);
            QPointF pos = nodePositions.at(vertex);
            resultScene->addEllipse(pos.x() - 15, pos.y() - 15,
                                    30, 30, QPen(Qt::black), QBrush(color));
        }
    }
}

void DirectedWindow::drawGraph(QGraphicsScene *scene, DirectedGraph<int> &graph) {
    scene->clear();
    auto edges = graph.getEdges();

    // Draw edges
    for (int i = 0; i < edges.getLength(); ++i) {
        int from = std::get<0>(edges[i]);
        int to = std::get<1>(edges[i]);
        int weight = std::get<2>(edges[i]);
        drawDirectedEdge(scene, from, to, weight);
    }

    // Draw vertices
    const int vertexRadius = 15;
    for (int i = 0; i < nodePositions.size(); ++i) {
        QPointF pos = nodePositions.at(i);
        scene->addEllipse(pos.x() - vertexRadius, pos.y() - vertexRadius,
                          2 * vertexRadius, 2 * vertexRadius,
                          QPen(Qt::black), QBrush(Qt::white));

        auto text = scene->addText(QString::number(i));
        text->setDefaultTextColor(Qt::black);
        text->setPos(pos.x() - text->boundingRect().width() / 2,
                     pos.y() - text->boundingRect().height() / 2);
    }

    scene->views().first()->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void DirectedWindow::drawDirectedEdge(QGraphicsScene *scene, int from, int to, int weight, const QPen &pen) {
    QPointF fromPos = nodePositions.at(from);
    QPointF toPos = nodePositions.at(to);

    QPointF direction = toPos - fromPos;
    qreal length = QLineF(fromPos, toPos).length();
    direction /= length;

    const int vertexRadius = 15;
    QPointF adjustedFromPos = fromPos + direction * vertexRadius;
    QPointF adjustedToPos = toPos - direction * (vertexRadius + 10);

    // Draw edge
    scene->addLine(QLineF(adjustedFromPos, adjustedToPos), pen);

    // Draw arrow
    const qreal arrowSize = 10;
    QPointF arrowP1 = adjustedToPos + QPointF(
            direction.y() * arrowSize - direction.x() * arrowSize,
            -direction.x() * arrowSize - direction.y() * arrowSize
    );
    QPointF arrowP2 = adjustedToPos + QPointF(
            -direction.y() * arrowSize - direction.x() * arrowSize,
            direction.x() * arrowSize - direction.y() * arrowSize
    );

    QPolygonF arrowHead;
    arrowHead << adjustedToPos << arrowP1 << arrowP2;
    scene->addPolygon(arrowHead, pen, QBrush(pen.color()));

    // Draw weight
    QPointF midPoint = (fromPos + toPos) / 2;
    QPointF normal(-direction.y(), direction.x());
    QPointF weightPos = midPoint + normal * 15;

    auto text = scene->addText(QString::number(weight));
    text->setDefaultTextColor(pen.color());
    text->setPos(weightPos.x() - text->boundingRect().width() / 2,
                 weightPos.y() - text->boundingRect().height() / 2);
}

void DirectedWindow::drawHighlightedEdge(QGraphicsScene *scene, int from, int to, int weight, const QColor &color) {
    drawDirectedEdge(scene, from, to, weight, QPen(color, 2.5));
}