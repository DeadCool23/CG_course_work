#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include <QVector3D>

class Camera {
public:
    Camera();

    QMatrix4x4 getViewMatrix() const;
    QVector3D getPosition() const;

    void reset();

    void adjustAngleOxz(double delta);
    void adjustAngleOzy(double delta);
    void adjustRadius(double delta);

private:
    double radius;
    double angle_Oxz;
    double angle_Ozy;
};

#endif // CAMERA_H
