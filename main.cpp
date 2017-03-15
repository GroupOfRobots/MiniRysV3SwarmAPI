#include <QCoreApplication>
#include "api.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    irys::SwarmServer *swarmServer = new irys::SwarmServer;
    QObject::connect(swarmServer, &irys::SwarmServer::serverClosed, &a, &QCoreApplication::quit);
    return a.exec();
}
