#pragma once
#include "opennet.h"
struct Ip6
{
    static const int SIZE = 16;
    Ip6(string& str);
    Ip6(uint32_t ip);
    Ip6(){};
protected:
    char ip6_[SIZE];
};
