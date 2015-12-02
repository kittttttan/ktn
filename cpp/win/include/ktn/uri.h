#pragma once
#include <string>

namespace ktn {

class Uri {
public:
    enum Type {
        Absolute,
        Relative,
    };

    explicit Uri(const char* uri, Type type=Absolute) { parse(uri, type); }
    ~Uri() {}

    void parse(const char* uri, Type type=Absolute);

    std::string str() const;
    std::string ext() const;

    const std::string& protocol() const { return protocol_; }
    const std::string& hostname() const { return hostname_; }
    const std::string& port() const { return port_; }
    const std::string& pathname() const { return pathname_; }
    const std::string& search() const { return search_; }
    const std::string& hash() const { return hash_; }

    std::string protocol(const char* protocol) { protocol_ = protocol; }
    std::string hostname(const char* hostname) { hostname_ = hostname; }
    std::string port(const char* port) { port_ = port; }
    std::string pathname(const char* pathname) { pathname_ = pathname; }
    std::string search(const char* search) { search_ = search; }
    std::string hash(const char* hash) { hash_ = hash; }

private:
    std::string protocol_;
    std::string hostname_;
    std::string port_;
    std::string pathname_;
    std::string search_;
    std::string hash_;
};

} // namespace ktn

#include "ktn/uri.inl"
