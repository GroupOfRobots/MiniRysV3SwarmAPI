#include "api.h"

irys::API::API()
    : m_Server(new QWebSocketServer(
                   QStringLiteral("Server name"),
                   QWebSocketServer::SecureMode, this)) {

}

irys::API::~API() { }
