#ifndef IP_H
#define IP_H
#include "opennet.h"

class ip
{
public:
    ip(string str);
    ip(uint32_t ip);
    const static int SIZE = 4;
private:
    char ip_[4];
};

#endif // IP_H
