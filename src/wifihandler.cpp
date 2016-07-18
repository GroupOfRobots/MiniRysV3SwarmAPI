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
