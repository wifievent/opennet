#include "opennet.h"
#include "intf.h"
#include "mac.h"
#include "ip.h"
#include <list>
struct IntfList: std::list<Intf>
{
    Intf* findByName(std::string name);
    Intf* findByIp(Ip ip);
    static Mac getMac(char* intfName);
    IntfList();
};

