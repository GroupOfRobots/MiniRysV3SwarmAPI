#include "wsclient.h"

irys::api::WSClient::WSClient() {
#ifdef _WIN32
    rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (rc) {
        printf("WSAStartup Failed.\n");
        return 1;
    }
#endif
    ws = std::unique_ptr<easywsclient::WebSocket>(easywsclient::WebSocket::from_url("ws://localhost:8080"));
}

void irys::api::WSClient::processReceivedRobotStatus() {
    if (ws && ws->getReadyState() != easywsclient::WebSocket::CLOSED) {
        easywsclient::WebSocket::pointer wsp = &*ws; // <-- because a unique_ptr cannot be copied into a lambda
        ws->poll();
        ws->dispatchBinary([wsp](const std::vector<uint8_t> &bin) {
            const RobotStatus *rs = reinterpret_cast<const RobotStatus*>(bin.data());
            std::cout << "Robot status received: id: " << rs->id << "\n";
        });
    }
    else
        std::cout << "Server doesn't working!\n";
}

void irys::api::WSClient::sendToEachRobot(const RobotStatus &rs) {
    ws->poll();
    const uint8_t *data = reinterpret_cast<const uint8_t*>(&rs);
    std::vector<uint8_t> status(data, data + sizeof(RobotStatus));
    ws->sendBinary(status);
}
