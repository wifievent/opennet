#include "opennet.h"
#include "rtmentry.h"
#include <list>
struct Rtm : list<RtmEntry>
{
    Rtm();
    RtmEntry* getBestEntry(Ip ip);
    Ip findGateway(string intfName, Ip ip);
    static bool checkA(char* buf, RtmEntry* entry);
    static bool checkB(char* buf, RtmEntry* entry);
    static bool checkC(char* buf, RtmEntry* entry);
    static bool checkD(char* buf, RtmEntry* entry);
    static bool decodeCidr(std::string cidr, uint32_t* dst, uint32_t* mask);
    static uint32_t numberToMask(int number);
};

