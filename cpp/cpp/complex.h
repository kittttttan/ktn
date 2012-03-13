#ifndef COMPLEX_H_
#define COMPLEX_H_

template<class T>
class Complex {
public:
	explicit Complex(T a=0, T b=0) : r_(a), i_(b) {}
	Complex(const Complex<T>& b) : r_(b.r_), i_(b.i_) {}
	~Complex() {}

	static const Complex<T> ZERO;
	static const Complex<T> ONE;

	T real() { return r_; }
	T imag() { return i_; }

	void out(bool br=true);

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
	Complex<T>& operator%=(const Complex<T>& b);

private:
	T r_;
	T i_;
};

template<class T>
const Complex<T> Complex<T>::ZERO(0);

template<class T>
const Complex<T> Complex<T>::ONE(1);

template<class T>
void Complex<T>::out(bool br) {
	std::cout << r_ <<
		(i_ < 0 ? "" : "+") << i_ << "J";
	if (br) { std::cout << std::endl; }
}

template<class T>
Complex<T> Complex<T>::conj() {
	Complex<T> temp;
	temp.i_ = -temp.i_;
	return temp;
}

template<class T>
bool Complex<T>::operator!() const {
	return r_ == 0 && i_ == 0;
}

template<class T>
Complex<T> Complex<T>::operator+() const {
	return *this;
}

template<class T>
Complex<T> Complex<T>::operator-() const {
	Complex<T> temp;
	temp.r_ = -temp.r_;
	temp.i_ = -temp.i_;
	return temp;
}

template<class T>
Complex<T>& Complex<T>::operator++() {
	*this = *this + ONE;
	return *this;
}

template<class T>
Complex<T> Complex<T>::operator--(int) {
	const ULong tmp(*this);
	--*this;
	return tmp;
}

template<class T>
Complex<T>& Complex<T>::operator--() {
	*this = *this + ONE;
	return *this;
}

template<class T>
Complex<T> Complex<T>::operator++(int) {
	const ULong tmp(*this);
	++*this;
	return tmp;
}

template<class T>
Complex<T>& Complex<T>::operator=(const Complex<T>& b) {
	if (this == &b) { return *this; }
	r_ = b.r_;
	i_ = b.i_;
	return *this;
}

template<class T>
Complex<T> Complex<T>::operator+(const Complex<T>& b) {
	return Complex(r_ + b.r_, i_ + b.i_);
}

template<class T>
Complex<T> Complex<T>::operator-(const Complex<T>& b) {
	return Complex(r_ - b.r_, i_ - b.i_);
}

template<class T>
Complex<T> Complex<T>::operator*(const Complex<T>& b) {
	return Complex(r_ * b.r_ - i_ * b.i_, r_ * b.i_ + i_ * b.r_);
}

template<class T>
Complex<T> Complex<T>::operator/(const Complex<T>& b) {
	T denom = b.r_ * b.r_ + b.i_ * b.i_;
	return Complex((r_ * b.r_ + i_ * b.i_) / denom,
				(i_ * b.r_ + r_ * b.i_) / denom);
}

template<class T>
Complex<T>& Complex<T>::operator+=(const Complex<T>& b) {
	*this = *this + b;
	return *this;
}

template<class T>
Complex<T>& Complex<T>::operator-=(const Complex<T>& b) {
	*this = *this - b;
	return *this;
}

template<class T>
Complex<T>& Complex<T>::operator*=(const Complex<T>& b) {
	*this = *this * b;
	return *this;
}

template<class T>
Complex<T>& Complex<T>::operator/=(const Complex<T>& b) {
	*this = *this / b;
	return *this;
}

#endif // COMPLEX_H_