#ifndef VECX_H
#define VECX_H

#include "mtxx.h"

template <class T>
struct VecX : public MtxX<T>{
    using MtxX<T>::n;
    using MtxX<T>::data;

    VecX();
    VecX(int n);
    VecX(std::vector<T> mat);
    VecX(int n, std::vector<T> v);
    VecX(const VecX<T> &other);
    VecX(const MtxX<T> &other);

    //Operators overloads
    T &operator()(int i);
    const T &operator()(int i) const;
    T &operator[](int i);
    const T &operator[](int i) const;
    void operator=(VecX<T> other);
    VecX operator+(const VecX<T> &other) const;
    VecX operator-(const VecX<T> &other) const;
    VecX operator-() const;
    VecX operator*(const float val) const;
    VecX operator/(const float val) const;
    VecX operator*(const VecX<T> &other) const = delete;

    //Vector functions
    T norm() const;
    T norm2() const;
    T dot(const VecX<T> &other) const;
    VecX<T> cross(const VecX<T> &other) const;
    VecX<T> unit() const;
    static VecX<T> ones(int n);
    static VecX<T> zeros(int n);

    friend std::ostream &operator<<(std::ostream &os, const VecX<T> &v) {
        for (int i = 0; i < v.n; i++) {
            os << std::setprecision(3) << std::fixed <<  v.data[i][0] << '\t';
        }
        return os;
    }
};

template <class T>
VecX<T>::VecX() : MtxX<T>(){}

template <class T>
VecX<T>::VecX(int n) : MtxX<T>(n, 1){}

template <class T>
VecX<T>::VecX(std::vector<T> mat){
    MtxX<T>::resize(mat.size(), 1);
    for (int i = 0; i < mat.size(); i++){
        data[i][0] = mat[i];
    }
}

template <class T>
VecX<T>::VecX(int n, std::vector<T> v) : MtxX<T>(n, 1, v){}

template <class T>
VecX<T>::VecX(const VecX<T> &other) : MtxX<T>(other){}

template <class T>
VecX<T>::VecX(const MtxX<T> &other) : MtxX<T>(other){}

template <class T>
T &VecX<T>::operator()(int i){
    if (i < 0 || i >= n){
        throw std::out_of_range("Index out of bounds");
    }
    return data[i][0];
}

template <class T>
const T &VecX<T>::operator()(int i) const{
    if (i < 0 || i >= n){
        throw std::out_of_range("Index out of bounds");
    }
    return data[i][0];
}

template <class T>
T &VecX<T>::operator[](int i){
    if (i < 0 || i >= n){
        throw std::out_of_range("Index out of bounds");
    }
    return data[i][0];
}

template <class T>
const T &VecX<T>::operator[](int i) const{
    if (i < 0 || i >= n){
        throw std::out_of_range("Index out of bounds");
    }
    return data[i][0];
}

template <class T>
void VecX<T>::operator=(VecX<T> other){
    MtxX<T>::operator=(other);
}

template <class T>
VecX<T> VecX<T>::operator+(const VecX<T> &other) const {
    return VecX(MtxX<T>::operator+(other));
}

template <class T>
VecX<T> VecX<T>::operator-(const VecX<T> &other) const {
    return VecX(MtxX<T>::operator-(other));
}

template <class T>
VecX<T> VecX<T>::operator-() const {
    return VecX(MtxX<T>::operator-());
}

template <class T>
VecX<T> VecX<T>::operator*(const float val) const {
    return VecX(MtxX<T>::operator*(val));
}

template <class T>
VecX<T> VecX<T>::operator/(const float val) const {
    return VecX(MtxX<T>::operator/(val));
}

//Vector functions
template <class T>
T VecX<T>::norm() const {
    return sqrt(this->dot(*this));
}

template <class T>
T VecX<T>::dot(const VecX<T> &other) const {
    return (other.trp()*(*this))(0,0);
}

template <class T>
VecX<T> VecX<T>::cross(const VecX<T> &other) const {
    if (n != other.n){
        throw std::runtime_error("Matrices cannot be multiplied.");
    }
    if (n == 4){ // ONLY FOR GRAPHICS COMPUTING
        return VecX(4, {data[1][0]*other.data[2][0] - data[2][0]*other.data[1][0],
                        data[2][0]*other.data[0][0] - data[0][0]*other.data[2][0],
                        data[0][0]*other.data[1][0] - data[1][0]*other.data[0][0], 1});
    }
    if (n != 3){
        throw std::runtime_error("Only 3D vectors are supported.");
    }
    return VecX<T>(3, {data[1][0]*other.data[2][0] - data[2][0]*other.data[1][0],
                        data[2][0]*other.data[0][0] - data[0][0]*other.data[2][0],
                        data[0][0]*other.data[1][0] - data[1][0]*other.data[0][0]});
}

template <class T>
T VecX<T>::norm2() const {
    return this->dot(*this);
}

template <class T>
VecX<T> VecX<T>::unit() const {
    return VecX<T>(*this / this->norm());
}

template<class T>
VecX<T> VecX<T>::zeros(int n) {
    return VecX<T>(MtxX<T>::zeros(n, 1));
}
    
template<class T>
VecX<T> VecX<T>::ones(int n) {
    return VecX<T>(MtxX<T>::ones(n, 1));
}

#endif