#include "httpserver.h"
#include "httprequest.h"
#include "httpresponse.h"
#include "uri.h"

#include <cstdio>
#include <iostream>
#include <fstream>
#include <exception>

#pragma comment(lib, "ws2_32.lib")

namespace ktn {

const size_t kDataSize = 1024 * 4;

const char* HttpServer::getFileType(const char* path)
{
    const char* ext = strrchr(path, '.');
    if (!ext) return "text/plain";

    if (strcmp(ext, ".htm") == 0 || strcmp(ext, ".html") == 0) {
        return "text/html";
    }
    if (strcmp(ext, ".css") == 0) {
        return "text/css";
    }
    if (strcmp(ext, ".js") == 0) {
        return "text/javascript";
    }
    if (strcmp(ext, ".ico") == 0) {
        return "image/x-icon";
    }
    if (strcmp(ext, ".png") == 0) {
        return "image/png";
    }
    if (strcmp(ext, ".jpg") == 0) {
        return "image/jpeg";
    }

    return "text/plain";
}

HttpServer::HttpServer(char* ip, int port) :
    port_(port), ipAddr_(ip), socket_(INVALID_SOCKET), rootDir_(".")
{
    int err = ::WSAStartup(MAKEWORD(2, 2), &wsaData_);
    if (err) {
        char strErr[64] = "failed WSAStartup: ";
        switch (err) {
        case WSASYSNOTREADY: strcat_s(strErr, "WSASYSNOTREADY"); break;
        case WSAVERNOTSUPPORTED: strcat_s(strErr, "WSAVERNOTSUPPORTED"); break;
        case WSAEINPROGRESS:strcat_s(strErr, "WSAEINPROGRESS"); break;
        case WSAEPROCLIM: strcat_s(strErr, "WSAEPROCLIM"); break;
        case WSAEFAULT: strcat_s(strErr, "WSAEFAULT"); break;
        default: strcat_s(strErr, "unknown"); break;
        }
        throw std::exception(strErr);
    }

    socket_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ == INVALID_SOCKET) {
        char what[32];
        sprintf_s(what, "socket failed: %d\n", ::WSAGetLastError());
        throw std::exception(what);
    }
}

void HttpServer::wsaInfo() const
{
    printf("%s ver.%u.%u %s\n",
        wsaData_.szDescription,
        (BYTE)wsaData_.wHighVersion , wsaData_.wHighVersion >> 8,
        wsaData_.szSystemStatus);
}

int HttpServer::serveFile(SOCKET socket, const char* path) const
{
    printf("open %s\n", path);

    char* buf = nullptr;
    size_t size = 0;
    std::ifstream fs(path, std::ios::in | std::ios::binary);
    if (!fs) {
        return HttpResponse::send(socket, 404, "text/html", HttpResponse::getCodeDesc(404), 0);
    }

    try {
        fs.exceptions(std::ios::badbit);

        fs.seekg(0, std::fstream::end);
        size = static_cast<size_t>(fs.tellg());

        fs.clear();
        fs.seekg(0, std::fstream::beg);

        buf = new char[size];
        memset(buf, 0, size);

        fs.read(buf, size);
        fs.close();
    } catch (std::ios_base::failure& e) {
        delete [] buf;
        std::cerr << "file read error: " << e.what() << std::endl;
        return HttpResponse::send(socket, 500, "text/html", HttpResponse::getCodeDesc(500), 0);
    }
    
    int n = HttpResponse::send(socket, 200, getFileType(path), buf, size);

    delete [] buf;
    return n;
}

void HttpServer::serve() const
{
    errno_t err = 0;
    struct sockaddr_in addr;
    struct sockaddr_in client;
    SOCKET clientSocket = INVALID_SOCKET;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);
    if (ipAddr_) {
        addr.sin_addr.s_addr = inet_addr(ipAddr_);
    } else {
        addr.sin_addr.S_un.S_addr = INADDR_ANY;
    }

    // TODO: setting
    int opt = 1;
    ::setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt));
    ::setsockopt(socket_, SOL_SOCKET, SO_KEEPALIVE, (const char *)&opt, sizeof(opt));

    if (::bind(socket_, (struct sockaddr *)&addr, sizeof(addr))) {
        char what[32];
        sprintf_s(what, "failed bind: %d", ::WSAGetLastError());
        throw std::exception(what);
    }

    if (::listen(socket_, SOMAXCONN)) {
        char what[32];
        sprintf_s(what, "failed listen: %d", ::WSAGetLastError());
        throw std::exception(what);
    }

    printf("serve at %s:%d\n", ipAddr_, port_);

    char rbuf[kDataSize];
    int n = 1;
    int len = sizeof(client);
    while (1) {
        clientSocket = ::accept(socket_, (sockaddr *)&client, &len);
        if (clientSocket == INVALID_SOCKET) {
            char what[32];
            sprintf_s(what, "socket failed: %d\n", ::WSAGetLastError());
            throw std::exception(what);
        }
        //printf("accept %s:%u\n",
        //    ::inet_ntoa(client.sin_addr), ::ntohs(client.sin_port));

        std::string strReq;
        while (1) {
            n = ::recv(clientSocket, rbuf, sizeof(rbuf) - 1, 0);
            if (n < 0) {
                err = 10;
                goto SHUT_DOWN_SOCKET;
            }
            rbuf[n] = '\0';
            strReq += rbuf;
            if (n < sizeof(rbuf) - 1) {
                break;
            }
        }
        printf("%s\n", strReq.c_str());

        {
            HttpRequest req;
            req.parse(strReq.c_str());
            const std::string& path = req.path();
            const std::string& method = req.method();
            printf("%s %s\n", method.c_str(), path.c_str());

            if (method == "GET" || method == "POST") {
                Uri uri(path.c_str(), Uri::Relative);

                if (path[path.length() - 1] == '/') {
                    n = serveFile(clientSocket, (rootDir_ + uri.pathname() + "index.html").c_str());
                } else {
                    n = serveFile(clientSocket, (rootDir_ + uri.pathname()).c_str());
                }

                if (n < 0) {
                    err = 20;
                    goto SHUT_DOWN_SOCKET;
                }
            } else {
                fprintf(stderr, "invalid method: %s\n", method.c_str());
            }
        }

SHUT_DOWN_SOCKET:
        if (::shutdown(clientSocket, SD_SEND)) {
            char what[32];
            sprintf_s(what, "failed shutdown: %d", ::WSAGetLastError());
            throw std::exception(what);
        }

        ::closesocket(clientSocket);
        //printf("close\n\n");

        if (err) {
            char what[32];
            switch (err) {
            case 10: sprintf_s(what, "failed recv: %d", ::WSAGetLastError()); break;
            case 20: sprintf_s(what, "failed send: %d", ::WSAGetLastError()); break;
            default: sprintf_s(what, "failed: %d", ::WSAGetLastError()); break;
            }

            throw std::exception(what);
        }
    }
}

} // namespace ktn