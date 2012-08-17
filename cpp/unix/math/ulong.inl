#ifndef KTN_MATH_ULONG_INL_
#define KTN_MATH_ULONG_INL_

/**
* @file  math/ulong.inl
* @brief ULong
*/

namespace ktn { namespace math {

/**
 * Fits length.
 */
inline void ULong::norm() {
    while (l_ > 0 && d_[l_ - 1] == 0) { --l_; }
}

inline bool ULong::operator!() const {
    return l_ < 1 || (l_ == 1 && d_[0] == 0);
}

inline ULong ULong::operator+() const {
    return *this;
}

inline ULong& ULong::operator++() {
    *this = *this + ONE;
    return *this;
}

inline ULong ULong::operator--(int) {
    const ULong tmp(*this);
    --*this;
    return tmp;
}

inline ULong& ULong::operator--() {
    *this = *this + ONE;
    return *this;
}

inline ULong ULong::operator++(int) {
    const ULong tmp(*this);
    ++*this;
    return tmp;
}

inline ULong ULong::operator/(const ULong& b) const {
    return divmod(b, false);
}

inline ULong ULong::operator%(const ULong& b) const {
    return divmod(b, true);
}

inline ULong& ULong::operator+=(const ULong& b) {
    *this = *this + b;
    return *this;
}

inline ULong& ULong::operator-=(const ULong& b) {
    *this = *this - b;
    return *this;
}

inline ULong& ULong::operator*=(const ULong& b) {
    *this = *this * b;
    return *this;
}

inline ULong& ULong::operator/=(const ULong& b) {
    *this = *this / b;
    return *this;
}

inline ULong& ULong::operator%=(const ULong& b) {
    *this = *this % b;
    return *this;
}

inline ULong& ULong::operator<<=(ddigit n) {
    *this = *this << n;
    return *this;
}

inline ULong& ULong::operator>>=(ddigit n) {
    *this = *this >> n;
    return *this;
}


inline ULong ULong::operator+(ddigit b) const {
    return *this + ULong(b);
}

inline ULong ULong::operator-(ddigit b) const {
    return *this - ULong(b);
}

inline ULong ULong::operator*(ddigit b) const {
    return *this * ULong(b);
}

inline ULong ULong::operator/(ddigit b) const {
    return *this / ULong(b);
}

inline ULong ULong::operator%(ddigit b) const {
    return *this % ULong(b);
}

inline ULong& ULong::operator+=(ddigit b) {
    *this = *this + b;
    return *this;
}

inline ULong& ULong::operator-=(ddigit b) {
    *this = *this - b;
    return *this;
}

inline ULong& ULong::operator*=(ddigit b) {
    *this = *this * b;
    return *this;
}

inline ULong& ULong::operator/=(ddigit b) {
    *this = *this / b;
    return *this;
}

inline ULong& ULong::operator%=(ddigit b) {
    *this = *this % b;
    return *this;
}

inline bool ULong::operator==(const ULong& b) const {
    return cmp(b) == 0;
}

inline bool ULong::operator!=(const ULong& b) const {
    return cmp(b) != 0;
}

inline bool ULong::operator>(const ULong& b) const {
    return cmp(b) > 0;
}

inline bool ULong::operator<(const ULong& b) const {
    return cmp(b) < 0;
}

inline bool ULong::operator>=(const ULong& b) const {
    return cmp(b) >= 0;
}

inline bool ULong::operator<=(const ULong& b) const {
    return cmp(b) <= 0;
}


inline int ULong::cmp(ddigit b) const {
    return cmp(ULong(b));
}

inline bool ULong::operator==(ddigit b) const {
    return cmp(b) == 0;
}

inline bool ULong::operator!=(ddigit b) const {
    return cmp(b) != 0;
}

inline bool ULong::operator>(ddigit b) const {
    return cmp(b) > 0;
}

inline bool ULong::operator<(ddigit b) const {
    return cmp(b) < 0;
}

inline bool ULong::operator>=(ddigit b) const {
    return cmp(b) >= 0;
}

inline bool ULong::operator<=(ddigit b) const {
    return cmp(b) <= 0;
}

}} // namespace ktn math

#endif // KTN_MATH_ULONG_INL_
