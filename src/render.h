#ifndef RENDER_H
#define RENDER_H

#include <QColor>
#include <QVector>

#include "light.h"
#include "polygonmodel.h"

class Render
{
public:
    Render() = default;
    explicit Render(int h, int w, QColor bg_color);

    QVector<QVector<QColor>> renderPolygonalModel(const PolygonModel &model, const Light &light);
    QVector<QVector<QColor>> getRenderImage();

private:
    int h, w;
    QVector<QVector<double>> zBuffer;
    QVector<QVector<QColor>> imageBuffer;

    void resetBuffers();
    void renderTriangle(const QVector<QPointF> &projectedVertices,
                        const QVector<QVector3D> &modelVertices,
                        const QVector<QVector3D> &normals,
                        const QVector<int> &triangle,
                        const QColor &baseColor,
                        const Light &light);
    bool isDegenerate(const QPointF &p0, const QPointF &p1, const QPointF &p2);
    std::tuple<double, double, double> computeBarycentricCoords(
        const QPointF &point,
        const QPointF &p0,
        const QPointF &p1,
        const QPointF &p2);
    void updateBuffers(int x, int y, double z, const QColor &color);

    QVector3D interpolate_normals(QVector3D n0, QVector3D n1, QVector3D n2, double alpha, double beta, double gamma);
    QColor computePhongShading(const QVector3D &normal,
                               const QVector3D &viewDir,
                               const Light &light,
                               const QVector3D &point,
                               const QColor &baseColor);

    QVector<QPointF> projectTo2D(const QVector<QVector3D> &points);
};

#endif // RENDER_H
