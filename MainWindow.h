#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "DirectedWindow.h"
#include "UndirectedWindow.h"
#include "HasseWindow.h"

class MainWindow : public QMainWindow {


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupUI();

    DirectedWindow *directedWindow;
    UndirectedWindow *undirectedWindow;
    HasseWindow *hasseWindow;
};

#endif // MAINWINDOW_H