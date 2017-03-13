#ifndef ROBOTADDRESSES_H
#define ROBOTADDRESSES_H

#include <QString>
#include <QList>
#include <QHash>
#include <QHostAddress>

const QHash<int, QString> ROBOT_NAMES = {
    { 1, "ROBOT1" },
    { 2, "ROBOT2" },
    { 3, "ROBOT3" },
    { 4, "ROBOT4" },
    { 5, "ROBOT5" },
    { 6, "ROBOT6" }
};

class RobotAddresses
{
public:
    RobotAddresses() {
        for (auto it = ROBOT_IDS.begin(); it != ROBOT_IDS.end(); ++it) {
            QString robotName = it.key();
            int robotAddress = it.value();
            QString fullIpAddress = QStringLiteral("192.168.0.%1").arg(robotAddress);
            addresses.insert(robotName, QHostAddress(fullIpAddress));
        }
    }

    const QHostAddress& getAddress(const QString &robotName) {
        return addresses[robotName];
    }

    QHash<QString, QHostAddress>::const_iterator cbegin() {
        return addresses.cbegin();
    }

    QHash<QString, QHostAddress>::const_iterator cend() {
        return addresses.cend();
    }

private:
    QHash<QString, QHostAddress> addresses;

    const QHash<QString, int> ROBOT_IDS {
        { ROBOT_NAMES[1], 101 },
        { ROBOT_NAMES[2], 102 },
        { ROBOT_NAMES[3], 103 },
        { ROBOT_NAMES[4], 104 },
        { ROBOT_NAMES[5], 105 },
        { ROBOT_NAMES[6], 106 }
    };
};

#endif // ROBOTADDRESSES_H
