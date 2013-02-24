#pragma once
#ifndef KTN_MATH_MATRIX_H_
#define KTN_MATH_MATRIX_H_

#include "math.h"

namespace ktn { namespace math {

/**
 * @brief
 * MxN Matrix
 */
template<class T, int M, int N=M>
class Matrix
{
    template<class FT, int FM, int FN>
    friend std::ostream& operator<<(std::ostream& os, const Matrix<FT, FM, FN>& m);
    //template<class FT, int FM, int FN>
    //friend std::istream& operator>>(std::istream& is, Matrix<FT, FM, FN>& m);

public:
    Matrix();
    Matrix(const T& t);
    Matrix(const Matrix<T, M, N>& b);
    virtual ~Matrix();

    static const Matrix<T, M, N> ZERO;
    static const Matrix<T, M, N> ONE;

    int row() const { return M; }
    int col() const { return N; }

    T& at(int x, int y);
    Matrix<T, N, M> t() const;

    bool operator!() const;
    Matrix<T, M, N> operator+() const;
    Matrix<T, M, N> operator-() const;

    Matrix<T, M, N> operator+(const Matrix<T, M, N>& m);
    Matrix<T, M, N> operator-(const Matrix<T, M, N>& m);
    Matrix<T, M, N> operator*(const T& n);

    template<int K>
    Matrix<T, K, N> operator*(const Matrix<T, K, M>& m);

    Matrix<T, M, N>& operator=(const Matrix<T, M, N>& m);
    Matrix<T, M, N>& operator+=(const Matrix<T, M, N>& m);
    Matrix<T, M, N>& operator-=(const Matrix<T, M, N>& m);
    Matrix<T, M, N>& operator*=(const T& n);

    bool operator==(const Matrix<T, M, N>& m) const;
    bool operator!=(const Matrix<T, M, N>& m) const;

