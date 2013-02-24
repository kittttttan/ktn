#pragma once
#include <string>
#include <map>

namespace ktn {

class HttpRequest
{
public:
    explicit HttpRequest(const char* src);
    ~HttpRequest();

    void parse(const char* src);

    const std::string& method() const { return method_; }
    const std::string& path() const { return path_; }
    const std::string& ver() const { return ver_; }
    const std::string& body() const { return body_; }
    const std::map<std::string, std::string>& header() { return headers_; }
private:
    std::string method_;
    std::string path_;
    std::string ver_;
    std::string body_;
    std::map<std::string, std::string> headers_;
};

} // namespace ktn