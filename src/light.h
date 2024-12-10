#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>
#include <QColor>

#define STD_LIGHT_COLOR Qt::white

class Light {
public:
    Light() = default;
    Light(double intensity, const QVector3D &position = {0, 0, 0}, const QColor &color= STD_LIGHT_COLOR);

    QVector3D position;
    QColor color = STD_LIGHT_COLOR;
    double intensity = 1.2;

    bool isEnable = false;
};

#endif // LIGHT_H
