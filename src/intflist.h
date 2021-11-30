#include "intf.h"
struct IntfList: std::list<Intf>
{
    Intf* findByName(std::string name);
    Intf* findByIp(Ip ip);
    static Mac getMac(char* intfName);
    IntfList();
};

