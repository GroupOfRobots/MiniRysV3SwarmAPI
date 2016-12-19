#include "robotstatus.h"
#include "wsclient.h"
//#include "wsserver.h"
#include <iostream>
#include <thread>
#include <future>
#include "communicator.h"

int main(int argc, char *argv[]) {
    //irys::api::WSClient client;
    irys::api::RobotStatus rs;
    rs.id = 12;
    irys::api::Communicator c;
    //auto handle = std::async(std::launch::async, &irys::api::WSClient::dispatch, &client);
    //std::thread t (&irys::api::WSClient::dispatch, &client);
    //client.processReceivedRobotStatus();
    //client.dispatch();
    for (int i=0; i < 10; ++i)
        c.send(rs);
}
