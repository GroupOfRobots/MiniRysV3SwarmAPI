#ifndef API_H
#define API_H

#include <iostream>

namespace irys {

class API {
public:
    API();
    virtual ~API();

    void foo() const;
};

}
#endif // API_H
