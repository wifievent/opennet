#pragma once
#include "opennet.h"
struct Ip
{
    Ip(){};
    Ip(const string& r);
    Ip(const uint32_t r) : ip_(r) {}
    const static int SIZE = 4;

    operator uint32_t() const { return ip_; } // default -> not explicit == auto
    explicit operator string() const;
    Ip& operator = (const Ip& r) { ip_ = r.ip_; return *this; }

private:
    uint32_t ip_;
};
