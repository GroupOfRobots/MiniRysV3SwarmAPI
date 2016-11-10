#include <QCoreApplication>
#include <QDebug>
#include "wifihandler.h"
#include "api.h"
#include "robotstatus.h"

int main(int argc, char *argv[]) {
    qDebug() << sizeof(irys::api::RobotStatus);
    /*
    QCoreApplication a(argc, argv);
//    irys::WifiHandler wifi;
//    wifi.scanForAccessPoints();
//    wifi.getFoundAccessPoints();
//    wifi.connectToAccessPoint("TP-LINK_CD5DC8");
    irys::API *api = new irys::API;
    QObject::connect(api, &irys::API::serverClosed, &a, &QCoreApplication::quit);
    return a.exec();
    */
}
