#include "api.h"

irys::Communicaton::Communicaton()
    : thisRobotWebsocketServer(
          new QWebSocketServer(
              QStringLiteral("IRys WebSocket Server"),
              QWebSocketServer::NonSecureMode, this)) {
    if (thisRobotWebsocketServer->listen(QHostAddress::Any, 8888)) {
        qDebug() << "Echoserver listening on port" << 8888;
    }
    connect(thisRobotWebsocketServer, &QWebSocketServer::newConnection,
            this, &Communicaton::onNewConnection);
    connect(thisRobotWebsocketServer, &QWebSocketServer::closed,
            this, &Communicaton::serverClosed);
}

irys::Communicaton::~Communicaton() {
    thisRobotWebsocketServer->close();
    qDeleteAll(receiverWebockets.begin(), receiverWebockets.end());
}

void irys::Communicaton::sendDataToEachRobot(QByteArray data) {

}

void irys::Communicaton::onNewConnection() {
    QWebSocket *pSocket = thisRobotWebsocketServer->nextPendingConnection();
    connect(pSocket, &QWebSocket::binaryMessageReceived,
            this, &Communicaton::processReceivedData);
    connect(pSocket, &QWebSocket::disconnected,
            this, &Communicaton::robotSocketDisconnected);
    //receiverWebockets << pSocket;
}

void irys::Communicaton::processReceivedData(QByteArray data) {
    qDebug() << "Received: " << data;
}

void irys::Communicaton::robotSocketDisconnected() {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "Socket Disconnected: " << pClient;
    if (pClient) {
        //receiverWebockets.removeAll(pClient);
        pClient->deleteLater();
    }
}

