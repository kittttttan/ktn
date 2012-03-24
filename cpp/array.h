#ifndef KTN_ARRAY_H_
#define KTN_ARRAY_H_

/**
 * @file  array.h
 * @brief Array
 */
#include <iostream>
#include <stdio.h>

namespace ktn {

template<class T>
inline void swap(T& a, T& b) {
	T t = a;
	a = b;
	b = t;
}

/**
 * @brief Array
 */
template<class T>
class Array {

template<class FT>
friend std::ostream& operator<<(std::ostream& os, const Array<FT>& a);
//template<class FT>
//friend std::istream& operator>>(std::istream &is, Array<FT>& a);

public:
	Array() : array_(nullptr), length_(0) {}
	explicit Array(int n, bool init=true);
	Array(const T* arr, int length);
	Array(const Array<T>& a);
	~Array() { delete [] array_; }

	int length() { return length_; }

	void reverse();

	T& operator[](int index);
	bool operator!() const;

	Array<T> operator+(const Array<T>& a) const;

	Array<T>& operator=(const Array<T>& a);

	bool operator==(const Array<T>& a) const;
	bool operator!=(const Array<T>& a) const;

private:
	T* array_;
	int length_;
};

template<class T>
Array<T>::Array(int n, bool init) : length_(n) {
	array_ = new T[n];
	if (!array_) {
		fprintf(stderr, "Falied new. %s:%d\n",__FILE__,__LINE__);
		return;
	}
	if (!init) { return; }
	for (int i = 0; i < n; ++i) {
		array_[i] = T();
	}
}

template<class T>
Array<T>::Array(const T* arr, int length) {
	if (length < 1) {
		length_ = 0;
		array_ = nullptr;
		return;
	}
	length_ = length;
	array_ = new T[length_];
	if (!array_) {
		fprintf(stderr, "Falied new. %s:%d\n",__FILE__,__LINE__);
		return;
	}
	for (int i = 0; i < length_; ++i) {
		array_[i] = arr[i];
	}
}

template<class T>
Array<T>::Array(const Array<T>& a) : length_(a.length_) {
	array_ = new T[length_];
	if (!array_) {
		fprintf(stderr, "Falied new. %s:%d\n",__FILE__,__LINE__);
		return;
	}
	for (int i = 0; i < length_; ++i) {
		array_[i] = a.array_[i];
	}
}

template<class T>
Array<T>& Array<T>::operator=(const Array<T>& b) {
	if (this == &b) { return *this; }
	if (length_ != b.length_) {
		length_ = b.length_;
		delete[] array_;
	}
	for (int i = 0; i < length_; ++i) {
		array_[i] = b.array_[i];
	}

	return *this;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const Array<T>& a) {
	std::cout << "[";
	if (a.length_ > 0) {
		std::cout << a.array_[0];
		for (int i = 1; i < a.length_; ++i) {
			std::cout << "," << a.array_[i];
		}
	}
	std::cout << "]";
	return os;
}
/*
template<class T>
std::istream& operator<<(std::istream& is, Array<T>& a) {
	return is;
}
*/

template<class T>
bool Array<T>::operator!() const {
	return length_ < 1;
}

template<class T>
T& Array<T>::operator[](int index) {
	if (index < 0) {
		index += length_;
	}
	if (index > length_) {
		fprintf(stderr, "Warn: index is too large. %s:%d:\n", __FILE__, __LINE__);
		index = length_ - 1;
	} else if (index < 0) {
		fprintf(stderr, "Warn: index is too small. %s:%d:\n", __FILE__, __LINE__);
		index = 0;
	}
	return array_[index];
}

template<class T>
Array<T> Array<T>::operator+(const Array<T>& a) const {
	Array<T> arr(length_ + a.length_, false);
	for (int i = 0; i < length_; ++i) {
		arr.array_[i] = array_[i];
	}
	for (int i = 0; i < a.length_; ++i) {
		arr.array_[i + length_] = a.array_[i];
	}
	return arr;
}

template<class T>
void Array<T>::reverse() {
	for (int i = 0; i < length_ / 2; ++i) {
		swap(array_[i], array_[length_ - 1 - i]);
	}
}

} // namespace ktn
#endif // KTN_ARRAY_H_