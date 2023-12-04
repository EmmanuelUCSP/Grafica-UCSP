#ifndef TRANSFORM_H
#define TRANSFORM_H

#define M_PI 3.14159265358979323846

#include "math/lib4f.h"

Mtx4f rotation_general(const Vec4f& axis, float angle) {
    angle = (angle * M_PI) / 180.0f;
    Mtx4f result(Mtx4f::eye());
    Vec4f unit_axis = axis.unit();

    float cos_angle = std::cos(angle);
    float sin_angle = std::sin(angle);
    float one_minus_cos_angle = 1.0f - cos_angle;

    float xx = unit_axis[0] * unit_axis[0];
    float xy = unit_axis[0] * unit_axis[1];
    float xz = unit_axis[0] * unit_axis[2];
    float yy = unit_axis[1] * unit_axis[1];
    float yz = unit_axis[1] * unit_axis[2];
    float zz = unit_axis[2] * unit_axis[2];
    float xs = unit_axis[0] * sin_angle;
    float ys = unit_axis[1] * sin_angle;
    float zs = unit_axis[2] * sin_angle;

    result(0, 0) = xx + (1 - xx) * cos_angle;
    result(0, 1) = xy * one_minus_cos_angle - zs;
    result(0, 2) = xz * one_minus_cos_angle + ys;
    result(1, 0) = xy * one_minus_cos_angle + zs;
    result(1, 1) = yy + (1 - yy) * cos_angle;
    result(1, 2) = yz * one_minus_cos_angle - xs;
    result(2, 0) = xz * one_minus_cos_angle - ys;
    result(2, 1) = yz * one_minus_cos_angle + xs;
    result(2, 2) = zz + (1 - zz) * cos_angle;

    return result;
}


Mtx4f translation_general(const Vec4f& direction, float distance) {
    Vec4f unit_direction = direction.unit();
    unit_direction = unit_direction * distance;
    Mtx4f result(Mtx4f::eye());
    result(0, 3) = unit_direction[0];
    result(1, 3) = unit_direction[1];
    result(2, 3) = unit_direction[2];
    return result;
}

Mtx4f translation_general(const Vec4f& destiny) {
    Mtx4f result(Mtx4f::eye());
    result(0, 3) = destiny[0];
    result(1, 3) = destiny[1];
    result(2, 3) = destiny[2];
    return result;
}

Mtx4f scale_general(const Vec4f& scale_) {
    Mtx4f result(Mtx4f::eye());
    result(0, 0) = scale_[0];
    result(1, 1) = scale_[1];
    result(2, 2) = scale_[2];
    return result;
}

#endif