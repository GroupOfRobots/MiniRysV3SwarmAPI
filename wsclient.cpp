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

void irys::api::WSClient::processMessage() {
    while (ws->getReadyState() != easywsclient::WebSocket::CLOSED) {
        easywsclient::WebSocket::pointer wsp = &*ws; // <-- because a unique_ptr cannot be copied into a lambda
        ws->poll();
        ws->dispatch([wsp](const std::string &message) {
            //RobotStatus *rs = reinterpret_cast<RobotStatus*>(bin.data());
            //std::cout << "Robot status received: id: " << rs->id; << "\n";
            std::cout << message << '\n';
            //printf(">>> %s\n", message.c_str());
            //if (message == "world") { wsp->close(); }
        });
    }
}
