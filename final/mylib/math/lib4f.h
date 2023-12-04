#ifndef LIB4F_H
#define LIB4F_H

#include <iomanip>
#include "vecx.h"

struct Vec4f : public VecX<float>{
    using VecX<float>::n;
    using VecX<float>::data;

    Vec4f() : VecX<float>(4){}
    Vec4f(float x, float y, float z, float w) : VecX<float>(4, {x, y, z, w}){}
    Vec4f(std::vector<float> v) : VecX<float>(4, v){}
    Vec4f(const Vec4f &other) : VecX<float>(other){}
    Vec4f(const VecX<float> &other) : VecX<float>(other){}

    void operator=(Vec4f other);
    Vec4f operator+(const Vec4f &other) const;
    Vec4f operator-(const Vec4f &other) const;
    Vec4f operator-() const;
    Vec4f operator*(const float val) const;
    Vec4f operator/(const float val) const;
    Vec4f unit() const;
    static Vec4f ones();
    static Vec4f zeros();
    Vec4f cross(const Vec4f &other) const;
};

void Vec4f::operator=(Vec4f other){
    VecX<float>::operator=(other);
}

Vec4f Vec4f::operator+(const Vec4f &other) const {
    return Vec4f(VecX<float>::operator+(other));
}

Vec4f Vec4f::operator-(const Vec4f &other) const {
    return Vec4f(VecX<float>::operator-(other));
}

Vec4f Vec4f::operator-() const {
    Vec4f tmp= Vec4f(VecX<float>::operator-());
    tmp[3] = 1.0f; // FOR GRAPHICS COMPUTING
    return tmp;
}

Vec4f Vec4f::operator*(const float val) const {
    return Vec4f(VecX<float>::operator*(val));
}

Vec4f Vec4f::operator/(const float val) const {
    return Vec4f(VecX<float>::operator/(val));
}

Vec4f Vec4f::unit() const {
    Vec4f tmp(*this); // ONLY FOR GRAPHICS COMPUTING
    tmp[3]= 0.0f;
    tmp = tmp / tmp.norm();
    tmp[3] = 1.0f;
    return Vec4f(tmp); //VecX<float>(*this / this->norm())
}

Vec4f Vec4f::ones() {
    return Vec4f(VecX<float>::ones(4));
}

Vec4f Vec4f::zeros() {
    return Vec4f(VecX<float>::zeros(4));
}

Vec4f Vec4f::cross(const Vec4f &other) const {
    return Vec4f(VecX<float>::cross(other));
}

struct Mtx4f : public MtxX<float>{
    using MtxX<float>::n;
    using MtxX<float>::m;
    using MtxX<float>::data;

    Mtx4f() : MtxX<float>(4, 4){}
    Mtx4f(std::vector<std::vector<float>> mat) : MtxX<float>(mat){}
    Mtx4f(std::vector<float> v) : MtxX<float>(4, 4, v){}
    Mtx4f(const Mtx4f &other) : MtxX<float>(other){}
    Mtx4f(const MtxX<float> &other) : MtxX<float>(other){}
    
    Mtx4f trp() const;
    Mtx4f inv() const;
    static Mtx4f eye();
    static Mtx4f zeros();
    static Mtx4f ones();
    Mtx4f operator*(const Mtx4f &other) const;
    Mtx4f operator*(const float val) const;
    Vec4f operator*(const Vec4f &vect) const;
    Mtx4f operator/(const float val) const;
    Mtx4f operator+(const Mtx4f &other) const;
    Mtx4f operator-(const Mtx4f &other) const;
    void operator=(const Mtx4f &other);

    friend std::ostream &operator<<(std::ostream &os, const Mtx4f &m) {
        for (int i = 0; i < m.n; i++) {
            for (int j = 0; j < m.m; j++) {
                os << std::setprecision(3) << std::fixed << m.data[i][j] << '\t';
            }
            os << std::endl;
        }
        return os;
    }
};

// Methods implemented
Mtx4f Mtx4f::trp() const {
    return Mtx4f(MtxX<float>::trp());
}

Mtx4f Mtx4f::inv() const {
    return Mtx4f(MtxX<float>::inv());
}

Mtx4f Mtx4f::eye() {
    return Mtx4f(MtxX<float>::eye(4));
}

Mtx4f Mtx4f::zeros() {
    return Mtx4f(MtxX<float>::zeros(4, 4));
}

Mtx4f Mtx4f::ones() {
    return Mtx4f(MtxX<float>::ones(4, 4));
}

// Operators overloads
Mtx4f Mtx4f::operator*(const Mtx4f &other) const {
    return Mtx4f(MtxX<float>::operator*(other));
}

Mtx4f Mtx4f::operator*(const float val) const {
    return Mtx4f(MtxX<float>::operator*(val));
}

Vec4f Mtx4f::operator*(const Vec4f &vect) const {//°°°°°°°°°°°°
    return Vec4f(MtxX<float>::operator*(vect));
}

Mtx4f Mtx4f::operator/(const float val) const {
    return Mtx4f(MtxX<float>::operator/(val));
}

Mtx4f Mtx4f::operator+(const Mtx4f &other) const {
    return Mtx4f(MtxX<float>::operator+(other));
}

Mtx4f Mtx4f::operator-(const Mtx4f &other) const {
    return Mtx4f(MtxX<float>::operator-(other));
}

void Mtx4f::operator=(const Mtx4f &other) {
    MtxX<float>::operator=(other);
}

#endif // LIB4F_H