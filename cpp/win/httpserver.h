#pragma once
#include <winsock2.h>
#include <string>

class HttpServer {
public:
    explicit HttpServer(char* ip=nullptr, int port=80);
    ~HttpServer();

    void serve();
    void wsaInfo();
    void rootdir(const char* dir) { rootDir_ = dir; }
private:
    int port_;
    SOCKET socket_;
    WSADATA wsaData_;
    char* ipAddr_;
    std::string rootDir_;

    int serveFile(SOCKET socket, const char* path);
};