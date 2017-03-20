#ifndef API_H
#define API_H

#include <QObject>
#include <QMap>
#include <QWebSocket>
#include <QWebSocketServer>
#include <iostream>
#include "robotstatus.h"

namespace irys {

class SwarmServer : public QObject {
    Q_OBJECT
public:
    SwarmServer();
    virtual ~SwarmServer();

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
    QWebSocketServer *webSocketServer;
    // websocket assigned to robot's id
    QList<QWebSocket *> robotsSockets;
};

}
#endif // API_H
