#include <QCoreApplication>
#include "wifihandler.h"
#include "api.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    irys::WifiHandler wifi;
    wifi.scanForAccessPoints();
    wifi.getFoundAccessPoints();
    wifi.connectToAccessPoint("TP-LINK_CD5DC8");
    return 0;
}
