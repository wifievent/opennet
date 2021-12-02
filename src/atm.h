#include "buf.h"
#include "pcapdevice.h"
typedef std::map<Ip, Mac> AtmMap;
struct Atm : public AtmMap, PcapDevice
{
    Atm();
};

