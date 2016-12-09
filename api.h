#ifndef API_H
#define API_H

#include <functional>
#include <iostream>

namespace irys {

class API {
public:
    API();
    virtual ~API();

    /// Sends byte-array to the specific robot with passed id.
    void sendDataToRobotWithId(int id, const char *data);

    /// Sends byte-array to each robot.
    void sendDataToEachRobot(const char *data);

    /// Sends string to each robot
    void sendTextToEachRobot(const std::string &text);

private:
    // wsockets side should be separated!

};

}
#endif // API_H
