#include "rtmentry.h"
struct Rtm : list<RtmEntry>
{
    Rtm();
    RtmEntry* getBestEntry(Ip ip);
    Ip findGateway(string intfName, Ip ip);
};

