#include "HasseWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QtMath>
#include <QGraphicsEllipseItem>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QMessageBox>
#include <random>
#include <QSet>

HasseWindow::HasseWindow(QWidget *parent) : QWidget(parent), hasseDiagram(0) {
    setupUI();
}

HasseWindow::~HasseWindow() {
    delete elementScene;
    delete hasseScene;
}

void HasseWindow::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Control Panel
    QGroupBox *controlGroup = new QGroupBox("Управление", this);
    QVBoxLayout *controlLayout = new QVBoxLayout;

    // Mode Selection
    QHBoxLayout *modeLayout = new QHBoxLayout;
    explicitMode = new QRadioButton("Явное задание", this);
    implicitMode = new QRadioButton("Неявное задание", this);
    explicitMode->setChecked(true);
    modeLayout->addWidget(explicitMode);
    modeLayout->addWidget(implicitMode);
    controlLayout->addLayout(modeLayout);

    QHBoxLayout *implicitLayout = new QHBoxLayout;
    QLabel *relationTypeLabel = new QLabel("Тип отношения:", this);
    relationTypeComboBox = new QComboBox(this);
    relationTypeComboBox->addItem("Делимость", "divisibility");
    relationTypeComboBox->addItem("Меньше чем", "less");
    relationTypeComboBox->addItem("Подмножество", "subset");
    relationTypeComboBox->addItem("Степень", "power");
    implicitLayout->addWidget(relationTypeLabel);
    implicitLayout->addWidget(relationTypeComboBox);
    controlLayout->addLayout(implicitLayout);

    relationTypeComboBox->setEnabled(false);
    connect(implicitMode, &QRadioButton::toggled, relationTypeComboBox, &QComboBox::setEnabled);

    connect(explicitMode, &QRadioButton::toggled, this, &HasseWindow::onModeChanged);

    // Element Generation
    QHBoxLayout *genLayout = new QHBoxLayout;
    QLabel *countLabel = new QLabel("Количество элементов:", this);
    elementCountEdit = new QLineEdit("5", this);

    QLabel *rangeLabel = new QLabel("Диапазон:", this);
    minRangeEdit = new QLineEdit("1", this);
    QLabel *toLabel = new QLabel("до", this);
    maxRangeEdit = new QLineEdit("20", this);

    generateButton = new QPushButton("Сгенерировать элементы", this);

    genLayout->addWidget(countLabel);
    genLayout->addWidget(elementCountEdit);
    genLayout->addWidget(rangeLabel);
    genLayout->addWidget(minRangeEdit);
    genLayout->addWidget(toLabel);
    genLayout->addWidget(maxRangeEdit);
    genLayout->addWidget(generateButton);

    controlLayout->addLayout(genLayout);

    // Element Input
    QLabel *elemLabel = new QLabel("Элементы множества:", this);
    elementInput = new QTextEdit(this);
    elementInput->setPlaceholderText("Введите элементы через запятую");
    controlLayout->addWidget(elemLabel);
    controlLayout->addWidget(elementInput);

    // Relation Input
    QLabel *relLabel = new QLabel("Отношение порядка:", this);
    relationInput = new QTextEdit(this);
    relationInput->setPlaceholderText(
            "Явное задание: (1,2), (2,4), (1,4)\n"
            "Неявное задание примеры:\n"
            "- Делимость: b % a == 0\n"
            "- Отношение '<': a < b\n"
            "- Подмножество: (a & b) == a\n"
            "- Степень: b = a^n"
    );
    controlLayout->addWidget(relLabel);
    controlLayout->addWidget(relationInput);

    createButton = new QPushButton("Построить диаграмму Хассе", this);
    controlLayout->addWidget(createButton);

    controlGroup->setLayout(controlLayout);
    mainLayout->addWidget(controlGroup);

    // Views
    QHBoxLayout *viewsLayout = new QHBoxLayout;

    // Elements View
    QVBoxLayout *elemViewLayout = new QVBoxLayout;
    QLabel *elemViewLabel = new QLabel("Исходное множество:", this);
    elementScene = new QGraphicsScene(this);
    elementView = new QGraphicsView(elementScene, this);
    elemViewLayout->addWidget(elemViewLabel);
    elemViewLayout->addWidget(elementView);
    viewsLayout->addLayout(elemViewLayout);

    // Hasse Diagram View
    QVBoxLayout *hasseViewLayout = new QVBoxLayout;
    QLabel *hasseViewLabel = new QLabel("Диаграмма Хассе:", this);
    hasseScene = new QGraphicsScene(this);
    hasseView = new QGraphicsView(hasseScene, this);
    hasseViewLayout->addWidget(hasseViewLabel);
    hasseViewLayout->addWidget(hasseView);
    viewsLayout->addLayout(hasseViewLayout);

    mainLayout->addLayout(viewsLayout);

    connect(generateButton, &QPushButton::clicked, this, &HasseWindow::onGenerateElements);
    connect(createButton, &QPushButton::clicked, this, &HasseWindow::onCreateHasseDiagram);
}

