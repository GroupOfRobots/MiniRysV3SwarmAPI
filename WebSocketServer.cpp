/** --------------------------------------------------------------------------
 *  WebSocketServer.cpp
 *
 *  Base class that WebSocket implementations must inherit from.  Handles the
 *  client connections and calls the child class callbacks for connection
 *  events like onConnect, onMessage, and onDisconnect.
 *
 *  Author    : Jason Kruse <jason@jasonkruse.com> or @mnisjk
 *  Copyright : 2014
 *  License   : BSD (see LICENSE)
 *  -------------------------------------------------------------------------- 
 **/

#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <string>
#include <fcntl.h>
#include "libwebsockets.h"
#include "WebSocketServer.h"

// 0 for unlimited
#define MAX_BUFFER_SIZE 0

// Nasty hack because certain callbacks are statically defined
EchoServer *self;

static int callback_main(   struct lws *wsi, 
                            enum lws_callback_reasons reason, 
                            void *user, 
                            void *in, 
                            size_t len )
{
    int fd;
    unsigned char buf[LWS_SEND_BUFFER_PRE_PADDING + 512 + LWS_SEND_BUFFER_POST_PADDING];
    unsigned char *p = &buf[LWS_SEND_BUFFER_PRE_PADDING];
    
    switch( reason ) {
        case LWS_CALLBACK_ESTABLISHED:
            self->onConnectWrapper( lws_get_socket_fd( wsi ) );
            lws_callback_on_writable( wsi );
            break;

        case LWS_CALLBACK_SERVER_WRITEABLE:
            fd = lws_get_socket_fd( wsi );
            while( !self->connections[fd]->buffer.empty( ) )
            {
                std::string message = self->connections[fd]->buffer.front( );
                int msgLen = message.length();
                int charsSent = lws_write( wsi, (unsigned char*)message.c_str(), msgLen, LWS_WRITE_TEXT );
                if( charsSent < msgLen )
                    self->onErrorWrapper( fd, std::string( "Error writing to socket" ) );
                else
                    // Only pop the message if it was sent successfully.
                    self->connections[fd]->buffer.pop_front( ); 
            }
            lws_callback_on_writable( wsi );
            break;
        
        case LWS_CALLBACK_RECEIVE:
            self->onMessage( lws_get_socket_fd( wsi ), std::string( (const char *)in ) );
            break;

        case LWS_CALLBACK_CLOSED:
            self->onDisconnectWrapper( lws_get_socket_fd( wsi ) );
            break;

        default:
            break;
    }
    return 0;
}

static struct lws_protocols protocols[] = {
    {
        "/",
        callback_main,
        0, // user data struct not used
        MAX_BUFFER_SIZE,
    },{ NULL, NULL, 0, 0 } // terminator
};

EchoServer::EchoServer( int port, const std::string certPath, const std::string& keyPath )
{
    this->_port     = port;
    this->_certPath = certPath;
    this->_keyPath  = keyPath; 

    lws_set_log_level( 0, lwsl_emit_syslog ); // We'll do our own logging, thank you.
    struct lws_context_creation_info info;
    memset( &info, 0, sizeof info );
    info.port = this->_port;
    info.iface = NULL;
    info.protocols = protocols;
#ifndef LWS_NO_EXTENSIONS
    info.extensions = lws_get_internal_extensions( );
#endif
    
    if( !this->_certPath.empty( ) && !this->_keyPath.empty( ) )
    {
        std::cout << std::string( "Using SSL certPath=" + this->_certPath + ". keyPath=" + this->_keyPath + ".\n" );
        info.ssl_cert_filepath        = this->_certPath.c_str( );
        info.ssl_private_key_filepath = this->_keyPath.c_str( );
    } 
    else 
    {
        std::cout << std::string( "Not using SSL" );
        info.ssl_cert_filepath        = NULL;
        info.ssl_private_key_filepath = NULL;
    }
    info.gid = -1;
    info.uid = -1;
    info.options = 0;

    // keep alive
    info.ka_time = 60; // 60 seconds until connection is suspicious
    info.ka_probes = 10; // 10 probes after ^ time
    info.ka_interval = 10; // 10s interval for sending probes
    this->_context = lws_create_context( &info );
    if( !this->_context )
        throw "libwebsocket init failed";
    std::cout << std::string( "Server started on port " + std::to_string( this->_port ) );

    // Some of the libwebsocket stuff is define statically outside the class. This 
    // allows us to call instance variables from the outside.  Unfortunately this
    // means some attributes must be public that otherwise would be private. 
    self = this;
}

EchoServer::~EchoServer( )
{
    // Free up some memory
    for( std::map<int,Connection*>::const_iterator it = this->connections.begin( ); it != this->connections.end( ); ++it )
    {
        Connection* c = it->second;
        this->connections.erase( it->first );
        delete c;
    }
}

void EchoServer::onConnectWrapper( int socketID )
{
    Connection* c = new Connection;
    c->createTime = time( 0 );
    this->connections[ socketID ] = c;
    this->onConnect( socketID );
}

void EchoServer::onDisconnectWrapper( int socketID )
{
    this->onDisconnect( socketID );
    this->_removeConnection( socketID );
}

void EchoServer::onErrorWrapper( int socketID, const std::string& message )
{
    std::cout << std::string( "Error: " + message + " on socketID '" + std::to_string( socketID ) + "'" );
    this->onError( socketID, message );
    this->_removeConnection( socketID );
}

void EchoServer::send( int socketID, std::string data )
{
    // Push this onto the buffer. It will be written out when the socket is writable.
    this->connections[socketID]->buffer.push_back( data );
}

void EchoServer::broadcast( std::string data )
{
    for( std::map<int,Connection*>::const_iterator it = this->connections.begin( ); it != this->connections.end( ); ++it )
        this->send( it->first, data );
}

void EchoServer::setValue( int socketID, const std::string& name, const std::string& value )
{
    this->connections[socketID]->keyValueMap[name] = value;
}

std::string EchoServer::getValue( int socketID, const std::string& name )
{
    return this->connections[socketID]->keyValueMap[name];
}
int EchoServer::getNumberOfConnections( )
{
    return this->connections.size( );
}

void EchoServer::run( uint64_t timeout )
{
    while( 1 )
    {
        this->wait( timeout );
    }
}

void EchoServer::wait( uint64_t timeout )
{
    if( lws_service( this->_context, timeout ) < 0 )
        throw "Error polling for socket activity.";
}

void EchoServer::_removeConnection( int socketID )
{
    Connection* c = this->connections[ socketID ];
    this->connections.erase( socketID );
    delete c;
}
