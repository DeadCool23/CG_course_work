#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include <QWidget>
#include <QPoint>
#include <QVector>
#include <QPainter>

class BezierCurve : public QWidget {
    Q_OBJECT

public:
    std::vector<QPoint> mainPoints;

    explicit BezierCurve(QWidget *parent = nullptr);
    QVector<QPointF> calculateBezierCurve();

    void clear();
    void undoLastPoint();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    const int steps = 100;
    std::vector<QPoint> controlPoints;

    QPointF deCasteljau(double t);
};

#endif // BEZIERCURVE_H
