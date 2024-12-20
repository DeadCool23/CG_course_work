#ifndef ROTATIONVIEWER_H
#define ROTATIONVIEWER_H

#include <QWidget>
#include <QVector>
#include <QVector3D>

#include "light.h"
#include "camera.h"
#include "render.h"
#include "polygonmodel.h"
#include "lightinputwindow.h"

class RotationViewer : public QWidget {
    Q_OBJECT

public:
    PolygonModel model;

    Light light;
    Camera camera;

    LightInputWindow *liWin;

    explicit RotationViewer(const QVector<QPointF> &curve, double rotationAxisX, QWidget *parent = nullptr);
    void saveSceneToImage(const QString &filePath);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void drawScene(const QVector<QVector<QColor>> &pixels, QPainter &painter);

    PolygonModel transformModelToCameraSpace();
    QVector3D transformToCameraSpace(const QVector3D &point);
    QVector<QVector3D> transformToCameraSpace(const QVector<QVector3D> &points);

    int segments = 64;
    Render render;
    QColor bg_color = QColor::fromRgb(20, 20, 20);
};

#endif // ROTATIONVIEWER_H
