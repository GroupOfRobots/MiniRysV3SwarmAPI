#include "wifihandler.h"

irys::WifiHandler::WifiHandler()
    : m_NetworkManagerInterface(
          NM_DBUS_SERVICE,
          NM_DBUS_PATH,
          NM_DBUS_INTERFACE,
          QDBusConnection::systemBus()
          ) {
    m_wirelessDevicePath = recognizeWirelessDevicePath();
    qDebug() << m_wirelessDevicePath.path();
}

QDBusObjectPath irys::WifiHandler::recognizeWirelessDevicePath() {
    QList<QDBusObjectPath> devices = getAllDevices();
    return findWirelessDeviceIn(devices);
}

QList<QDBusObjectPath> irys::WifiHandler::getAllDevices() {
    QDBusReply<QList<QDBusObjectPath>> busReply = m_NetworkManagerInterface.call("GetAllDevices");
    if (!busReply.isValid()) {
        throw GetAllDevicesUnsuccessful();
    }
    return busReply.value();
}

QDBusObjectPath irys::WifiHandler::findWirelessDeviceIn(QList<QDBusObjectPath> devices) {
    for (auto &device : devices) {
        QDBusInterface deviceInterface(NM_DBUS_INTERFACE,
                                       device.path(),
                                       NM_DBUS_INTERFACE_DEVICE,
                                       QDBusConnection::systemBus());
        QVariant busReply = deviceInterface.property("DeviceType");
        int deviceType = qvariant_cast<int>(busReply);
        if (deviceType == NMDeviceType::NM_DEVICE_TYPE_WIFI) {
            return device;
        }
    }
    throw WirelessDeviceNotFound();
}

irys::WifiHandler::~WifiHandler() { }


