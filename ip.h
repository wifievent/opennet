#ifndef IP_H
#define IP_H
#include "opennet.h"

class Ip
{
public:
    Ip(){};
    Ip(string& str);
    Ip(uint32_t ip);
    const static int SIZE = 4;
private:
    char ip_[SIZE];
};

#endif // IP_H
