#include "light.h"

Light::Light(double intensity, const QVector3D &position, const QColor &color)
    : position(position), color(color), intensity(intensity) {}
