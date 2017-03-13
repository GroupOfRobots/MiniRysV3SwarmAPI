#ifndef API_H
#define API_H

#include <QObject>
#include <QMap>
#include <QWebSocket>
#include <QWebSocketServer>
#include <iostream>

#include "robotaddresses.h"

namespace irys {

class Communicaton : public QObject {
    Q_OBJECT
public:
    Communicaton();
    virtual ~Communicaton();

    /// Sends byte-array to the specific robot with passed name.
    /// Throws RobotDisconnected.
    void sendDataToRobot(QString robotName, QByteArray data);

    /// Sends byte-array to each robot.
    /// Throws DataTransferToRobotUnsuccessful.
    void sendDataToEachRobot(QByteArray data);

Q_SIGNALS:
    /// Signal emited after websocket server closure.
    void serverClosed();

    /// Signal emited after disconnection a robot with specific id.
    void robotDisconnected(int id);

private Q_SLOTS:
    void onNewConnection();
    void processReceivedData(QByteArray data);
    void robotSocketDisconnected();

private:
    const RobotAddresses robotAddresses;

    QWebSocketServer *thisRobotWebsocketServer;
    QHash<QString, QWebSocket *> thisRobotWebsockets;
    QHash<QString, QWebSocket *> receiverWebockets;
};

}
#endif // API_H
