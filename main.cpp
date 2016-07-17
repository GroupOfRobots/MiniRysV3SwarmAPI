#include <QCoreApplication>
#include "wifhandler.h"
#include "api.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    irys::WifiHandler wifi;
    a.exec();
    return 0;
}
