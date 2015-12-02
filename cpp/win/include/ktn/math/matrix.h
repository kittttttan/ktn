#pragma once
#ifndef KTN_MATH_MATRIX_H_
#define KTN_MATH_MATRIX_H_

#include "ktn/math/math.h"

namespace ktn { namespace math {

/*!
 @brief
 MxN Matrix
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

}} // namespace ktn math

#include "matrix.inl"

#endif // KTN_MATH_MATRIX_H_