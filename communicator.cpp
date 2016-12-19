#include "communicator.h"

irys::api::Communicator::Communicator()
    : client(),
      dispatcher(std::async(std::launch::async, &WSClient::dispatch, &client))
{ }


void irys::api::Communicator::send(const RobotStatus &rs) {
    client.sendStatus(rs);
}

irys::api::RobotStatus irys::api::Communicator::getReceivedStatus() {
    return client.popReceivedStatus();
}
