#ifndef WSCLIENT_H
#define WSCLIENT_H

#include "easywsclient.hpp"
#include "robotstatus.h"
#include <memory>
#include <iostream>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <queue>
#include <future>

namespace irys {
namespace api {

class WSClient
{
    std::unique_ptr<easywsclient::WebSocket> ws;

    //std::thread dispatcher;
    std::future<void> dispatcher;
    std::queue<RobotStatus> statusesToSend;
    std::mutex statusesToSendMutex;
    std::queue<RobotStatus> statusesReceived;
    std::mutex statusesReceivedMutex;
public:
    WSClient();
    void sendStatus(const RobotStatus &rs);
    RobotStatus popReceivedStatus();

    // dispatcher func: has to be run in std::async
    void dispatch();
private:
    void sendToEachRobot(const RobotStatus &rs);
};

} // namespace api
} // namespace irys

#endif // WSCLIENT_H
