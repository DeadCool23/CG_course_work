#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "beziercurve.h"

class QPushButton;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showHelp();

private:
    BezierCurve *bezierCanvas;
    QPushButton *undoButton;
    QPushButton *clearButton;
    QPushButton *generateButton;
};

#endif // MAINWINDOW_H
