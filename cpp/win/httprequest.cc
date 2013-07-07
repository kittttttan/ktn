#include "httprequest.h"

namespace ktn {

void HttpRequest::parse(const char* src)
{
    int begin = 0;
    int cur = begin;
    int blank = 0;
    int row = 0;
    char c;
    while (1) {
        c = src[cur];
        if (c == EOF) break;
        if (row == 0) {
            // (GET|POST) /(path)? HTTP/1.[01]
            if (c == ' ') {
                switch (blank) {
                case 0:
                    method_ = std::string(src + begin, cur - begin);
                    break;
                case 1:
                    path_ = std::string(src + begin, cur - begin);
                    break;
                }
                begin = cur + 1;
                ++blank;
            } else if (strncmp(src + cur, "\r\n", 2) == 0) {
                ver_ = std::string(src + begin, cur - begin);
                ++cur;
                blank = 0;
                ++row;
                begin = cur + 1;
            }
            ++cur;
        } else {
            // Option: value
            if (strncmp(src + begin, "\r\n", 2) == 0) {
                ++row;
                body_ = src + begin + 2;
                break;
            } else {
                // skip lines
                std::string opt;
                while (1) {
                    if (src[cur] == EOF) break;
                    if (strncmp(src + cur, ": ", 2) == 0) {
                        opt = std::string(src + begin, cur - begin);
                        cur += 2;
                        begin = cur;
                    } else if (strncmp(src + cur, "\r\n", 2) == 0) {
                        headers_.insert(
                            std::map<std::string, std::string>::value_type(opt,
                                std::string(src + begin, cur - begin)));
                        blank = 0;
                        ++row;
                        cur += 2;
                        begin = cur;
                        break;
                    } else {
                        ++cur;
                    }
                }
            }
        }
    }
    //printf("row: %d\n", row);
}

} // namespace ktn