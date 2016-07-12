#include <iostream>
#include "api.h"

int main(int argc, char *argv[]) {
    std::cout << "Hello iRys!" << std::endl;
    irys::API{}.foo();
    return 0;
}
