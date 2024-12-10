#include "polygonmodel.h"

#include <cmath>
#include <QDebug>
#include <QRandomGenerator>

PolygonModel PolygonModel::generateFromCurve(const QVector<QPointF> &curve, double rotationAxisX, int segments) {
    PolygonModel model;

    double angleStep = 2 * M_PI / segments;

    for (const auto &point : curve) {
        double relativeX = point.x() - rotationAxisX;
        for (int i = 0; i <= segments; ++i) {
            double angle = i * angleStep;
            double x = relativeX * cos(angle);
            double y = point.y();
            double z = relativeX * sin(angle);
            model.vertices.append(QVector3D(x, y, z));
        }
    }

    int curvePointCount = curve.size();
    for (int j = 0; j < curvePointCount - 1; ++j) {
        int baseIndex = j * (segments + 1);
        int nextBaseIndex = baseIndex + (segments + 1);

        for (int i = 0; i < segments; ++i) {
            int current = baseIndex + i;
            int next = baseIndex + i + 1;
            int nextLayerCurrent = nextBaseIndex + i;
            int nextLayerNext = nextBaseIndex + i + 1;

            model.triangles.append({current, nextLayerCurrent, next});
            model.triangleColors.append(QColor(STD_COLOR));
            model.triangles.append({next, nextLayerCurrent, nextLayerNext});
            model.triangleColors.append(QColor(STD_COLOR));
        }
    }

    int topCenter = model.vertices.size();
    model.vertices.append(QVector3D(0.0, curve.first().y(), 0.0));
    for (int i = 0; i < segments; ++i) {
        model.triangles.append({i, (i + 1) % (segments + 1), topCenter});
        model.triangleColors.append(QColor(STD_COLOR));
    }

    int bottomCenter = model.vertices.size();
    model.vertices.append(QVector3D(0.0, curve.last().y(), 0.0));
    int baseIndex = (curvePointCount - 1) * (segments + 1);
    for (int i = 0; i < segments; ++i) {
        model.triangles.append({baseIndex + i, bottomCenter, baseIndex + (i + 1) % (segments + 1)});
        model.triangleColors.append(QColor(STD_COLOR));
    }

    model.normalize();
    model.validateTriangleOrientation();
    model.calculateNormals();
    return model;
}

void PolygonModel::validateTriangleOrientation() {
    for (auto &triangle : triangles) {
        QVector3D v0 = vertices[triangle[1]] - vertices[triangle[0]];
        QVector3D v1 = vertices[triangle[2]] - vertices[triangle[0]];
        QVector3D normal = QVector3D::crossProduct(v0, v1).normalized();

        QVector3D centroid = (vertices[triangle[0]] + vertices[triangle[1]] + vertices[triangle[2]]) / 3.0;
        QVector3D toCentroid = centroid.normalized();

        if (QVector3D::dotProduct(normal, toCentroid) < 0) {
            // Если порядок неправильный, меняем местами две вершины
            std::swap(triangle[1], triangle[2]);
        }
    }
}

void PolygonModel::calculateNormals() {
    normals.resize(vertices.size());
    std::fill(normals.begin(), normals.end(), QVector3D(0, 0, 0));

    for (const auto &triangle : triangles) {
        QVector3D v0 = vertices[triangle[1]] - vertices[triangle[0]];
        QVector3D v1 = vertices[triangle[2]] - vertices[triangle[0]];
        QVector3D normal = QVector3D::crossProduct(v0, v1);

        for (int i = 0; i < 3; ++i) {
            normals[triangle[i]] += normal;
        }
    }

    for (auto &normal : normals) {
        if (!normal.isNull()) {
            normal.normalize();
        }
    }
}

void PolygonModel::normalize() {
    QVector3D center(0, 0, 0);
    for (const auto &vertex : vertices) {
        center += vertex;
    }
    center /= vertices.size();

    for (auto &vertex : vertices) {
        vertex -= center;
    }
}

void PolygonModel::generateColors() {
    triangleColors.clear();
    for (int i = 0; i < triangles.size(); ++i) {
        QColor color = QColor::fromRgb(
            QRandomGenerator::global()->bounded(256),
            QRandomGenerator::global()->bounded(256),
            QRandomGenerator::global()->bounded(256)
        );
        triangleColors.append(color);
    }
    isRandColors = true;
}

void PolygonModel::setColor(const QColor &color) {
    triangleColors.clear();
    for (int i = 0; i < triangles.size(); ++i) {
        triangleColors.append(color);
    }
    isRandColors = false;
}

