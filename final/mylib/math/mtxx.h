#ifndef MTXX_H
#define MTXX_H

#include <initializer_list>
#include <vector>
#include <iostream>
#include <cmath>

template<class U>
struct MtxX {
    using mat = MtxX<U>;
    using vect = std::vector<U>;
    std::vector<vect> data;
    int n, m;

    MtxX();
    MtxX(int n, int m);
    MtxX(std::vector<std::vector<U>> mat);
    MtxX(int n, int m, vect v);
    MtxX(const mat &other);

    void resize(int n, int m);
    U trace() const;
    U det() const;
    mat trp() const;
    mat inv() const;
    static mat eye(int n);
    static mat zeros(int n, int m);
    static mat ones(int n, int m);
    vect to_vec() const;
    
    //Operators overloads
    U &operator()(int i, int j);
    const U &operator()(int i, int j) const;
    mat operator*(const mat &other) const;
    mat operator*(const U val) const;
    mat operator/(const U val) const;
    mat operator+(const mat &other) const;
    mat operator-(const mat &other) const;
    mat operator-() const;
    void operator=(MtxX<U> other);

    friend std::ostream &operator<<(std::ostream &os, const MtxX<U> &m) {
        for (int i = 0; i < m.n; i++) {
            for (int j = 0; j < m.m; j++) {
                os << m.data[i][j] << '\t';
            }
            os << std::endl;
        }
        return os;
    }
};

// Implementation of the constructors
template<class U>
MtxX<U>::MtxX() : n(0), m(0) {}

template<class U>
MtxX<U>::MtxX(int n, int m) : n(n), m(m), data(n, vect(m)) {}

template<class U>
MtxX<U>::MtxX(std::vector<std::vector<U>> mat) : n(mat.size()), m(mat.begin()->size()), data(mat) {}

template<class U>
MtxX<U>::MtxX(int n, int m, vect v) : n(n), m(m), data(n, vect(m)) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            data[i][j] = v[i * m + j];
        }
    }
}

template<class U>
MtxX<U>::MtxX(const mat &other) : n(other.n), m(other.m), data(other.data) {}

// Methods implementation
template<class U>
void MtxX<U>::resize(int n, int m) {
    this->n = n;
    this->m = m;
    data.resize(n);
    for (int i = 0; i < n; i++) {
        data[i].resize(m);
    }
}

template<class U>
U MtxX<U>::trace() const {
    U sum = 0;
    int i = n < m ? n : m;
    for (int j = 0; j < i; j++) {
        sum += data[j][j];
    }
    return sum;
}

template<class U>
U MtxX<U>::det() const {
    if (n != m) {
        throw std::runtime_error("MtxX::det: Matrix is not square.");
    }
    mat temp(*this);
    U determinant = 1;
    for (int i = 0; i < n; i++) {
        int max_row = i;
        for (int j = i + 1; j < n; j++) {
            if (std::abs(temp(j, i)) > std::abs(temp(max_row, i))) {
                max_row = j;
            }
        }
        if (max_row != i) {
            std::swap(temp.data[i], temp.data[max_row]);
            determinant *= -1;
        }
        U pivot = temp(i, i);
        if (pivot == 0) {
            return 0;
        }
        determinant *= pivot;
        for (int j = i + 1; j < n; j++) {
            U factor = temp(j, i) / pivot;
            for (int k = i; k < n; k++) {
                temp(j, k) -= factor * temp(i, k);
            }
        }
    }
    return determinant;
}

template<class U>
MtxX<U> MtxX<U>::trp() const {
    mat transp(m, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            transp.data[j][i] = data[i][j];
        }
    }
    return transp;
}

