#pragma once
#include <winsock2.h>

namespace HttpResponse {

const char* getCodeDesc(int code);
int send(SOCKET sokcet, int code, const char* contentType, const char* body, size_t bodySize);

}