void HasseWindow::onModeChanged() {
    bool isImplicit = implicitMode->isChecked();
    relationTypeComboBox->setEnabled(isImplicit);
    relationInput->setVisible(!isImplicit);
    if (!isImplicit) {
        relationInput->setPlaceholderText("Введите пары (a,b) через запятую");
    }
}


void HasseWindow::onGenerateElements() {
    int count = elementCountEdit->text().toInt();
    int minRange = minRangeEdit->text().toInt();
    int maxRange = maxRangeEdit->text().toInt();

    if (minRange >= maxRange) {
        QMessageBox::warning(this, "Ошибка", "Минимальное значение должно быть меньше максимального");
        return;
    }

    QSet<int> usedNumbers;
    elements.clear();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(minRange, maxRange);

    while (usedNumbers.size() < count && usedNumbers.size() < (maxRange - minRange + 1)) {
        int num = dis(gen);
        if (!usedNumbers.contains(num)) {
            usedNumbers.insert(num);
            elements.append(num);
        }
    }

    QString elemStr;
    for (int i = 0; i < elements.getLength(); i++) {
        if (i > 0) elemStr += ", ";
        elemStr += QString::number(elements.get(i));
    }
    elementInput->setText(elemStr);

    drawElements();
}

void HasseWindow::drawElements() {
    elementScene->clear();

    const int radius = 100;
    const int elementRadius = 15;

    for (int i = 0; i < elements.getLength(); i++) {
        double angle = (2 * M_PI * i) / elements.getLength();
        qreal x = qCos(angle) * radius;
        qreal y = qSin(angle) * radius;

        QBrush brush(Qt::white);
        QPen pen(Qt::black, 2);
        elementScene->addEllipse(x - elementRadius, y - elementRadius,
                                 2 * elementRadius, 2 * elementRadius,
                                 pen, brush);

        auto text = elementScene->addText(QString::number(elements.get(i)));
        text->setDefaultTextColor(Qt::black);
        text->setPos(x - text->boundingRect().width()/2,
                     y - text->boundingRect().height()/2);
    }

    elementView->setSceneRect(elementScene->itemsBoundingRect());
    elementView->fitInView(elementScene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void HasseWindow::onCreateHasseDiagram() {
    elements.clear();
    QString elemStr = elementInput->toPlainText();
    QStringList elemList = elemStr.split(",", Qt::SkipEmptyParts);
    for (const QString &elem : elemList) {
        bool ok;
        int value = elem.trimmed().toInt(&ok);
        if (ok) {
            elements.append(value);
        }
    }

    if (elements.getLength() < 2) {
        QMessageBox::warning(this, "Ошибка", "Необходимо минимум 2 элемента");
        return;
    }

    hasseDiagram = DirectedGraph<int>(elements.getLength());

    if (explicitMode->isChecked()) {
        if (!createExplicitDiagram()) {
            return;
        }
    } else {
        if (!createImplicitDiagram()) {
            return;
        }
    }

    QMap<int, int> levels;
    calculateHasseLevels(hasseDiagram, levels);

    if (!positionNodes(levels)) {
        return;
    }

    drawHasseDiagram(hasseScene, hasseDiagram);
}

bool HasseWindow::createExplicitDiagram() {
    QString relStr = relationInput->toPlainText();
    QRegularExpression rx("\\((\\d+),(\\d+)\\)");
    QRegularExpressionMatchIterator matchIterator = rx.globalMatch(relStr);

    bool hasEdges = false;
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        int from = match.captured(1).toInt() - 1;
        int to = match.captured(2).toInt() - 1;

        if (from >= 0 && from < elements.getLength() &&
            to >= 0 && to < elements.getLength()) {
            hasseDiagram.addEdge(from, to, 1);
            hasEdges = true;
        }
    }

    if (!hasEdges) {
        QMessageBox::warning(this, "Ошибка", "Не найдены корректные отношения");
        return false;
    }
    return true;
}

bool HasseWindow::createImplicitDiagram() {
    QString relationType = relationTypeComboBox->currentData().toString();
    std::function<bool(const int&, const int&)> implicitRelation;

    if (relationType == "divisibility") {
        implicitRelation = [](const int &a, const int &b) -> bool {
            return a != b && b % a == 0 && a > 0;
        };
    }
    else if (relationType == "less") {
        implicitRelation = [](const int &a, const int &b) -> bool {
            return a < b;
        };
    }
    else if (relationType == "subset") {
        implicitRelation = [](const int &a, const int &b) -> bool {
            return a != b && (a & b) == a;
        };
    }
    else if (relationType == "power") {
        implicitRelation = [](const int &a, const int &b) -> bool {
            if (a <= 1 || b <= 1 || a == b) return false;
            double logResult = std::log(b) / std::log(a);
            return std::abs(logResult - std::round(logResult)) < 1e-10;
        };
    }

    try {
        Lattice<int> lattice(elements, implicitRelation);
        hasseDiagram = lattice.getHasseDiagram();
        if (hasseDiagram.getEdges().getLength() == 0) {
            QMessageBox::warning(this, "Предупреждение", "Не найдены отношения между элементами");
            return false;
        }
        return true;
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", QString("Ошибка построения диаграммы: %1").arg(e.what()));
        return false;
    }
}

bool HasseWindow::positionNodes(const QMap<int, int>& levels) {
    nodePositions.clear();
    int maxLevel = 0;
    for (const auto &level : levels) {
        maxLevel = qMax(maxLevel, level);
    }

    if (maxLevel == 0) {
        QMessageBox::warning(this, "Предупреждение", "Граф не имеет иерархической структуры");
        return false;
    }

    QMap<int, int> levelCounts;
    QMap<int, int> levelCurrentPos;
    for (int i = 0; i < elements.getLength(); i++) {
        levelCounts[levels[i]]++;
    }

    for (int i = 0; i < elements.getLength(); i++) {
        int level = levels[i];
        int position = levelCurrentPos[level]++;
        double x = (position + 0.5) * (300.0 / (levelCounts[level] + 1)) - 150;
        double y = -150 + (300.0 * level) / maxLevel;
        nodePositions.append(QPointF(x, y));
    }
    return true;
}


void HasseWindow::calculateHasseLevels(const DirectedGraph<int>& graph, QMap<int, int>& levels) {
    int n = graph.getVertexCount();
    levels.clear();

    for (int i = 0; i < n; i++) {
        levels[i] = 0;
    }

    bool changed;
    do {
        changed = false;
        for (int i = 0; i < n; i++) {
            auto neighbors = graph.getNeighbors(i);
            for (int j = 0; j < neighbors.getLength(); j++) {
                int neighbor = neighbors.get(j).first;
                if (levels[neighbor] <= levels[i]) {
                    levels[neighbor] = levels[i] + 1;
                    changed = true;
                }
            }
        }
    } while (changed);
}

void HasseWindow::drawHasseDiagram(QGraphicsScene *scene, DirectedGraph<int> &graph,const QPen &pen) {
    scene->clear();

    auto edges = graph.getEdges();
    for (int i = 0; i < edges.getLength(); i++) {
        int from = std::get<0>(edges[i]);
        int to = std::get<1>(edges[i]);

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
    }

    const int vertexRadius = 15;
    for (int i = 0; i < nodePositions.size(); i++) {
        qreal x = nodePositions.at(i).x();
        qreal y = nodePositions.at(i).y();

        QBrush brush(Qt::white);
        QPen pen(Qt::black, 2);
        scene->addEllipse(x - vertexRadius, y - vertexRadius,
                          2 * vertexRadius, 2 * vertexRadius,
                          pen, brush);

        auto text = scene->addText(QString::number(elements.get(i)));
        text->setDefaultTextColor(Qt::black);
        text->setPos(x - text->boundingRect().width()/2,
                     y - text->boundingRect().height()/2);
    }

    hasseView->setSceneRect(scene->itemsBoundingRect());
    hasseView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}