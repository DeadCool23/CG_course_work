#ifndef POLYGONMODEL_H
#define POLYGONMODEL_H

#include <QColor>
#include <QVector>
#include <QVector3D>


#define STD_COLOR Qt::gray

class PolygonModel {
public:
    QVector<QVector3D> normals;
    QVector<QVector3D> vertices;
    QVector<QVector<int>> triangles;
    QVector<QColor> triangleColors;

    static PolygonModel generateFromCurve(const QVector<QPointF> &curve, double rotationAxisX, int segments);

    void generateColors();
    void setColor(const QColor &color);

    bool isRandColors = false;
private:
    void validateTriangleOrientation();
    void calculateNormals();
    void normalize();
};


#endif // POLYGONMODEL_H
