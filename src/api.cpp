#include "api.h"

irys::API::API()
    : m_webSocketServer(
          new QWebSocketServer(
              QStringLiteral("IRys WebSocket Server"),
              QWebSocketServer::NonSecureMode, this)) {
    if (m_webSocketServer->listen(QHostAddress::Any, 8080)) {
        qDebug() << "Echoserver listening on port" << 8080;
    }
    connect(m_webSocketServer, &QWebSocketServer::newConnection,
            this, &API::onNewConnection);
    connect(m_webSocketServer, &QWebSocketServer::closed,
            this, &API::serverClosed);
}

irys::API::~API() {
    m_webSocketServer->close();
    qDeleteAll(m_robotsSockets.begin(), m_robotsSockets.end());
}

void irys::API::sendDataToEachRobot(QByteArray data) {

}

void irys::API::onNewConnection() {
    QWebSocket *pSocket = m_webSocketServer->nextPendingConnection();
    connect(pSocket, &QWebSocket::binaryMessageReceived,
            this, &API::processReceivedData);
    connect(pSocket, &QWebSocket::textMessageReceived,
            this, &API::processReceivedMessage);
    connect(pSocket, &QWebSocket::disconnected,
            this, &API::robotSocketDisconnected);
    m_robotsSockets << pSocket;
    qDebug() << "connection!";
    pSocket->sendTextMessage("lololol");
}

void irys::API::processReceivedData(QByteArray data) {
    qDebug() << "Received: " << data;
}


void irys::API::processReceivedMessage(QString data) {
    qDebug() << data;
}

void irys::API::robotSocketDisconnected() {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "Socket Disconnected: " << pClient;
    if (pClient) {
        m_robotsSockets.removeAll(pClient);
        pClient->deleteLater();
    }
}

