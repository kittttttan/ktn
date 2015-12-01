#pragma once

/*!
 @file  math/vector.inl
 @brief inline functions for Vector
 */

namespace ktn { namespace math {

template<class T, int N>
const Vector<T, N> Vector<T, N>::ZERO;

template<class T, int N>
Vector<T, N>::Vector()
{
    try {
        data_ = new T[N];
    } catch (std::bad_alloc) {
        data_ = nullptr;
        return;
    }
    for (int i = 0; i < N; ++i) {
        data_[i] = T();
    }
}

template<class T, int N>
Vector<T, N>::Vector(const Vector<T, N>& b)
{
    try {
        data_ = new T[N];
    } catch (std::bad_alloc) {
        data_ = nullptr;
        return;
    }
    for (int i = 0; i < N; ++i) {
        data_[i] = b.data_[i];
    }
}

template<class T, int N>
Vector<T, N>& Vector<T, N>::operator=(const Vector<T, N>& b)
{
    if (this == &b) { return *this; }
    for (int i = 0; i < N; ++i) {
        data_[i] = b.data_[i];
    }

    return *this;
}

template<class T, int N>
Vector<T, N>::~Vector()
{
    delete[] data_;
}

template<class T, int N>
std::ostream& operator<<(std::ostream& os, const Vector<T, N>& v)
{
    os << "(" << v.data_[0];
    for (int x = 1; x < N; ++x) {
        os << "," << v.data_[x];
    }
    os << ")";
    return os;
}

template<class T, int N>
T& Vector<T, N>::operator[](int index)
{
    return data_[index];
}

template<class T, int N>
bool Vector<T, N>::operator!() const
{
    for (int i = 0; i < N; ++i) {
        if (!!data_[i]) { return false; }
    }
    return true;
}

template<class T, int N>
Vector<T, N> Vector<T, N>::operator+() const
{
    return *this;
}

template<class T, int N>
Vector<T, N> Vector<T, N>::operator-() const
{
    Vector<T, N> v;
    for (int i = 0; i < N; ++i) {
        v.data_[i] = -data_[i];
    }
    return v;
}

template<class T, int N>
Vector<T, N> Vector<T, N>::operator+(const Vector<T, N>& v)
{
    Vector<T, N> w;
    for (int i = 0; i < N; ++i) {
        w.data_[i] = data_[i] + v.data_[i];
    }
    return w;
}

template<class T, int N>
Vector<T, N> Vector<T, N>::operator-(const Vector<T, N>& v)
{
    Vector<T, N> w;
    for (int i = 0; i < N; ++i) {
        w.data_[i] = data_[i] - v.data_[i];
    }
    return w;
}

template<class T, int N>
Vector<T, N> Vector<T, N>::operator*(const T& n)
{
    Vector<T, N> w;
    for (int i = 0; i < N; ++i) {
        w.data_[i] = data_[i] * n;
    }
    return w;
}

/*!
 Dot product
 */
template<class T, int N>
T Vector<T, N>::operator*(const Vector<T, N>& v)
{
    T t = 0;
    for (int i = 0; i < N; ++i) {
        t += data_[i] * v.data_[i];
    }
    return t;
}

template<class T, int N>
Vector<T, N>& Vector<T, N>::operator+=(const Vector<T, N>& b)
{
    for (int i = 0; i < N; ++i) {
        data_[i] += b.data_[i];
    }

    return *this;
}

template<class T, int N>
Vector<T, N>& Vector<T, N>::operator-=(const Vector<T, N>& b)
{
    for (int i = 0; i < N; ++i) {
        data_[i] -= b.data_[i];
    }

    return *this;
}

template<class T, int N>
Vector<T, N>& Vector<T, N>::operator*=(const T& n)
{
    for (int i = 0; i < N; ++i) {
        data_[i] *= n;
    }

    return *this;
}

template<class T, int N>
bool Vector<T, N>::operator==(const Vector<T, N>& v) const
{
    for (int i = 0; i < N; ++i) {
        if (data_[i] != v.data_[i]) { return false; }
    }
    return true;
}

template<class T, int N>
bool Vector<T, N>::operator!=(const Vector<T, N>& v) const
{
    for (int i = 0; i < N; ++i) {
        if (data_[i] == v.data_[i]) { return false; }
    }
    return true;
}

}} // namespace ktn math
