#pragma once
#include "rtmentry.h"

struct Rtm : std::list<RtmEntry> { // routing table manager
    friend struct RtmEntry;
    friend struct NetInfo;

    Rtm();
    virtual ~Rtm() { clear(); }

public:
    RtmEntry* getBestEntry(Ip ip);
    Ip findGateway(std::string intfName, Ip ip);


protected:
    static bool checkA(char* buf, RtmEntry* entry);
    static bool checkB(char* buf, RtmEntry* entry);
    static bool checkC(char* buf, RtmEntry* entry);
    static bool checkD(char* buf, RtmEntry* entry);
    static bool decodeCidr(std::string cidr, uint32_t* dst, uint32_t* mask);
    static uint32_t numberToMask(int number);
};
