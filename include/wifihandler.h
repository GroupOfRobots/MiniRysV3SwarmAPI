#ifndef WIFHANDLER_H
#define WIFHANDLER_H

#include <QObject>
#include <QDBusInterface>
#include <QDBusConnection>
#include <QDBusReply>
#include <QDebug>

#include "exceptions.h"
#include "nm-dbus-interface.h"

Q_DECLARE_METATYPE(QList<QDBusObjectPath>)
Q_DECLARE_METATYPE(NMDeviceType)
Q_DECLARE_METATYPE(QDBusReply<NMDeviceType>)

namespace irys {

class WifiHandler : protected QObject {
    Q_OBJECT
public:
    WifiHandler();
    virtual ~WifiHandler();

private:
    QDBusInterface m_NetworkManagerInterface;
    QDBusObjectPath m_wirelessDevicePath;

    QDBusObjectPath recognizeWirelessDevicePath();
    QList<QDBusObjectPath> getAllDevices();
    QDBusObjectPath findWirelessDeviceIn(QList<QDBusObjectPath> devicesPaths);
};

}
#endif // WIFHANDLER_H
