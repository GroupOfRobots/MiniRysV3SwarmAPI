#include "api.h"

irys::API::API() {

}

irys::API::~API() { }

void irys::API::foo() const {
    std::cout << "Hello from foo!\n";
}
