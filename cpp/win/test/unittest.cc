#include "../math/test.h"
#include "../uri.h"

#include <gtest/gtest.h>
#ifdef _M_AMD64
#pragma comment(lib, "gtest_x64d.lib")
#pragma comment(lib, "gtest_main_x64d.lib")
#else
#pragma comment(lib, "gtestd.lib")
#pragma comment(lib, "gtest_maind.lib")
#endif

using namespace ktn;
using namespace ktn::math;

TEST(ULong, Basic) {
    ULong a(7777777), b(10000);
    ULong c("7777777777", 10), d("1000000000", 10);
    char str[64];

    a.cstr(str);
    EXPECT_STREQ("7777777", str);

    b.cstr(str);
    EXPECT_STREQ("10000", str);

    EXPECT_TRUE(a > b);

    (a + b).cstr(str);
    EXPECT_STREQ("7787777", str);

    (a - b).cstr(str);
    EXPECT_STREQ("7767777", str);

    (a * b).cstr(str);
    EXPECT_STREQ("77777770000", str);

    (a / b).cstr(str);
    EXPECT_STREQ("777", str);

    (a % b).cstr(str);
    EXPECT_STREQ("7777", str);

    (c + d).cstr(str);
    EXPECT_STREQ("8777777777", str);

    (c - d).cstr(str);
    EXPECT_STREQ("6777777777", str);

    (c * d).cstr(str);
    EXPECT_STREQ("7777777777000000000", str);

    (c / d).cstr(str);
    EXPECT_STREQ("7", str);

    (c % d).cstr(str);
    EXPECT_STREQ("777777777", str);
}

TEST(ULong, Shift) {
    ULong one(1);
    ULong two(2);
    ULong ls7;
    ULong ls17;
    ULong ls27;
    char str[64];

    ls7 = (one << 7);
    ls7.cstr(str);
    EXPECT_STREQ("128", str);

    ls17 = (one << 17);
    ls17.cstr(str);
    EXPECT_STREQ("131072", str);

    ls27 = (one << 27);
    ls27.cstr(str);
    EXPECT_STREQ("134217728", str);

    two.pow(7).cstr(str);
    EXPECT_STREQ("128", str);

    two.pow(17).cstr(str);
    EXPECT_STREQ("131072", str);

    two.pow(27).cstr(str);
    EXPECT_STREQ("134217728", str);

    (ls7 >> 7).cstr(str);
    EXPECT_STREQ("1", str);

    (ls17 >> 17).cstr(str);
    EXPECT_STREQ("1", str);

    (ls27 >> 27).cstr(str);
    EXPECT_STREQ("1", str);
}

TEST(Uri, Absolute) {
    Uri uri("http://www.yyy.zzz:8000/aaa/bbb/ccc.cgi?KEY=CGI#XYZ");
    EXPECT_STREQ("http:", uri.protocol().c_str());
    EXPECT_STREQ("www.yyy.zzz", uri.hostname().c_str());
    EXPECT_STREQ("8000", uri.port().c_str());
    EXPECT_STREQ("/aaa/bbb/ccc.cgi", uri.pathname().c_str());
    EXPECT_STREQ("?KEY=CGI", uri.search().c_str());
    EXPECT_STREQ("#XYZ", uri.hash().c_str());
    EXPECT_STREQ(".cgi", uri.ext().c_str());
    EXPECT_STREQ("http://www.yyy.zzz:8000/aaa/bbb/ccc.cgi?KEY=CGI#XYZ", uri.str().c_str());

    uri.parse("https://abc.def/#main");
    EXPECT_STREQ("https:", uri.protocol().c_str());
    EXPECT_STREQ("abc.def", uri.hostname().c_str());
    EXPECT_STREQ("", uri.port().c_str());
    EXPECT_STREQ("/", uri.pathname().c_str());
    EXPECT_STREQ("", uri.search().c_str());
    EXPECT_STREQ("#main", uri.hash().c_str());
    EXPECT_STREQ("", uri.ext().c_str());
    EXPECT_STREQ("https://abc.def/#main", uri.str().c_str());
}

TEST(Uri, Relative) {
    Uri uri("/aaa/bbb/ccc.cgi?KEY=CGI#XYZ", Uri::Relative);
    EXPECT_STREQ("", uri.protocol().c_str());
    EXPECT_STREQ("", uri.hostname().c_str());
    EXPECT_STREQ("", uri.port().c_str());
    EXPECT_STREQ("/aaa/bbb/ccc.cgi", uri.pathname().c_str());
    EXPECT_STREQ("?KEY=CGI", uri.search().c_str());
    EXPECT_STREQ("#XYZ", uri.hash().c_str());
    EXPECT_STREQ(".cgi", uri.ext().c_str());
    EXPECT_STREQ("/aaa/bbb/ccc.cgi?KEY=CGI#XYZ", uri.str().c_str());

    uri.parse("/abc.def/#main", Uri::Relative);
    EXPECT_STREQ("", uri.protocol().c_str());
    EXPECT_STREQ("", uri.hostname().c_str());
    EXPECT_STREQ("", uri.port().c_str());
    EXPECT_STREQ("/abc.def/", uri.pathname().c_str());
    EXPECT_STREQ("", uri.search().c_str());
    EXPECT_STREQ("#main", uri.hash().c_str());
    EXPECT_STREQ("", uri.ext().c_str());
    EXPECT_STREQ("/abc.def/#main", uri.str().c_str());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}