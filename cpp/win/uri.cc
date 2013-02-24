#include "uri.h"
#include <cstring>

namespace ktn {

Uri::Uri(const char* uri, Type type)
{
    parse(uri, type);
}

Uri::~Uri()
{

}

void Uri::parse(const char* uri, Type type)
{
    size_t size = strlen(uri) + 1;
    char* src = new char[size];
    strcpy_s(src, size, uri);
    char* begin = src;
    char* cur;
    
    protocol_ = "";
    hostname_ = "";
    port_ = "";
    pathname_ = "";
    search_ = "";
    hash_ = "";

    if (type == Absolute) {
        cur = strchr(begin, '/');
        if (!cur) goto END_PARSE;

        *cur = '\0';
        protocol_ = begin;
        ++cur;
        if (cur == '\0') goto END_PARSE;
        ++cur;
        begin = cur;
    }

    cur = strchr(begin, ':');
    if (cur) {
        *cur = '\0';
        hostname_ = begin;
        ++cur;
        begin = cur;

        cur = strchr(begin, '/');
        if (!cur) {
            port_ = begin;
            goto END_PARSE;
        }
        *cur = '\0';
        port_ = begin;
    } else {
        cur = strchr(begin, '/');
        if (!cur) {
            hostname_ = begin;
            goto END_PARSE;
        }
        *cur = '\0';
        hostname_ = begin;
    }
    *cur = '/';
    begin = cur;

    cur = strchr(begin, '?');
    if (cur) {
        *cur = '\0';
        pathname_ = begin;
        *cur = '?';
        begin = cur;

        cur = strchr(begin, '#');
        if (!cur) {
            search_ = begin;
            goto END_PARSE;
        }

        *cur = '\0';
        search_ = begin;
        *cur = '#';
    } else {
        cur = strchr(begin, '#');
        if (!cur) {
            pathname_ = begin;
            goto END_PARSE;
        }

        *cur = '\0';
        pathname_ = begin;
        *cur = '#';
    }

    hash_ = cur;

END_PARSE:
    delete[] src;
}

std::string Uri::str() const
{
    return
        (protocol_ == "" ? "" : protocol_ + "//") +
        hostname_ +
        (port_ == "" ? "" : ":" + port_) +
        pathname_ +
        search_ +
        hash_;
}

std::string Uri::ext() const
{
    size_t pos = pathname_.rfind('/');
    if (pos == std::string::npos) {
        pos = pathname_.rfind('.');
        if (pos == std::string::npos) {
            return std::string();
        }
        return pathname_.substr(pos);
    }

    std::string path = pathname_.substr(pos);
    pos = path.rfind('.');
    if (pos == std::string::npos) {
        return std::string();
    }
    return path.substr(pos);
}

} // namespace ktn