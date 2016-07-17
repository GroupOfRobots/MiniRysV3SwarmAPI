#include "api.h"

irys::API::API()
    : m_Server(new QWebSocketServer(
                   QStringLiteral("Server name"),
                   QWebSocketServer::SecureMode, this)) {

}

irys::API::~API() { }

void irys::API::sendDataToEachRobot(QByteArray data) {

}

void irys::API::onNewConnection() {

}

void irys::API::processReceivedData(QByteArray data) {

}

void irys::API::robotSocketDisconnected() {

}

