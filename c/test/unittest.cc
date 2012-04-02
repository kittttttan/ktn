#include "../math/test.h"

#include <gtest/gtest.h>
#pragma comment(lib, "gtestd.lib")
#pragma comment(lib, "gtest_maind.lib")

TEST(ULong, Basic) {
	char str[256];
	ULong a, b, t;
	ulongInit(&a);
	ulongInit(&b);
	ulongInit(&t);

	ulongNum(&a, 7777777);
	ulongNum(&b, 10000);

	ulongStr(&a, str);
	EXPECT_STREQ("7777777", str);

	ulongStr(&b, str);
	EXPECT_STREQ("10000", str);

	EXPECT_TRUE(ulongGt(&a, &b));

	ulongAdd(&t, &a, &b);
	ulongStr(&t, str);
	EXPECT_STREQ("7787777", str);

	ulongSub(&t, &a, &b);
	ulongStr(&t, str);
	EXPECT_STREQ("7767777", str);

	ulongMul(&t, &a, &b);
	ulongStr(&t, str);
	EXPECT_STREQ("77777770000", str);

	ulongDiv(&t, &a, &b);
	ulongStr(&t, str);
	EXPECT_STREQ("777", str);

	ulongMod(&t, &a, &b);
	ulongStr(&t, str);
	EXPECT_STREQ("7777", str);

	ulongSquare(&t, &b);
	ulongStr(&t, str);
	EXPECT_STREQ("100000000", str);

	ulongSqrt(&t, &b);
	ulongStr(&t, str);
	EXPECT_STREQ("100", str);

	ulongPow(&t, &b, 7);
	ulongStr(&t, str);
	EXPECT_STREQ("10000000000000000000000000000", str);

	ulongLeftShift(&t, &b, 7);
	ulongStr(&t, str);
	EXPECT_STREQ("1280000", str);

	ulongRightShift(&t, &b, 7);
	ulongStr(&t, str);
	EXPECT_STREQ("78", str);

	ulongFree(&a);
	ulongFree(&b);
	ulongFree(&t);
}
