#ifndef IP_H
#define IP_H
#include "opennet.h"

class Ip
{
public:
    Ip(){};
    Ip(const string& r);
    Ip(const uint32_t r) : ip_(r) {}
    const static int SIZE = 4;
    explicit operator string() const;
    Ip& operator = (const Ip& r) { ip_ = r.ip_; return *this; }
private:
    uint32_t ip_;
};

#endif // IP_H
