#include "wifihandler.h"

irys::WifiHandler::WifiHandler() {
    recognizeWirelessDevice();
    qDebug() << m_wirelessDeviceInterface->path();
}

void irys::WifiHandler::recognizeWirelessDevice() {
    QList<QDBusObjectPath> devices = getAllDevices();
    findWirelessDeviceIn(devices);
}

QList<QDBusObjectPath> irys::WifiHandler::getAllDevices() const {
    std::unique_ptr<QDBusInterface> NMInterface = getNetworkManagerInterface();
    QDBusReply<QList<QDBusObjectPath>> busReply = NMInterface->call("GetAllDevices");
    if (!busReply.isValid()) {
        throw GetAllDevicesUnsuccessful();
    }
    return busReply.value();
}

std::unique_ptr<QDBusInterface> irys::WifiHandler::getNetworkManagerInterface() const {
    return std::make_unique<QDBusInterface>(NM_DBUS_SERVICE,
                                            NM_DBUS_PATH,
                                            NM_DBUS_INTERFACE,
                                            QDBusConnection::systemBus()
                                            );
}

void irys::WifiHandler::findWirelessDeviceIn(const QList<QDBusObjectPath> &devicesPaths) {
    for (const auto &devicePath : devicesPaths) {
        std::unique_ptr<QDBusInterface> deviceInterface = getDeviceInterfaceFrom(devicePath);
        if (isWirelessDeviceInterface(deviceInterface)) {
            m_wirelessDeviceInterface = getWirelessDeviceInterfaceFrom(devicePath);
            return;
        }
    }
    throw WirelessDeviceNotFound();
}

std::unique_ptr<QDBusInterface> irys::WifiHandler::getDeviceInterfaceFrom(const QDBusObjectPath &devicePath) const {
    return std::make_unique<QDBusInterface>(NM_DBUS_SERVICE,
                                            devicePath.path(),
                                            NM_DBUS_INTERFACE_DEVICE,
                                            QDBusConnection::systemBus()
                                            );
}

bool irys::WifiHandler::isWirelessDeviceInterface(std::unique_ptr<QDBusInterface> &deviceInterface) const {
    int deviceType = getDeviceTypeFrom(deviceInterface);
    return deviceType == NMDeviceType::NM_DEVICE_TYPE_WIFI;
}

int irys::WifiHandler::getDeviceTypeFrom(std::unique_ptr<QDBusInterface> &deviceInterface) const {
    return qvariant_cast<int>(getDeviceTypePropertyFrom(deviceInterface));
}

QVariant irys::WifiHandler::getDeviceTypePropertyFrom(std::unique_ptr<QDBusInterface> &deviceInterface) const {
    return deviceInterface->property("DeviceType");
}

std::unique_ptr<QDBusInterface> irys::WifiHandler::getWirelessDeviceInterfaceFrom(const QDBusObjectPath &devicePath) const {
    return std::make_unique<QDBusInterface>(NM_DBUS_SERVICE,
                                            devicePath.path(),
                                            NM_DBUS_INTERFACE_DEVICE_WIRELESS,
                                            QDBusConnection::systemBus()
                                            );
}

irys::WifiHandler::~WifiHandler() { }

void irys::WifiHandler::scanForAccessPoints() {
    m_wirelessDeviceInterface->call("RequestScan");
    QDBusReply<QList<QDBusObjectPath>> busReply = m_wirelessDeviceInterface->call("GetAllAccessPoints");
    if (!busReply.isValid()) {
        throw GetAllAccessPointsFailed();
    }
    m_accessPoints.clear();
    for (auto &accessPointPath : busReply.value()) {
        std::unique_ptr<QDBusInterface> accessPoint = getAccessPointInterface(accessPointPath);
        QString ssid = getSsidFrom(accessPoint);
        m_accessPoints[ssid] = accessPointPath;
    }
}

std::unique_ptr<QDBusInterface> irys::WifiHandler::getAccessPointInterface(const QDBusObjectPath &accessPointPath) const {
    return std::make_unique<QDBusInterface>(NM_DBUS_SERVICE,
                                            accessPointPath.path(),
                                            NM_DBUS_INTERFACE_ACCESS_POINT,
                                            QDBusConnection::systemBus()
                                            );
}

QString irys::WifiHandler::getSsidFrom(std::unique_ptr<QDBusInterface> &accessPointInterface) const {
    return qvariant_cast<QString>(getSsidPropertyFrom(accessPointInterface));
}

QVariant irys::WifiHandler::getSsidPropertyFrom(std::unique_ptr<QDBusInterface> &accessPointInterface) const {
    return accessPointInterface->property("Ssid");
}

QList<QString> irys::WifiHandler::getFoundAccessPoints() const {
    QList<QString> ssids = m_accessPoints.keys();
    for (auto &el : ssids) {
        qDebug() << el;
    }
    return ssids;
}

void irys::WifiHandler::connectToAccessPoint(const QString &ssid) {
    std::unique_ptr<QDBusInterface> nmsettings(new QDBusInterface(NM_DBUS_SERVICE,
                                                                  NM_DBUS_PATH_SETTINGS,
                                                                  NM_DBUS_INTERFACE_SETTINGS,
                                                                  QDBusConnection::systemBus()));
    qDBusRegisterMetaType<Connection>();
    /*Connection connection;
    connection["connection"]["uuid"] = QUuid::createUuid().toString().remove('{').remove('}');
    connection["connection"]["id"] = ssid;
    connection["connection"]["autoconnect"] = "true";
    // build up ipv4 settings
    connection["ipv4"]["method"] = "auto";
    // use IEEE 802-11 wireless standard
    connection["connection"]["type"] = "802-11-wireless";
    // build up the "802-11-wireless" setting
    connection["802-11-wireless"];
    // tell which ssid should be chosen
    connection["802-11-wireless"]["ssid"] = ssid;
    // choose connection mode - adhoc - connect to existing access point
    //connection["802-11-wireless"]["mode"] = "adhoc";
    // security wpa
    //connection["802-11-wireless"]["security"] = "802-11-wireless-security";


    // start the connection
    QDBusReply<QDBusObjectPath> result = nmsettings->call("ActivateConnection",
                                                          QVariant::fromValue(connection),
                                                          m_wirelessDeviceInterface->path(),
                                                          m_accessPoints[ssid].path());
    if (result.isValid()) {
        qDebug() << "Connected to: " << ssid;
    }*/

    // FOR NOW: reverse engineering to get how connections looks like
    QDBusReply<QList<QDBusObjectPath>> result = nmsettings->call("ListConnections");
    for (auto &el : result.value()) {
        qDebug() << el.path();
        QDBusInterface conn(NM_DBUS_SERVICE,
                            el.path(),
                            NM_DBUS_INTERFACE_SETTINGS_CONNECTION,
                            QDBusConnection::systemBus());
        QDBusReply<Connection> set = conn.call("GetSettings");
        auto map = set.value();
        for (auto &key1 : map.keys()) {
            for(auto &key2 : map[key1].keys()) {
                qDebug() << "[" << key1 << "][" << key2 << "] = " << map[key1][key2];
            }
        }
    }

}
