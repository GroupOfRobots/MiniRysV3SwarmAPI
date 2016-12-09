/** --------------------------------------------------------------------------
 *  MyEchoServer.cpp
 *
 *  A basic example of how to implement a WebSocketServer.  Creats a WS server
 *  bound to port 8080.  It listens for connections, and when a message is 
 *  received, it echos the same message back to the client.
 *
 *  Author    : Jason Kruse <jason@jasonkruse.com> or @mnisjk
 *  Copyright : 2014
 *  License   : BSD (see LICENSE)
 *  -------------------------------------------------------------------------- 
 **/


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

MyEchoServer::MyEchoServer( int port ) : EchoServer( port )
{
}

MyEchoServer::~MyEchoServer( )
{
}


void MyEchoServer::onConnect( int socketID )
{
    std::cout << "New connection\n";
}

void MyEchoServer::onMessage( int socketID, const std::string& data )
{
    // Reply back with the same message
    std::cout << "Received: " << data << '\n';
    this->send( socketID, data );
}

void MyEchoServer::onDisconnect( int socketID )
{
    std::cout << "Disconnect\n";
}

void MyEchoServer::onError( int socketID, const std::string& message )
{
    std::cout << "Error: " << message << '\n';
}


