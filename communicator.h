#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include "wsclient.h"
#include "robotstatus.h"
#include <future>

namespace irys {
namespace api {


class Communicator
{
    WSClient client;
    std::future<void> dispatcher;
public:
    Communicator();
    void send(const RobotStatus &rs);
    RobotStatus getReceivedStatus();
};


} // irys
} // api

#endif // COMMUNICATOR_H
