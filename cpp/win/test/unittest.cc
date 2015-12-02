#include <ktn/math/ulong.h>
#include <ktn/uri.h>

#include <gtest/gtest.h>
#ifdef _M_AMD64
#  if _DEBUG
#pragma comment(lib, "gtest_x64d.lib")
#pragma comment(lib, "gtest_main_x64d.lib")
#  else
#pragma comment(lib, "gtest_x64.lib")
#pragma comment(lib, "gtest_main_x64.lib")
#  endif
#else
#  if _DEBUG
#pragma comment(lib, "gtestd.lib")
#pragma comment(lib, "gtest_maind.lib")
#  else
#pragma comment(lib, "gtest.lib")
#pragma comment(lib, "gtest_main.lib")
#  endif
#endif

using namespace ktn;
using namespace ktn::math;

TEST(ULong, Basic) {
    ULong a(7777777), b(10000);
    ULong c("7777777777", 10), d("1000000000", 10);
    char str[80];

    ULong::ZERO.cstr(str);
    EXPECT_STREQ("0", str);
    EXPECT_TRUE(ULong::ZERO.isEven());
    EXPECT_FALSE(ULong::ZERO.isOdd());

    ULong::ONE.cstr(str);
    EXPECT_STREQ("1", str);
    EXPECT_FALSE(ULong::ONE.isEven());
    EXPECT_TRUE(ULong::ONE.isOdd());

    a.cstr(str);
    EXPECT_STREQ("7777777", str);

    b.cstr(str);
    EXPECT_STREQ("10000", str);

    EXPECT_TRUE(ULong::ZERO == ULong("", 10));

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

    ULong e("1000000000000000000", 10);
    e.cstr(str);
    EXPECT_STREQ("1000000000000000000", str);

    (d*d).cstr(str);
    EXPECT_STREQ("1000000000000000000", str);

    d.square().cstr(str);
    EXPECT_STREQ("1000000000000000000", str);

    c.sqrt().cstr(str);
    EXPECT_STREQ("88191", str);

    a.gcd(c).cstr(str);
    EXPECT_STREQ("7", str);

    a.gcdBin(c).cstr(str);
    EXPECT_STREQ("7", str);
}

TEST(ULong, Shift) {
    ULong one(1);
    ULong two(2);
    ULong ls7;
    ULong ls17;
    ULong ls27;
    ULong ls37;
    ULong ls47;
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

    ls37 = (one << 37);
    ls37.cstr(str);
    EXPECT_STREQ("137438953472", str);

    ls47 = (one << 47);
    ls47.cstr(str);
    EXPECT_STREQ("140737488355328", str);

    two.pow(7).cstr(str);
    EXPECT_STREQ("128", str);

    two.pow(17).cstr(str);
    EXPECT_STREQ("131072", str);

    two.pow(27).cstr(str);
    EXPECT_STREQ("134217728", str);

    two.pow(37).cstr(str);
    EXPECT_STREQ("137438953472", str);

    two.pow(47).cstr(str);
    EXPECT_STREQ("140737488355328", str);

    (ls7 >> 7).cstr(str);
    EXPECT_STREQ("1", str);

    (ls17 >> 17).cstr(str);
    EXPECT_STREQ("1", str);

    (ls27 >> 27).cstr(str);
    EXPECT_STREQ("1", str);

    (ls37 >> 37).cstr(str);
    EXPECT_STREQ("1", str);

    (ls47 >> 47).cstr(str);
    EXPECT_STREQ("1", str);
}

TEST(ULong, Factorial) {
    char str[128];

    ULong::factorial(0).cstr(str);
    EXPECT_STREQ("1", str);

    ULong::factorial(1).cstr(str);
    EXPECT_STREQ("1", str);

    ULong::factorial(2).cstr(str);
    EXPECT_STREQ("2", str);

    ULong::factorial(3).cstr(str);
    EXPECT_STREQ("6", str);

    ULong::factorial(10).cstr(str);
    EXPECT_STREQ("3628800", str);

    ULong::factorial(17).cstr(str);
    EXPECT_STREQ("355687428096000", str);
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