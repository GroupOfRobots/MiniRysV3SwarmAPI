#ifndef API_H
#define API_H

#include <QObject>
#include <QMap>
#include <QWebSocket>
#include <QWebSocketServer>
#include <iostream>

namespace irys {

class API : public QObject {
    Q_OBJECT
public:
    API();
    virtual ~API();

    /// Sends byte-array to the specific robot with passed id.
    /// Throws DataTransferToRobotUnsuccessful.
    void sendDataToRobotWithId(int id, QByteArray data);

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
    void processReceivedMessage(QString data);
    void processReceivedData(QByteArray data);
    void robotSocketDisconnected();

private:
    // wsockets side should be separated!
    QWebSocket m_webSocket;
    QWebSocketServer *m_webSocketServer;
    // websocket assigned to robot's id
    QList<QWebSocket *> m_robotsSockets;
};

}
#endif // API_H
