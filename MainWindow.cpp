#include "MainWindow.h"
#include <QTabWidget>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
{

    setupUI();
}

MainWindow::~MainWindow()
{
    delete directedWindow;
    delete undirectedWindow;
    delete hasseWindow;
}

void MainWindow::setupUI()
{
    QTabWidget *tabWidget = new QTabWidget(this);
    directedWindow = new DirectedWindow(this);
    undirectedWindow = new UndirectedWindow(this);
    hasseWindow = new HasseWindow(this);

    tabWidget->addTab(directedWindow, "Ориентированные графы");
    tabWidget->addTab(undirectedWindow, "Неориентированные графы");
    tabWidget->addTab(hasseWindow, "Диаграмма Хассе");

    setCentralWidget(tabWidget);
}