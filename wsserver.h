#ifndef WSSERVER_H
#define WSSERVER_H

#include "WebSocketServer.h"


class MyEchoServer : public EchoServer
{
public:
    MyEchoServer( int port );
    ~MyEchoServer( );
    virtual void onConnect(    int socketID                        );
    virtual void onMessage(    int socketID, const std::string& data    );
    virtual void onDisconnect( int socketID                        );
    virtual void   onError(    int socketID, const std::string& message );
};

namespace irys {
namespace api {

class WSServer
{
    MyEchoServer server;
public:
    WSServer();
};

} // namespace api
} // namespace irys

#endif // WSSERVER_H
