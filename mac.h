#ifndef MAC_H
#define MAC_H
#include "opennet.h"

class Mac
{
public:
    Mac() {}
    Mac(const Mac& r) { memcpy(this->mac_, r.mac_, SIZE); }
    Mac(const gbyte* r) { memcpy(this->mac_, r, SIZE); }
    Mac(const string& str);
    static Mac& nullMac();
    static Mac& broadcastMac();
    const static int SIZE = 6;
private:
    gbyte mac_[SIZE];
};
typedef Mac *PMac;
#endif // MAC_H