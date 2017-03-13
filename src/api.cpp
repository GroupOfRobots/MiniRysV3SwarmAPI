#include "api.h"

irys::Communicaton::Communicaton()
    : thisRobotWebsocketServer(
          new QWebSocketServer(
                  QStringLiteral("IRys WebSocket Server"),
                  QWebSocketServer::NonSecureMode,
                  this)
          )
{
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
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    qDebug() << "Received: " << data;
    RobotStatus *rs = static_cast<RobotStatus *>(data.data();
    receivedStatuses.enqueue(*rs);
    QHostAddress clientAddress = client->peerAddress();
    // TODO: find in ROBOT_ADDRESSES id of the robot
    int clientId = ROBOT_ADDRESSES.getId(clientAddress);
    emit dataReceived(clientId);
}

void irys::Communicaton::robotSocketDisconnected() {
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    qDebug() << "Socket Disconnected: " << client;
    if (client) {
        QList<int> clientAtPositions = otherRobotsSockets.keys(client);
        if (clientAtPositions.size() > 0) {
            int clientKey = clientAtPositions.at(0);
            otherRobotsSockets.remove(clientKey);
            client->deleteLater();
        }
    }
}

