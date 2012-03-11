#ifndef MATHKTN_MATH_H_
#define MATHKTN_MATH_H_

namespace mathktn {

template <typename Integer>
inline Integer gcd(Integer A, Integer B) {
   do {
      const Integer tmp(B);
      B = A % B;
      A = tmp;
   } while (B != 0);

   return A;
}

template <typename Integer>
inline Integer gcdBin(const Integer& A, const Integer& B) {
	if (A < B) { return gcdBin(B, A); }
	Integer x(A), y(B), g(1);
	while ((x & 1) == 0 && (y & 1) == 0) {
		x >>= 1;
		y >>= 1;
		g <<= 1;
	}

	while (x != 0) {
		while ((x & 1) == 0) {
			x >>= 1;
		}
		while ((y & 1) == 0) {
			y >>= 1;
		}

		if (x < y) {
			y = (y - x) >> 1;
		} else {
			x = (x - y) >> 1;
		}
	}

	return g * y;
}

template <typename Integer>
inline Integer lcm(const Integer& A, const Integer& B) {
   Integer ret(A);
   ret /= gcd(A, B);
   ret *= B;
   return ret;
}

} // namespace mathktn
#endif // MATHKTN_MATH_H_