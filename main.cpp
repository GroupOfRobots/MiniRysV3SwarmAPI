#include <QCoreApplication>
#include "api.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
//    irys::WifiHandler wifi;
//    wifi.scanForAccessPoints();
//    wifi.getFoundAccessPoints();
//    wifi.connectToAccessPoint("TP-LINK_CD5DC8");
    irys::Communicaton *api = new irys::Communicaton;
    QObject::connect(api, &irys::Communicaton::serverClosed, &a, &QCoreApplication::quit);
    return a.exec();
}
