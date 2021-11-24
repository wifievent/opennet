#ifndef MAC_H
#define MAC_H
#include "opennet.h"

class mac
{
public:
    mac(string str);
    mac(uint32_t mac);
    mac nullMac();
    mac broadcastMac();
    const static int SIZE = 6;
private:
    char mac_[6];
};

#endif // MAC_H
