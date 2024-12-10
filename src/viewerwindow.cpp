#include "viewerwindow.h"
#include <QMenuBar>
#include <QMessageBox>

ViewerWindow::ViewerWindow(const QVector<QPointF> &curve, double rotationAxisX, QWidget *parent)
    : QMainWindow(parent) {
    setWindowTitle("Тело вращения");
    setMinimumSize(800, 600);

    viewer = new RotationViewer(curve, rotationAxisX, this);
    setCentralWidget(viewer);

    auto *helpMenu = menuBar()->addMenu("Справка");

    auto *cameraHelpAction = new QAction("Управление камерой", this);
    connect(cameraHelpAction, &QAction::triggered, this, &ViewerWindow::showCameraHelp);
    auto *saveHelpAction = new QAction("Сохранение сцены", this);
    connect(saveHelpAction, &QAction::triggered, this, &ViewerWindow::showSaveHelp);
    auto *colorChangeHelpAction = new QAction("Изменение цвета", this);
    connect(colorChangeHelpAction, &QAction::triggered, this, &ViewerWindow::showColorChangeHelp);
    auto *lightHelpAction = new QAction("Источник света", this);
    connect(lightHelpAction, &QAction::triggered, this, &ViewerWindow::showLightHelp);
    helpMenu->addAction(saveHelpAction);
    helpMenu->addAction(lightHelpAction);
    helpMenu->addAction(colorChangeHelpAction);
    helpMenu->addAction(cameraHelpAction);
}


void ViewerWindow::showCameraHelp() {
    QMessageBox::information(this, "Управление камерой",
                             "Управление камерой:\n"
                             "- W / S: Перемещение вверх / вниз.\n"
                             "- A / D: Перемещение влево / вправо.\n"
                             "- Q / E: Перемещение ближе / дальше.\n"
                             "- R - вернуть камеру в исходное положение");
}

void ViewerWindow::showLightHelp() {
    QMessageBox::information(this, "Источник света",
                             "Чтобы задать источник света нажмите кнопку \"L\\l\"\n"
                             "чтобы задать положения источника света относительно обекта\n"
                             "нажмите на кнопку [*]\n"
                             "* - источник света\n"
                             "О - объект\n");
}


void ViewerWindow::showSaveHelp() {
    QMessageBox::information(this, "Сохранение сцены",
                             "Чтобы сохранить сцену в виде изображения нажмите \"I\\i\"");
}

void ViewerWindow::showColorChangeHelp() {
    QMessageBox::information(this, "Изменение цвета",
                             "Чтобы изменить цвет тела вращения нажмите \"C\\c\"");
}

ViewerWindow::~ViewerWindow() = default;
