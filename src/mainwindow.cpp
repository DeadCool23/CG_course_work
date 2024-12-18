#include "mainwindow.h"
#include "viewerwindow.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QMenuBar>

#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    bezierCanvas = new BezierCurve(this);

    undoButton = new QPushButton("Откатить", this);
    connect(undoButton, &QPushButton::clicked, bezierCanvas, &BezierCurve::undoLastPoint);
    clearButton = new QPushButton("Очистить", this);
    connect(clearButton, &QPushButton::clicked, bezierCanvas, &BezierCurve::clear);

    generateButton = new QPushButton("Сгенерировать тело вращения", this);
    connect(generateButton, &QPushButton::clicked, this, [this]() {
        if (bezierCanvas->mainPoints.size() != 2) {
            QMessageBox::warning(this, "Ошибка", "Кривая еще не задана.");
            return;
        }
        QVector<QPointF> curve = bezierCanvas->calculateBezierCurve();

        auto *viewerWindow = new ViewerWindow(curve, bezierCanvas->width() / 2);
        viewerWindow->show();
    });

    auto *buttonLayout = new QVBoxLayout();
    auto *centralWidget = new QWidget(this);
    auto *buttonBezierLayout = new QHBoxLayout();
    buttonBezierLayout->addWidget(undoButton);
    buttonBezierLayout->addWidget(clearButton);
    buttonLayout->addLayout(buttonBezierLayout);
    buttonLayout->addWidget(generateButton);

    auto *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(bezierCanvas);
    mainLayout->addLayout(buttonLayout);

    setCentralWidget(centralWidget);

    auto *helpMenu = menuBar()->addMenu("Справка");
    auto *helpAction = new QAction("Как пользоваться", this);
    connect(helpAction, &QAction::triggered, this, &MainWindow::showHelp);
    helpMenu->addAction(helpAction);

    setWindowTitle("Кривая Безье");
    resize(800, 600);
}

MainWindow::~MainWindow() = default;

void MainWindow::showHelp() {
    QMessageBox::information(this, "Справка",
                             "Как работать с программой:\n"
                             "1. Левой кнопкой мыши ставьте опорные точки (максимум 2).\n"
                             "2. Правой кнопкой мыши добавляйте точки для кривизны.\n"
                             "3. Кнопка 'Откатить последнюю точку' удаляет последнюю добавленную точку.");
}
