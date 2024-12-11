#include "render.h"

#include <QDebug>

Render::Render(int h, int w, QColor bg_color) : h(h), w(w) {
    zBuffer.resize(h, QVector<double>(w, std::numeric_limits<double>::infinity()));
    imageBuffer.resize(h, QVector<QColor>(w, bg_color));
}

QColor Render::computePhongShading(const QVector3D &normal,
                                   const QVector3D &viewDir,
                                   const Light &light,
                                   const QVector3D &point,
                                   const QColor &baseColor) {
    QVector3D lightDir = (light.position - point);
    lightDir.normalize();

    double ambientStrength = 0.3;
    QColor ambient = QColor(
        int(baseColor.red() * ambientStrength * light.color.redF()),
        int(baseColor.green() * ambientStrength * light.color.greenF()),
        int(baseColor.blue() * ambientStrength * light.color.blueF()));

    double diff = std::pow(std::max(QVector3D::dotProduct(normal, lightDir), 0.2f), 0.7);
    QColor diffuse = QColor(
        int(baseColor.red() * diff * light.intensity * light.color.redF()),
        int(baseColor.green() * diff * light.intensity * light.color.greenF()),
        int(baseColor.blue() * diff * light.intensity * light.color.blueF()));

    double specularStrength = 0.0;
    QVector3D reflectDir = (2.0 * QVector3D::dotProduct(normal, lightDir) * normal - lightDir).normalized();
    double spec = std::pow(std::max(QVector3D::dotProduct(viewDir, reflectDir), 0.0f), 32.0);
    QColor specular = QColor(
        int(255 * spec * light.intensity * specularStrength * light.color.redF()),
        int(255 * spec * light.intensity * specularStrength * light.color.greenF()),
        int(255 * spec * light.intensity * specularStrength * light.color.blueF()));

    int r = std::min(ambient.red() + diffuse.red() + specular.red(), 255);
    int g = std::min(ambient.green() + diffuse.green() + specular.green(), 255);
    int b = std::min(ambient.blue() + diffuse.blue() + specular.blue(), 255);

    return QColor(r, g, b);
}

QVector<QVector<QColor>> Render::renderPolygonalModel(const PolygonModel &model, const Light &light) {
    resetBuffers();

    auto projectedVertices = projectTo2D(model.vertices);

    for (int i = 0; i < model.triangles.size(); ++i) {
        const auto &triangle = model.triangles[i];
        const auto &normals = model.normals;
        auto color = model.triangleColors[i];

        renderTriangle(projectedVertices, model.vertices, normals, triangle, color, light);
    }

    return imageBuffer;
}

QVector3D Render::interpolate_normals(QVector3D n0, QVector3D n1, QVector3D n2, double alpha, double beta, double gamma) {
    QVector3D result;
    result.setX(n0.x() * alpha + n1.x() * beta + n2.x() * gamma);
    result.setY(n0.y() * alpha + n1.y() * beta + n2.y() * gamma);
    result.setZ(n0.z() * alpha + n1.z() * beta + n2.z() * gamma);

    return result;
}

void Render::renderTriangle(const QVector<QPointF> &projectedVertices,
                            const QVector<QVector3D> &modelVertices,
                            const QVector<QVector3D> &normals,
                            const QVector<int> &triangle,
                            const QColor &baseColor,
                            const Light &light) {
    QPointF p0 = projectedVertices[triangle[0]];
    QPointF p1 = projectedVertices[triangle[1]];
    QPointF p2 = projectedVertices[triangle[2]];

    QVector3D n0 = normals[triangle[0]];
    QVector3D n1 = normals[triangle[1]];
    QVector3D n2 = normals[triangle[2]];

    double z0 = modelVertices[triangle[0]].z();
    double z1 = modelVertices[triangle[1]].z();
    double z2 = modelVertices[triangle[2]].z();

    QVector3D v0 = modelVertices[triangle[0]];
    QVector3D v1 = modelVertices[triangle[1]];
    QVector3D v2 = modelVertices[triangle[2]];

    if (isDegenerate(p0, p1, p2)) return;

    int minX = std::max(0, static_cast<int>(std::floor(std::min({p0.x(), p1.x(), p2.x()}))));
    int maxX = std::min(w - 1, static_cast<int>(std::ceil(std::max({p0.x(), p1.x(), p2.x()}))));
    int minY = std::max(0, static_cast<int>(std::floor(std::min({p0.y(), p1.y(), p2.y()}))));
    int maxY = std::min(h - 1, static_cast<int>(std::ceil(std::max({p0.y(), p1.y(), p2.y()}))));

    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            QPointF point(x + 0.5, y + 0.5);

            auto [alpha, beta, gamma] = computeBarycentricCoords(point, p0, p1, p2);
            if (alpha < 0 || beta < 0 || gamma < 0) continue;

            double z = alpha * z0 + beta * z1 + gamma * z2;

            QColor pixelColor;
            if (light.isEnable) {
                QVector3D normal = interpolate_normals(n0, n1, n2, alpha, beta, gamma);
                QVector3D worldPoint = alpha * v0 + beta * v1 + gamma * v2;
                QVector3D viewDir = -worldPoint.normalized();
                pixelColor = computePhongShading(normal, viewDir, light, worldPoint, baseColor);
            } else {
                pixelColor = baseColor;
            }

            updateBuffers(x, y, z, pixelColor);
        }
    }
}

bool Render::isDegenerate(const QPointF &p0, const QPointF &p1, const QPointF &p2) {
    double denom = (p1.y() - p2.y()) * (p0.x() - p2.x()) + (p2.x() - p1.x()) * (p0.y() - p2.y());
    return std::abs(denom) < 1e-6;
}

std::tuple<double, double, double> Render::computeBarycentricCoords(const QPointF &point,
                                                                    const QPointF &p0,
                                                                    const QPointF &p1,
                                                                    const QPointF &p2) {
    double denom = (p1.y() - p2.y()) * (p0.x() - p2.x()) + (p2.x() - p1.x()) * (p0.y() - p2.y());
    double alpha = ((p1.y() - p2.y()) * (point.x() - p2.x()) + (p2.x() - p1.x()) * (point.y() - p2.y())) / denom;
    double beta = ((p2.y() - p0.y()) * (point.x() - p2.x()) + (p0.x() - p2.x()) * (point.y() - p2.y())) / denom;
    double gamma = 1.0 - alpha - beta;
    return {alpha, beta, gamma};
}

void Render::updateBuffers(int x, int y, double z, const QColor &color) {
    if (z < zBuffer[y][x]) {
        zBuffer[y][x] = z;
        imageBuffer[y][x] = color;
    }
}

QVector<QVector<QColor>> Render::getRenderImage() {
    return imageBuffer;
}

QVector<QPointF> Render::projectTo2D(const QVector<QVector3D> &points) {
    QVector<QPointF> projectedPoints;

    double perspective = 1000;
    for (const auto &point : points) {
        double scale = perspective / (perspective + point.z());
        double x = w / 2 + point.x() * scale;
        double y = h / 2 + point.y() * scale;

        projectedPoints.append(QPointF(x, y));
    }

    return projectedPoints;
}

void Render::resetBuffers() {
    for (auto &row : zBuffer) {
        std::fill(row.begin(), row.end(), std::numeric_limits<double>::infinity());
    }
    for (auto &row : imageBuffer) {
        std::fill(row.begin(), row.end(), Qt::black);
    }
}
