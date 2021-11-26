#ifndef IP6_H
#define IP6_H

#include "opennet.h"
class Ip6
{
public:
    static const int SIZE = 16;
    Ip6(string& str);
    Ip6(uint32_t ip);
    Ip6(){};
protected:
    char ip6_[SIZE];
};

#endif // IP6_H
