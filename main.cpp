#include "robotstatus.h"
#include <iostream>
#include <thread>
#include <future>
#include "communicator.h"

int main(int argc, char *argv[]) {
    irys::api::RobotStatus rs;
    rs.id = 12;

    irys::api::Communicator c;
    for (int i=0; i < 10; ++i)
        c.send(rs);
}
