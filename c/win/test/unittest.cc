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

  longFree(&a);
  longFree(&b);
  longFree(&t);
}

TEST(Long, Shift) {
  char str[256];
  Long one, two, shift, t;
  longInit(&one);
  longInit(&two);
  longInit(&shift);
  longInit(&t);
  
  longNum(&one, 1);
  longNum(&two, 2);

  longPow(&t, &two, 7);
  longStr(&t, str);
  EXPECT_STREQ("128", str);

  longPow(&t, &two, 17);
  longStr(&t, str);
  EXPECT_STREQ("131072", str);

  longPow(&t, &two, 27);
  longStr(&t, str);
  EXPECT_STREQ("134217728", str);

  longLeftShift(&shift, &one, 7);
  longStr(&shift, str);
  EXPECT_STREQ("128", str);

  longRightShift(&t, &shift, 7);
  longStr(&t, str);
  EXPECT_STREQ("1", str);

  longLeftShift(&shift, &one, 17);
  longStr(&shift, str);
  EXPECT_STREQ("131072", str);

  longRightShift(&t, &shift, 17);
  longStr(&t, str);
  EXPECT_STREQ("1", str);

  longLeftShift(&shift, &one, 27);
  longStr(&shift, str);
  EXPECT_STREQ("134217728", str);

  longRightShift(&t, &shift, 27);
  longStr(&t, str);
  EXPECT_STREQ("1", str);

  longFree(&one);
  longFree(&two);
  longFree(&shift);
  longFree(&t);
}

TEST(Long, Signed) {
  char str[256];
  Long a, b, t;
  longInit(&a);
  longInit(&b);
  longInit(&t);

  longNum(&a, 7777777);
  longNum(&b, -10000);

  longStr(&a, str);
  EXPECT_STREQ("7777777", str);

  longStr(&b, str);
  EXPECT_STREQ("-10000", str);

  EXPECT_TRUE(longGt(&a, &b));

  longAdd(&t, &a, &b);
  longStr(&t, str);
  EXPECT_STREQ("7767777", str);

  longSub(&t, &a, &b);
  longStr(&t, str);
  EXPECT_STREQ("7787777", str);

  longMul(&t, &a, &b);
  longStr(&t, str);
  EXPECT_STREQ("-77777770000", str);

  longDiv(&t, &a, &b);
  longStr(&t, str);
  EXPECT_STREQ("-777", str);

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
  EXPECT_STREQ("-10000000000000000000000000000", str);

  longLeftShift(&t, &b, 7);
  longStr(&t, str);
  EXPECT_STREQ("-1280000", str);

  longRightShift(&t, &b, 7);
  longStr(&t, str);
  EXPECT_STREQ("-78", str);

  longFree(&a);
  longFree(&b);
  longFree(&t);
}
