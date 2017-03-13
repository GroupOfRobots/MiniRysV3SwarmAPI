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

    for (auto&& it = ROBOT_ADDRESSES.cbegin(); it != ROBOT_ADDRESSES.cend(); ++it) {
        int robotId = it.key();
        const QHostAddress &address = it.value();
        const QUrl url = QStringLiteral("ws://%1:8888").arg(address.toString());
        std::unique_ptr<QWebSocket> ws(new QWebSocket);
        ws->open(url);
        if (ws->isValid()) {
            masterSockets.insert(robotId, std::move(ws));
        }
    }
}

irys::Communicaton::~Communicaton() {
    thisRobotWebsocketServer->close();
    qDeleteAll(slaveSockets.begin(), slaveSockets.end());
    qDeleteAll(masterSockets.begin(), masterSockets.end());
}

void irys::Communicaton::sendDataToEachRobot(QByteArray data) {
    for (auto&& socket : masterSockets) {
        socket->sendBinaryMessage(data);
    }
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
    RobotStatus *rs = reinterpret_cast<RobotStatus *>(data.data());
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
        QList<int> clientAtPositions = slaveSockets.keys(client);
        if (clientAtPositions.size() > 0) {
            int clientKey = clientAtPositions.at(0);
            slaveSockets.remove(clientKey);
            client->deleteLater();
        }
    }
}