template<class U>
MtxX<U> MtxX<U>::inv() const {
    if (n != m) {
        throw std::runtime_error("La matriz debe ser cuadrada para calcular la inversa.");
    }
    U determinant = det();
    if (std::abs(determinant) < 0.0001) { // error of aproximation
        throw std::runtime_error("La matriz no es invertible (determinante igual a cero).");
    }
    mat temp(*this);
    mat identity(n, n);
    for (int i = 0; i < n; i++) {
        identity(i, i) = 1;
    }
    for (int i = 0; i < n; i++) {
        int max_row_index = i;
        for (int j = i + 1; j < n; j++) {
            if (std::abs(temp(j, i)) > std::abs(temp(max_row_index, i))) {
                max_row_index = j;
            }
        }
        if (max_row_index != i) {
            std::swap(temp.data[i], temp.data[max_row_index]);
            std::swap(identity.data[i], identity.data[max_row_index]);
        }
        U pivot = temp(i, i);
        for (int j = 0; j < n; j++) {
            temp(i, j) /= pivot;
            identity(i, j) /= pivot;
        }
        for (int j = 0; j < n; j++) {
            if (j != i) {
                U factor = temp(j, i);
                for (int k = 0; k < n; k++) {
                    temp(j, k) -= factor * temp(i, k);
                    identity(j, k) -= factor * identity(i, k);
                }
            }
        }
    }
    return identity;
}

template<class U>
MtxX<U> MtxX<U>::eye(int n) {
    MtxX<U> res(n, n);
    for (int i = 0; i < n; i++) {
        res.data[i][i] = 1;
    }
    return res;
}

template<class U>
MtxX<U> MtxX<U>::zeros(int n, int m) {
    MtxX<U> res(n, m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            res.data[i][j] = 0;
        }
    }
    return res;
}

template<class U>
MtxX<U> MtxX<U>::ones(int n, int m) {
    MtxX<U> res(n, m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            res.data[i][j] = 1;
        }
    }
    return res;
}

template <class U>
std::vector<U> MtxX<U>::to_vec() const {
    vect vector_;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            vector_.push_back(data[i][j]);
        }
    }
    return vector_;
}

// Operators overloads
template<class U>
U &MtxX<U>::operator()(int i, int j) {
    if (i < 0 || i >= n || j < 0 || j >= m) {
        throw std::out_of_range("Index out of bounds");
    }
    return data[i][j];
}

template<class U>
const U &MtxX<U>::operator()(int i, int j) const {
    if (i < 0 || i >= n || j < 0 || j >= m) {
        throw std::out_of_range("Index out of bounds");
    }
    return data[i][j];
}

template<class U>
MtxX<U> MtxX<U>::operator*(const mat &other) const {
    if (m != other.n) {
        throw std::runtime_error("Matrices cannot be multiplied.");
    }
    MtxX<U> res(n, other.m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < other.m; j++) {
            for (int k = 0; k < m; k++) {
                res.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }
    return res;
}

template<class U>
MtxX<U> MtxX<U>::operator*(const U val) const {
    MtxX<U> res(n, m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            res.data[i][j] = data[i][j] * val;
        }
    }
    return res;
}

template<class U>
MtxX<U> MtxX<U>::operator/(const U val) const {
    MtxX<U> res(n, m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            res.data[i][j] = data[i][j] / val;
        }
    }
    return res;
}

template<class U>
MtxX<U> MtxX<U>::operator+(const mat &other) const {
    MtxX<U> res(n, m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            res.data[i][j] = data[i][j] + other.data[i][j];
        }
    }
    return res;
}

template<class U>
MtxX<U> MtxX<U>::operator-(const mat &other) const {
    MtxX<U> res(n, m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            res.data[i][j] = data[i][j] - other.data[i][j];
        }
    }
    return res;   
}

template <class U>
void MtxX<U>::operator=(MtxX<U> other) {
    this->n = other.n;
    this->m = other.m;
    this->data = other.data;
}

template <class U>
MtxX<U> MtxX<U>::operator-() const {
    return MtxX<U>::zeros(n, m) - *this;
}
#endif