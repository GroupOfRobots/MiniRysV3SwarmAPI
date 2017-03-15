#include "api.h"

irys::SwarmServer::SwarmServer()
    : webSocketServer(
          new QWebSocketServer(
              QStringLiteral("IRys WebSocket Server"),
              QWebSocketServer::NonSecureMode, this)) {
    if (webSocketServer->listen(QHostAddress::Any, 8888)) {
        qDebug() << "Echoserver listening on port" << 8888;
    }
    connect(webSocketServer, &QWebSocketServer::newConnection,
            this, &SwarmServer::onNewConnection);
    connect(webSocketServer, &QWebSocketServer::closed,
            this, &SwarmServer::serverClosed);
}

irys::SwarmServer::~SwarmServer() {
    webSocketServer->close();
    qDeleteAll(robotsSockets.begin(), robotsSockets.end());
}

void irys::SwarmServer::onNewConnection() {
    QWebSocket *pSocket = webSocketServer->nextPendingConnection();
    connect(pSocket, &QWebSocket::binaryMessageReceived,
            this, &SwarmServer::processReceivedData);
    connect(pSocket, &QWebSocket::textMessageReceived,
            this, &SwarmServer::processReceivedMessage);
    connect(pSocket, &QWebSocket::disconnected,
            this, &SwarmServer::robotSocketDisconnected);
    robotsSockets << pSocket;
    qDebug() << "connection!";
}

void irys::SwarmServer::processReceivedData(QByteArray data) {
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    for (auto&& ws : robotsSockets) {
        if (ws != client) {
            ws->sendBinaryMessage(data);
        }
    }
}


void irys::SwarmServer::processReceivedMessage(QString data) {
    qDebug() << data;
}

void irys::SwarmServer::robotSocketDisconnected() {
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    qDebug() << "Socket Disconnected: " << client;
    if (client) {
        robotsSockets.removeAll(client);
        client->deleteLater();
    }
}

