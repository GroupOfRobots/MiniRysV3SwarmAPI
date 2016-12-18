#include "robotstatus.h"
#include "wsclient.h"
//#include "wsserver.h"
#include <iostream>
#include <thread>

int main(int argc, char *argv[]) {
    irys::api::WSClient client;
    irys::api::RobotStatus rs;
    rs.id = 12;
    //std::async
    //client.processReceivedRobotStatus();
    client.sendToEachRobot(rs);
    client.processReceivedRobotStatus();
}
