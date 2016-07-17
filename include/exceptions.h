#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

namespace irys {

class RuntimeError : public std::runtime_error {
public:
    RuntimeError(const std::string& whatArg) : std::runtime_error(whatArg)
    { }
};

class DataTransferToRobotUnsuccessful : public RuntimeError {
public:
    DataTransferToRobotUnsuccessful(int id)
        : RuntimeError("Unsuccessful data transfer to a robot with id: ")
    { }
};

class GetAllDevicesUnsuccessful : public RuntimeError {
public:
    GetAllDevicesUnsuccessful()
        : RuntimeError("Unable to get devices list from NM interface")
    { }
};

class GetDeviceTypeUnsuccessful : public RuntimeError {
public:
    GetDeviceTypeUnsuccessful()
        : RuntimeError("Unable to get device type")
    { }
};

class WirelessDeviceNotFound : public RuntimeError {
public:
    WirelessDeviceNotFound()
        : RuntimeError("No wireless device found")
    { }
};

}

#endif // EXCEPTIONS_H
