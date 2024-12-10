#include "beziercurve.h"
#include <QMouseEvent>
#include <QMessageBox>
#include <QPainterPath>

BezierCurve::BezierCurve(QWidget *parent) : QWidget(parent) {
    setMinimumSize(100, 100);
}

void BezierCurve::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (mainPoints.size() >= 2) {
            QMessageBox::warning(this, "Ошибка", "Вы не можете добавить более двух опорных точек.");
            return;
        }
        mainPoints.emplace_back(event->pos());
    } else if (event->button() == Qt::RightButton) {
        if (mainPoints.size() < 2) {
            QMessageBox::warning(this, "Ошибка", "Сначала задайте две опорные точки.");
            return;
        }
        controlPoints.emplace_back(event->pos());
    }
    update();
}

void BezierCurve::undoLastPoint() {
    if (!controlPoints.empty()) {
        controlPoints.pop_back();
    } else if (!mainPoints.empty()) {
        mainPoints.pop_back();
    }
    update();
}

void BezierCurve::clear() {
    controlPoints.clear();
    mainPoints.clear();
    update();
}

void BezierCurve::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(QPen(Qt::gray, 1, Qt::DashLine));
    int centerX = width() / 2;
    painter.drawLine(centerX, 0, centerX, height());

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    for (const auto &point : mainPoints)
        painter.drawEllipse(point, 3, 3);

    painter.setBrush(Qt::red);
    for (const auto &point : controlPoints)
        painter.drawEllipse(point, 3, 3);

    if (mainPoints.size() == 2) {
        painter.setPen(Qt::black);
        QVector<QPointF> curvePoints = calculateBezierCurve();

        for (int i = 0; i < curvePoints.size() - 1; ++i) {
            painter.drawLine(curvePoints[i], curvePoints[i + 1]);
        }
    }
}

QVector<QPointF> BezierCurve::calculateBezierCurve() {
    QVector<QPointF> curvePoints;

    for (int step = 0; step <= steps; ++step) {
        double t = static_cast<double>(step) / steps;
        curvePoints.append(deCasteljau(t));
    }

    return curvePoints;
}

QPointF BezierCurve::deCasteljau(double t) {
    QVector<QPointF> points = {mainPoints[0]};
    for (auto point: controlPoints)
        points.append(point);
    points.append(mainPoints[1]);

    while (points.size() > 1) {
        QVector<QPointF> nextLevel;
        for (int i = 0; i < points.size() - 1; ++i) {
            QPointF point = (1 - t) * points[i] + t * points[i + 1];
            nextLevel.append(point);
        }
        points = nextLevel;
    }

    return points[0];
}