    //private:
    T** data_;
};

template<class T, int M, int N>
const Matrix<T, M, N> Matrix<T, M, N>::ZERO;

template<class T, int M, int N>
const Matrix<T, M, N> Matrix<T, M, N>::ONE(T(1));

template<class T, int M, int N>
Matrix<T, M, N>::Matrix()
{
    data_ = new T*[N];
    if (!data_) { return; }
    for (int y = 0; y < N; ++y) {
        data_[y] = new T[M];
        if (!data_[y]) { return; }
        for (int x = 0; x < M; ++x) {
            data_[y][x] = T();
        }
    }
}

template<class T, int M, int N>
Matrix<T, M, N>::Matrix(const T& t)
{
    try {
        data_ = new T*[N];
    } catch (std::bad_alloc) {
        data_ = nullptr;
        return;
    }
    for (int y = 0; y < N; ++y) {
        try {
            data_[y] = new T[M];
        } catch (std::bad_alloc) {
            data_[y] = nullptr;
            return;
        }
        for (int x = 0; x < M; ++x) {
            if (x == y) {
                data_[y][x] = t;
            } else {
                data_[y][x] = T();
            }
        }
    }
}

template<class T, int M, int N>
Matrix<T, M, N>::Matrix(const Matrix<T, M, N>& b)
{
    if (this == &b) { return; }
    try {
        data_ = new T*[N];
    } catch (std::bad_alloc) {
        data_ = nullptr;
        return;
    }
    for (int y = 0; y < N; ++y) {
        try {
            data_[y] = new T[M];
        } catch (std::bad_alloc) {
            data_[y] = nullptr;
            return;
        }
        for (int x = 0; x < M; ++x) {
            data_[y][x] = b.data_[y][x];
        }
    }
}

template<class T, int M, int N>
Matrix<T, M, N>::~Matrix()
{
    for (int y = 0; y < N; ++y) {
        delete[] data_[y];
    }
    delete[] data_;
}

/**
* @param x from 1 to M
* @param y from 1 to N
*/
template<class T, int M, int N>
T& Matrix<T, M, N>::at(int x, int y)
{
    if (x < 1) { x = 1; } else if (x > M) { x = M; }
    if (y < 1) { y = 1; } else if (y > N) { y = N; }
    return data_[y - 1][x - 1];
}

/**
* Transpose
*/
template<class T, int M, int N>
Matrix<T, N, M> Matrix<T, M, N>::t() const
{
    Matrix<T, N, M> m;
    for (int y = 0; y < N; ++y) {
        for (int x = 0; x < M; ++x) {
            m.data_[x][y] = data_[y][x];
        }
    }
    return m;
}

template<class T, int M, int N>
std::ostream& operator<<(std::ostream& os, const Matrix<T, M, N>& m)
{
    for (int y = 0; y < N; ++y) {
        for (int x = 0; x < M; ++x) {
            os << m.data_[y][x] << ",";
        }
        os << std::endl;
    }
    return os;
}
/*
template<class T, int M, int N>
std::istream& operator<<(std::istream& is, Matrix<T, M, N>& c)
{
return is;
}
*/

template<class T, int M, int N>
bool Matrix<T, M, N>::operator!() const
{
    for (int y = 0; y < N; ++y) {
        for (int x = 0; x < M; ++x) {
            if (!!data_[y][x]) { return false; }
        }
    }
    return true;
}

template<class T, int M, int N>
Matrix<T, M, N> Matrix<T, M, N>::operator+() const
{
    return *this;
}

template<class T, int M, int N>
Matrix<T, M, N> Matrix<T, M, N>::operator-() const
{
    Matrix<T, M, N> temp;
    for (int y = 0; y < N; ++y) {
        for (int x = 0; x < M; ++x) {
            temp.data_[y][x] = -temp.data_[y][x];
        }
    }
    return temp;
}

template<class T, int M, int N>
Matrix<T, M, N>& Matrix<T, M, N>::operator=(const Matrix<T, M, N>& b)
{
    if (this == &b) { return *this; }
    for (int y = 0; y < N; ++y) {
        for (int x = 0; x < M; ++x) {
            data_[y][x] = b.data_[y][x];
        }
    }
    return *this;
}

template<class T, int M, int N>
Matrix<T, M, N> Matrix<T, M, N>::operator+(const Matrix<T, M, N>& b)
{
    Matrix<T, M, N> m;
    for (int y = 0; y < N; ++y) {
        for (int x = 0; x < M; ++x) {
            m.data_[y][x] = data_[y][x] + b.data_[y][x];
        }
    }
    return m;
}

template<class T, int M, int N>
Matrix<T, M, N> Matrix<T, M, N>::operator-(const Matrix<T, M, N>& b)
{
    Matrix<T, M, N> m;
    for (int y = 0; y < N; ++y) {
        for (int x = 0; x < M; ++x) {
            m.data_[y][x] = data_[y][x] - b.data_[y][x];
        }
    }
    return m;
}

template<class T, int M, int N>
Matrix<T, M, N> Matrix<T, M, N>::operator*(const T& n)
{
    Matrix<T, M, N> m;
    for (int y = 0; y < N; ++y) {
        for (int x = 0; x < M; ++x) {
            m.data_[y][x] = data_[y][x] * n;
        }
    }
    return m;
}

template<class T, int M, int N>
template<int K>
Matrix<T, K, N> Matrix<T, M, N>::operator*(const Matrix<T, K, M>& b)
{
    Matrix<T, K, N> m;
    for (int y = 0; y < N; ++y) {
        for (int x = 0; x < K; ++x) {
            m.data_[y][x] = T();
            for (int i = 0; i < M; ++i) {
                m.data_[y][x] += data_[y][i] * b.data_[i][x];
            }
        }
    }
    return m;
}

template<class T, int M, int N>
Matrix<T, M, N>& Matrix<T, M, N>::operator+=(const Matrix<T, M, N>& b)
{
    *this = *this + b;
    return *this;
}

template<class T, int M, int N>
Matrix<T, M, N>& Matrix<T, M, N>::operator-=(const Matrix<T, M, N>& b)
{
    *this = *this - b;
    return *this;
}

template<class T, int M, int N>
Matrix<T, M, N>& Matrix<T, M, N>::operator*=(const T& n)
{
    *this = *this * n;
    return *this;
}

template<class T, int M, int N>
bool Matrix<T, M, N>::operator==(const Matrix<T, M, N>& b) const
{
    for (int y = 0; y < N; ++y) {
        for (int x = 0; x < M; ++x) {
            if (data_[y][x] != b.data_[y][x]) { return false; }
        }
    }
    return true;
}

template<class T, int M, int N>
bool Matrix<T, M, N>::operator!=(const Matrix<T, M, N>& b) const
{
    for (int y = 0; y < N; ++y) {
        for (int x = 0; x < M; ++x) {
            if (data_[y][x] == b.data_[y][x]) { return false; }
        }
    }
    return true;
}


//template<class T, int M, int N=M>
//class Matrix_ : public Matrix<T, M, N> {
//public:
//  T det();
//  Matrix_<T, M - 1, N - 1> cof(int i, int j);
//
//  //T** data_;
//};
//
///**
// * Cofactor
// */
//template<class T, int M, int N>
//Matrix_<T, M - 1, N - 1> Matrix_<T, M, N>::cof(int i, int j) {
//  Matrix_<T, M - 1, N - 1> m;
//  for (int y = 0, b = 0; y < N; ++y) {
//    if (y == j - 1) { continue; }
//    for (int x = 0, a = 0; x < M; ++x) {
//      if (x == i - 1) { continue; }
//      m.data_[b][a] = data_[y][x];
//      ++a;
//    }
//    ++b;
//  }
//  return m;
//}
//
///**
// * Determinant
// */
//template<class T, int M, int N>
//T Matrix_<T, M, N>::det() {
//  int x = M;
//  T t = 0;
//  while (x--) {
//    if (data_[0][x] != 0) {
//      if ((x & 1) == 0) {
//        t += this->cof(x+1, 1).det() * data_[0][x];
//      } else {
//        t -= this->cof(x+1, 1).det() * data_[0][x];
//      }
//    }
//  }
//  return t;
//}
//
//template<class T>
//class Matrix_<T, 2, 2> : public Matrix<T, 2, 2> {
//public:
//  T det();
//  Matrix_<T, 1, 1> cof(int i, int j);
//
//  //T** data_;
//};
//
//template<class T>
//Matrix_<T, 1, 1> Matrix_<T, 2, 2>::cof(int i, int j) {
//  Matrix_<T, 1, 1> m;
//  m.data_[0][0] = data_[(i == 1 ? 0 : 1)][(j == 1 ? 0 : 1)];
//  return m;
//}
//
//template<class T>
//T Matrix_<T, 2, 2>::det() {
//  return abs(data_[0][0] * data_[1][1] - data_[1][0] * data_[0][1]);
//}
}} // namespace ktn math
#endif // KTN_MATH_MATRIX_H_