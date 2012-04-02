#include "../math/test.h"

#include <gtest/gtest.h>
#pragma comment(lib, "gtestd.lib")
#pragma comment(lib, "gtest_maind.lib")

using namespace ktn::math;

TEST(ULong, Basic) {
	ULong a(7777777), b(10000);
	char str[64];

	a.cstr(str);
	EXPECT_STREQ("7777777", str);

	b.cstr(str);
	EXPECT_STREQ("10000", str);

	EXPECT_TRUE(a > b);

	(a+b).cstr(str);
	EXPECT_STREQ("7787777", str);

	(a-b).cstr(str);
	EXPECT_STREQ("7767777", str);

	(a*b).cstr(str);
	EXPECT_STREQ("77777770000", str);

	(a/b).cstr(str);
	EXPECT_STREQ("777", str);

	(a%b).cstr(str);
	EXPECT_STREQ("7777", str);
}
