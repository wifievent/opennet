#pragma once
#include "opennet.h"
struct Mac
{
    Mac() {}
    Mac(const Mac& r) { memcpy(this->mac_, r.mac_, SIZE); }
    Mac(const byte* r) { memcpy(this->mac_, r, SIZE); }
    Mac(const string& str);
    static Mac& nullMac();
    static Mac& broadcastMac();
    const static int SIZE = 6;

    Mac& operator = (const Mac& r) { memcpy(this->mac_, r.mac_, SIZE); return *this; }
    bool operator == (const Mac& r) const { return memcmp(mac_, r.mac_, SIZE) == 0; }
    explicit operator std::string() const;
private:
    byte mac_[SIZE];
};
typedef Mac *PMac;
