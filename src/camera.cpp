#include "camera.h"
#include <cmath>

Camera::Camera()
    : radius(7000.0),
    angle_Oxz(0.0),
    angle_Ozy(0.0) {}

QMatrix4x4 Camera::getViewMatrix() const {
    double rad_angle_Oxz = qDegreesToRadians(angle_Oxz);
    double rad_angle_Ozy = qDegreesToRadians(angle_Ozy);

    float angle_Ozy_coefs[] = {
        1, 0                  , 0                 , 0,
        0, (float)cos(rad_angle_Ozy) , (float)sin(rad_angle_Ozy), 0,
        0, (float)-sin(rad_angle_Ozy), (float)cos(rad_angle_Ozy), 0,
        0, 0                  , 0                 , 1
    };

    float angle_Oxz_coefs[] = {
        (float)cos(rad_angle_Oxz) , 0, (float)sin(rad_angle_Oxz), 0,
        0                    , 1, 0                   , 0,
        (float)-sin(rad_angle_Oxz), 0, (float)cos(rad_angle_Oxz), 0,
        0                    , 0, 0                   , 1
    };

    float scale_coefs[] = {
        (float)(5000 / radius), 0, 0, 0,
        0, (float)(5000 / radius), 0, 0,
        0, 0, (float)(5000 / radius), 0,
        0, 0, 0                     , 1
    };

    QMatrix4x4 scaleMatrix(scale_coefs);
    QMatrix4x4 angle_OzyRotateMatrix(angle_Ozy_coefs);
    QMatrix4x4 angle_OxzRotateMatrix(angle_Oxz_coefs);

    return scaleMatrix * angle_OzyRotateMatrix * angle_OxzRotateMatrix;
}

QVector3D Camera::getPosition() const {
    double rad_angle_Oxz = qDegreesToRadians(angle_Oxz);
    double rad_angle_Ozy = qDegreesToRadians(angle_Ozy);

    double x = radius * cos(rad_angle_Ozy) * sin(rad_angle_Oxz);
    double y = radius * sin(rad_angle_Ozy);
    double z = radius * cos(rad_angle_Ozy) * cos(rad_angle_Oxz);

    return QVector3D(x, y, z);
}

void Camera::reset() {
    angle_Oxz = angle_Ozy = 0;
    radius = 7000;
}


void Camera::adjustAngleOxz(double delta) {
    angle_Oxz += delta;
}

void Camera::adjustAngleOzy(double delta) {
    angle_Ozy = std::clamp(angle_Ozy + delta, -90.0, 90.0);
}

void Camera::adjustRadius(double delta) {
    radius = std::clamp(radius + delta, 5000.0, 10000.0);
}
