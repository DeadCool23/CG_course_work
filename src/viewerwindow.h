#ifndef VIEWERWINDOW_H
#define VIEWERWINDOW_H

#include <QMainWindow>
#include "rotationviewer.h"

class ViewerWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit ViewerWindow(const QVector<QPointF> &curve, double rotationAxisX, QWidget *parent = nullptr);
    ~ViewerWindow();

private:
    void showCameraHelp();
    void showSaveHelp();
    void showLightHelp();
    void showColorChangeHelp();

    RotationViewer *viewer;
};

#endif // VIEWERWINDOW_H
