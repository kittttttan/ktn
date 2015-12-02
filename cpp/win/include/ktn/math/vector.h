#pragma once
#ifndef KTN_MATH_VECTOR_H_
#define KTN_MATH_VECTOR_H_

namespace ktn { namespace math {

/*!
 @brief Vector
 */
template<class T, int N>
class Vector
{
    template<class FT, int FN>
    friend std::ostream& operator<<(std::ostream& os, const Vector<FT, FN>& v);
    //template<class FT, int FN>
    //friend std::istream& operator>>(std::istream& is, Vector<FT, FN>& v);

public:
    static const Vector<T, N> ZERO;

    Vector();
    Vector(const Vector<T, N>& b);
    ~Vector();

    int dimension() const { return N; }

    T& operator[](int index);
    bool operator!() const;
    Vector<T, N> operator+() const;
    Vector<T, N> operator-() const;

    Vector<T, N> operator+(const Vector<T, N>& v);
    Vector<T, N> operator-(const Vector<T, N>& v);
    Vector<T, N> operator*(const T& n);
    T operator*(const Vector<T, N>& v);

    Vector<T, N>& operator=(const Vector<T, N>& v);
    Vector<T, N>& operator+=(const Vector<T, N>& v);
    Vector<T, N>& operator-=(const Vector<T, N>& v);
    Vector<T, N>& operator*=(const T& n);

    bool operator==(const Vector<T, N>& v) const;
    bool operator!=(const Vector<T, N>& v) const;

private:
    T* data_;
};

}} // namespace ktn math

#include "ktn/math/vector.inl"

#endif // KTN_MATH_VECTOR_H_
