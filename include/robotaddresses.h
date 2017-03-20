#ifndef ROBOTADDRESSES_H
#define ROBOTADDRESSES_H

#include <QString>
#include <QList>
#include <QHash>
#include <QHostAddress>

namespace irys {

class RobotAddresses
{
public:
    RobotAddresses() {
        for (auto it = ROBOTS_ID_WITH_IP_TAILS.cbegin(); it != ROBOTS_ID_WITH_IP_TAILS.cend(); ++it) {
            int robotId = it.key();
            int robotIpTail = it.value();
            QString fullIpAddress = QStringLiteral("192.168.1.%1").arg(robotIpTail);
            addresses.insert(robotId, QHostAddress(fullIpAddress));
        }
    }

    const QHostAddress& getAddress(int robotId) const {
        auto it = addresses.find(robotId);
        return it.value();
    }

    int getId(const QHostAddress &address) const {
        QList<int> addressesAtPositions = addresses.keys(address);
        if (addressesAtPositions.size() == 0) {
            return -1;
        }
        return addressesAtPositions.at(0);
    }

    QHash<int, QHostAddress>::const_iterator cbegin() const {
        return addresses.cbegin();
    }

    QHash<int, QHostAddress>::const_iterator cend() const {
        return addresses.cend();
    }

private:
    QHash<int, QHostAddress> addresses;

    const QHash<int, int> ROBOTS_ID_WITH_IP_TAILS {
        { 1, 101 },
        { 2, 102 },
        { 3, 103 },
        { 4, 104 },
        { 5, 105 },
        { 6, 106 }
    };
};

} // namespace irys

#endif // ROBOTADDRESSES_H
