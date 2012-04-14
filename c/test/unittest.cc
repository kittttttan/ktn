#include "../math/test.h"

#include <gtest/gtest.h>
#pragma comment(lib, "gtestd.lib")
#pragma comment(lib, "gtest_maind.lib")

TEST(Long, Basic) {
	char str[256];
	Long a, b, t;
	longInit(&a);
	longInit(&b);
	longInit(&t);

	longNum(&a, 7777777);
	longNum(&b, 10000);

	longStr(&a, str);
	EXPECT_STREQ("7777777", str);

	longStr(&b, str);
	EXPECT_STREQ("10000", str);

	EXPECT_TRUE(longGt(&a, &b));

	longAdd(&t, &a, &b);
	longStr(&t, str);
	EXPECT_STREQ("7787777", str);

	longSub(&t, &a, &b);
	longStr(&t, str);
	EXPECT_STREQ("7767777", str);

	longMul(&t, &a, &b);
	longStr(&t, str);
	EXPECT_STREQ("77777770000", str);

	longDiv(&t, &a, &b);
	longStr(&t, str);
	EXPECT_STREQ("777", str);

	longMod(&t, &a, &b);
	longStr(&t, str);
	EXPECT_STREQ("7777", str);

	longSquare(&t, &b);
	longStr(&t, str);
	EXPECT_STREQ("100000000", str);

	longSqrt(&t, &b);
	longStr(&t, str);
	EXPECT_STREQ("100", str);

	longPow(&t, &b, 7);
	longStr(&t, str);
	EXPECT_STREQ("10000000000000000000000000000", str);

	longLeftShift(&t, &b, 7);
	longStr(&t, str);
	EXPECT_STREQ("1280000", str);

	longRightShift(&t, &b, 7);
	longStr(&t, str);
	EXPECT_STREQ("78", str);

	longFree(&a);
	longFree(&b);
	longFree(&t);
}
