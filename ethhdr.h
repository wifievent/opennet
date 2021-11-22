#ifndef ETHHDR_H
#define ETHHDR_H
#include "opennet.h"
#include "ip.h"
#include "mac.h"
class ethhdr
{
public:
    mac dmac_;
    mac smac_;
    uint16_t type;
};

#endif // ETHHDR_H
