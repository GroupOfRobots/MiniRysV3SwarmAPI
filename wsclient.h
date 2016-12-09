#ifndef WSCLIENT_H
#define WSCLIENT_H

#include "easywsclient.hpp"
#include "robotstatus.h"
#include <memory>
#include <iostream>

namespace irys {
namespace api {

class WSClient
{
#ifdef _WIN32
    INT rc;
    WSADATA wsaData;
#endif
    std::unique_ptr<easywsclient::WebSocket> ws;
public:
    WSClient();
    void processMessage();
};

} // namespace api
} // namespace irys

#endif // WSCLIENT_H
