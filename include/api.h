#ifndef API_H
#define API_H

#include <QObject>
#include <QMap>
#include <QWebSocket>
#include <QWebSocketServer>
#include <iostream>

#include "robotaddresses.h"
#include "robotstatus.h"

namespace irys {

class Communicaton : public QObject {
    Q_OBJECT
public:
    Communicaton();
    virtual ~Communicaton();

    /// Sends byte-array to the specific robot with passed name.
    /// Throws RobotDisconnected.
    void sendDataToRobot(int robotId, QByteArray data);

    /// Sends byte-array to each robot.
    /// Throws DataTransferToRobotUnsuccessful.
    void sendDataToEachRobot(QByteArray data);

Q_SIGNALS:
    /// Signal emited after websocket server closure.
    void serverClosed();

    /// Signal emited after disconnection a robot with specific id.
    void robotDisconnected(int robotId);

    /// Signal emited after receiving data from paricular robot
    void dataReceived(int robotId);

private Q_SLOTS:
    void onNewConnection();
    void processReceivedData(QByteArray data);
    void robotSocketDisconnected();

private:
    const RobotAddresses ROBOT_ADDRESSES;

    QWebSocketServer *thisRobotWebsocketServer;

    /// This sockets represent other robots sockets - they are used to transmit data
    /// They are being managed by thisRobotWebsocketServer
    QHash<int, QWebSocket *> otherRobotsSockets;

    /// This sockets will receive data from other robot's servers
    QHash<int, QWebSocket *> otherRobotsServers;

    QQueue<RobotStatus> receivedStatuses;
}
#endif // API_H
