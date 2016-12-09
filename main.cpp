#include "robotstatus.h"
#include "wsclient.h"
#include "wsserver.h"
#include <iostream>

int main(int argc, char *argv[]) {
    irys::api::WSClient client;
    if (argc == 1) client.processMessage();
    else new irys::api::WSServer;
}
