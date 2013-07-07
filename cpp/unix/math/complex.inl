#pragma once

/*!
 @file  math/complex.inl
 @brief inline functions for Complex
 */

namespace ktn { namespace math {

template<class T>
const Complex<T> Complex<T>::ZERO(0);

template<class T>
const Complex<T> Complex<T>::ONE(1);

template<class T>
std::ostream& operator<<(std::ostream& os, const Complex<T>& c)
{
    return os << c.r_ <<
        (c.i_ < 0 ? "" : "+") << c.i_ << "J";
}
/*
template<class T>
std::istream& operator<<(std::istream& is, Complex<T>& c)
{
    return is;
}
*/
template<class T>
void Complex<T>::out(bool br) const
{
    std::cout << r_ <<
        (i_ < 0 ? "" : "+") << i_ << "J";
    if (br) { std::cout << std::endl; }
}

template<class T>
Complex<T> Complex<T>::conj()
{
    Complex<T> temp;
    temp.i_ = -temp.i_;
    return temp;
}

template<class T>
bool Complex<T>::operator!() const
{
    return r_ == 0 && i_ == 0;
}

template<class T>
Complex<T> Complex<T>::operator+() const
{
    return *this;
}

template<class T>
Complex<T> Complex<T>::operator-() const
{
    Complex<T> temp;
    temp.r_ = -temp.r_;
    temp.i_ = -temp.i_;
    return temp;
}

template<class T>
Complex<T>& Complex<T>::operator++()
{
    *this = *this + ONE;
    return *this;
}

template<class T>
Complex<T> Complex<T>::operator--(int)
{
    const Complex<T> tmp(*this);
    --*this;
    return tmp;
}

template<class T>
Complex<T>& Complex<T>::operator--()
{
    *this = *this + ONE;
    return *this;
}

template<class T>
Complex<T> Complex<T>::operator++(int)
{
    const Complex<T> tmp(*this);
    ++*this;
    return tmp;
}

template<class T>
Complex<T>& Complex<T>::operator=(const Complex<T>& b)
{
    if (this == &b) { return *this; }
    r_ = b.r_;
    i_ = b.i_;
    return *this;
}

template<class T>
Complex<T> Complex<T>::operator+(const Complex<T>& b)
{
    return Complex(r_ + b.r_, i_ + b.i_);
}

template<class T>
Complex<T> Complex<T>::operator-(const Complex<T>& b)
{
    return Complex(r_ - b.r_, i_ - b.i_);
}

template<class T>
Complex<T> Complex<T>::operator*(const Complex<T>& b)
{
    return Complex(r_ * b.r_ - i_ * b.i_, r_ * b.i_ + i_ * b.r_);
}

template<class T>
Complex<T> Complex<T>::operator/(const Complex<T>& b)
{
    T denom = b.r_ * b.r_ + b.i_ * b.i_;
    return Complex((r_ * b.r_ + i_ * b.i_) / denom,
        (i_ * b.r_ + r_ * b.i_) / denom);
}

template<class T>
Complex<T>& Complex<T>::operator+=(const Complex<T>& b)
{
    *this = *this + b;
    return *this;
}

template<class T>
Complex<T>& Complex<T>::operator-=(const Complex<T>& b)
{
    *this = *this - b;
    return *this;
}

template<class T>
Complex<T>& Complex<T>::operator*=(const Complex<T>& b)
{
    *this = *this * b;
    return *this;
}

template<class T>
Complex<T>& Complex<T>::operator/=(const Complex<T>& b)
{
    *this = *this / b;
    return *this;
}

template<class T>
bool Complex<T>::operator==(const Complex<T>& b) const
{
    return r_ == b.r_ && i_ == b.i_;
}

template<class T>
bool Complex<T>::operator!=(const Complex<T>& b) const
{
    return r_ != b.r_ || i_ != b.i_;
}

}} // namespace ktn math
