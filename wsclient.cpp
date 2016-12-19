#include "wsclient.h"

irys::api::WSClient::WSClient()
    : ws(easywsclient::WebSocket::from_url("ws://localhost:8080"))
{ }

void irys::api::WSClient::sendStatus(const RobotStatus &rs) {
    std::lock_guard<std::mutex> g (statusesToSendMutex);
    statusesToSend.push(rs);
}

irys::api::RobotStatus irys::api::WSClient::popReceivedStatus() {
    std::lock_guard<std::mutex> g (statusesReceivedMutex);
    auto rs = statusesReceived.front();
    statusesReceived.pop();
    return rs;
}

void irys::api::WSClient::dispatch() {
    while (ws && ws->getReadyState() != easywsclient::WebSocket::CLOSED) {
        easywsclient::WebSocket::pointer wsp = &*ws; // <-- because a unique_ptr cannot be copied into a lambda
        // fetch arrived messages
        ws->poll();
        ws->dispatchBinary([wsp,this](const std::vector<uint8_t> &bin) {
            const RobotStatus *rs = reinterpret_cast<const RobotStatus*>(bin.data());
            std::cout << "Robot status received: id: " << rs->id << "\n";

            std::lock_guard<std::mutex> g (statusesReceivedMutex);
            statusesReceived.push(std::move(*rs));
        });

        // send if any robot statuses
        std::unique_lock<std::mutex> lock (statusesToSendMutex, std::try_to_lock);
        if (lock) {
            while (!statusesToSend.empty()) {
                ws->poll();
                sendToEachRobot(statusesToSend.front());
                statusesToSend.pop();
            }
        }
    }
    std::cout << "Server doesn't working!\n";
}

void irys::api::WSClient::sendToEachRobot(const RobotStatus &rs) {
    const uint8_t *data = reinterpret_cast<const uint8_t*>(&rs);
    std::vector<uint8_t> status(data, data + sizeof(RobotStatus));
    ws->sendBinary(status);
}
