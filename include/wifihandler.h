#ifndef WIFHANDLER_H
#define WIFHANDLER_H

#include <QObject>
#include <QDBusInterface>
#include <QDBusConnection>
#include <QDBusReply>
#include <QUuid>
#include <QDebug>
#include <QDBusMetaType>
#include <QDBusArgument>
#include <QNetworkConfiguration>
#include <QNetworkConfigurationManager>
#include <QNetworkSession>
#include <QByteArray>

#include <memory>
#include "exceptions.h"
#include "nm-dbus-interface.h"


using Connection = QMap<QString, QMap<QString, QVariant>>;
Q_DECLARE_METATYPE(Connection)
Q_DECLARE_METATYPE(QList<QDBusObjectPath>)
Q_DECLARE_METATYPE(NMDeviceType)
Q_DECLARE_METATYPE(QDBusReply<NMDeviceType>)
Q_DECLARE_METATYPE(QList<QString>)

namespace irys {

class WifiHandler : protected QObject {
    Q_OBJECT
public:
    WifiHandler();
    virtual ~WifiHandler();

    void scanForAccessPoints();
    QList<QString> getFoundAccessPoints() const;
    void connectToAccessPoint(const QString &ssid);

private:
    std::unique_ptr<QDBusInterface> m_wirelessDeviceInterface;
    QMap<QString, QDBusObjectPath> m_accessPoints; // <SSID, PATH>

    void recognizeWirelessDevice();
    QList<QDBusObjectPath> getAllDevices() const;
    std::unique_ptr<QDBusInterface> getNetworkManagerInterface() const;
    void findWirelessDeviceIn(const QList<QDBusObjectPath> &devicesPaths);
    std::unique_ptr<QDBusInterface> getDeviceInterfaceFrom(const QDBusObjectPath &devicePath) const;
    bool isWirelessDeviceInterface(std::unique_ptr<QDBusInterface> &deviceInterface) const;
    int getDeviceTypeFrom(std::unique_ptr<QDBusInterface> &deviceInterface) const;
    QVariant getDeviceTypePropertyFrom(std::unique_ptr<QDBusInterface> &deviceInterface) const;
    std::unique_ptr<QDBusInterface> getWirelessDeviceInterfaceFrom(const QDBusObjectPath &devicePath) const;
    std::unique_ptr<QDBusInterface> getAccessPointInterface(const QDBusObjectPath &accessPointPath) const;
    QString getSsidFrom(std::unique_ptr<QDBusInterface> &accessPointInterface) const;
    QVariant getSsidPropertyFrom(std::unique_ptr<QDBusInterface> &accessPointInterface) const;
};

}
#endif // WIFHANDLER_H
