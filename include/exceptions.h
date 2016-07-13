#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

namespace irys {

class Exception : public std::exception {
    Exception(const std::string& whatArg) : std::exception(whatArg)
    { }
};

class RuntimeError : public std::runtime_error {
    RuntimeError(const std::string& whatArg) : std::runtime_error(whatArg)
    { }
};

class DataTransferToRobotUnsuccessful : public RuntimeError {
    DataTransferToRobotUnsuccessful(int id)
        : RuntimeError("Unsuccessful data transfer to a robot with id: ")
    { }
};

}

#endif // EXCEPTIONS_H
