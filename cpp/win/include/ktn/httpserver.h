#pragma once
#include <winsock2.h>
#include <string>

namespace ktn {

class HttpServer {
public:
    static const char* getFileType(const char* path);

    explicit HttpServer(char* ip=nullptr, int port=80);
    ~HttpServer()
    {
        ::shutdown(socket_, SD_BOTH);
        ::WSACleanup();
    }

    void serve() const;
    void wsaInfo() const;
    void rootdir(const char* dir) { rootDir_ = dir; }
private:
    int serveFile(SOCKET socket, const char* path) const;

    int port_;
    SOCKET socket_;
    WSADATA wsaData_;
    char* ipAddr_;
    std::string rootDir_;
};

} // namespace ktn

#include "ktn/httpserver.inl"
