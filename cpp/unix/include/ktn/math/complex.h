#pragma once
#ifndef KTN_MATH_COMPLEX_H_
#define KTN_MATH_COMPLEX_H_

namespace ktn { namespace math {

/*!
 @brief Complex
 */
template<class T>
class Complex
{
    template<class FT>
    friend std::ostream& operator<<(std::ostream& os, const Complex<FT>& c);
    //template<class FT>
    //friend std::istream& operator>>(std::istream& is, Complex<FT>& c);

public:
    static const Complex<T> ZERO;
    static const Complex<T> ONE;

    explicit Complex(T a=0, T b=0) : r_(a), i_(b) {}
    Complex(const Complex<T>& b) : r_(b.r_), i_(b.i_) {}
    ~Complex() {}

    T real() const { return r_; }
    T imag() const { return i_; }

    void out(bool br=true) const;

    Complex<T> conj();

    bool operator!() const;
    Complex<T> operator+() const;
    Complex<T> operator-() const;
    Complex<T>& operator++();
    Complex<T> operator++(int);
    Complex<T>& operator--();
    Complex<T> operator--(int);

    Complex<T> operator+(const Complex<T>& b);
    Complex<T> operator-(const Complex<T>& b);
    Complex<T> operator*(const Complex<T>& b);
    Complex<T> operator/(const Complex<T>& b);

    Complex<T>& operator=(const Complex<T>& b);
    Complex<T>& operator+=(const Complex<T>& b);
    Complex<T>& operator-=(const Complex<T>& b);
    Complex<T>& operator*=(const Complex<T>& b);
    Complex<T>& operator/=(const Complex<T>& b);

    bool operator==(const Complex<T>& b) const;
    bool operator!=(const Complex<T>& b) const;

private:
    T r_;  /**< real */
    T i_;  /**< imaginary */
};

}} // namespace ktn math

#include "complex.inl"

#endif // KTN_MATH_COMPLEX_H_