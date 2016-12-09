#include "wsserver.h"

irys::api::WSServer::WSServer()
    : server(8080)
{
    server.run();
}


MyEchoServer::MyEchoServer( int port ) : EchoServer( port )
{
}

MyEchoServer::~MyEchoServer( )
{
}


void MyEchoServer::onConnect( int socketID )
{
    std::cout << "\nNew connection:" << socketID << "\n";
    send(socketID, "msg from on connect\n");
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

