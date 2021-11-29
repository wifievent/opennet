#include "opennet.h"
#include "rtmentry.h"
#include <list>
struct Rtm : list<RtmEntry>
{
    Rtm();
    RtmEntry* getBestEntry(Ip ip);
    Ip findGateway(string intfName, Ip ip);
};

