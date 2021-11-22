#ifndef IP_H
#define IP_H
#include "opennet.h"

class ip
{
public:
    ip(string str);
    ip(uint32_t tmp);
private:
    char ip_[4];
};

#endif // IP_H
