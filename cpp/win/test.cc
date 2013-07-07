/*!
 @file  cpp/win/test.cc
 @brief test for String
 */
#include "dbg.h"
#include "httpserver.h"
#include "logger.h"
#include "loggerw.h"
#include "string.h"
#include "stringw.h"
#include "date.h"

#include <cstdio>
#include <cstdlib>
#include <clocale>
#include <sstream>

using namespace ktn;

void stringWTest()
{
    setlocale(LC_CTYPE, "");

    LOGGER_INIT;
    LOGGER("StringTest");
    LOGGER_INFO("sample");

    LoggerW loggerw;
    StringW titlew(L"UTF-8 サンプル");
    LOGGERW_INFO(loggerw, titlew.str().c_str());

    StringW a(L"水樹"), b(L"奈々");
    std::wcout << a[-1] << std::endl;

    StringW c = a + b;
    std::wcout << c << std::endl;
    std::wcout << (-c) << std::endl;

    std::wcout << c.indexOf(L'々') << std::endl;
    std::wcout << c.lastIndexOf(L'水', 2) << std::endl;
    std::wcout << StringW(c).slice(2) << std::endl;
    std::wcout << StringW(c).slice(1, -2) << std::endl;
    std::wcout << StringW(c).substr(2, 2) << std::endl;

    LOGGER_INFO("sample2");

    StringW d = b;
    b *= 3;
    std::wcout << b << std::endl;

    b *= -2;
    std::wcout << b << std::endl;

    LOGGER_INFO("sample3");
}

void stringTest()
{
    String str("  Nana  Mizuki   ");
    std::cout << str << std::endl;
    std::cout << (-str) << std::endl;

    printf("index 3: %c\n", str[3]);
    printf("index of a: %d\n", str.indexOf('a'));
    printf("last index of i: %d\n", str.lastIndexOf('i', 7));

    std::cout << String(str).slice(2) << std::endl;
    std::cout << String(str).slice(3, -7) << std::endl;
    std::cout << String(str).substr(2, 2) << std::endl;

    String cases(str);
    printf("'%s'\n", cases.toUpperCase().str().c_str());
    printf("'%s'\n", cases.toLowerCase().str().c_str());
    printf("'%s'\n", String(str).trimLeft().str().c_str());
    printf("'%s'\n", String(str).trimRight().str().c_str());
    printf("'%s'\n", String(str).trim().str().c_str());
    str -= ' ';
    printf("'%s'\n", str.str().c_str());
    str *= -3;
    printf("'%s'\n", str.str().c_str());
}

void dateTest()
{
    Date d;
    std::cout << d << std::endl;
    std::cout << (d + 3 * Date::MINUTE) << std::endl;
    std::cout << (d - Date::DAY) << std::endl;

    d += Date::WEEK;
    std::cout << d << std::endl;

    Date d2(d);
    std::cout << d2.month(2) << std::endl;
    std::cout << d2.addMonth(-2) << std::endl;

    std::cout.setf(std::ios::boolalpha);
    std::cout << "d < d2 == " << (d < d2) << std::endl;

    std::cout << Date::parse("1980-01-21") << std::endl;
    std::cout << Date::parse("2000") << std::endl;

    std::istringstream is("2000-01-21");
    is >> d;
    std::cout << d << std::endl;
}

void perform(bool show = false)
{
    const size_t cnt = 9999;
    clock_t t1, t2, t0 = clock();

    {
        std::string st("test");
        for (size_t i = 0; i < cnt; ++i) {
            st += "loop";
        }
        if (show) printf("%s\n", st.c_str());
    }
    t1 = clock();

    {
        String str("test");
        for (size_t i = 0; i < cnt; ++i) {
            str += "loop";
        }
        if (show) printf("%s\n", str.str().c_str());
    }
    t2 = clock();

    printf("%ldms\n", t1 - t0);
    printf("%ldms\n", t2 - t1);
}

void runServer(char* url, int port)
{
    try {
        HttpServer server(url, port);
        server.wsaInfo();
        server.serve();
    } catch(...) {
        fprintf(stderr, "error\n");
    }
}

int main(int argc, const char** argv)
{
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);

    if (argc > 1 && !strcmp(argv[1], "-s")) {
        char* url = argc > 2 ? argv[2] : "192.168.12.3";
        int port = argc > 3 ? atoi(argv[3]) : 80;
        runServer(url, port);
    } else {
        stringWTest();
        stringTest();
        dateTest();

        perform();
    }

    system("pause");

    return 0;
}
