#ifndef ARPHDR_H
#define ARPHDR_H
#include "opennet.h"
#include "mac.h"
#include "ip.h"
class arphdr
{
public:
    uint8_t htype;
    uint16_t ptype;
    uint8_t hsize;
    uint8_t psize;
    uint8_t op;
    mac smac;
    mac tmac;
    ip sip;
    ip tip;
};

#endif // ARPHDR_H
