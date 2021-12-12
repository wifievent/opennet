#pragma once
#include "intf.h"
struct IntfList: std::list<Intf>
{
    Intf* findByName(std::string name);
    Intf* findByIp(Ip ip);
    Mac getMac(char* intfName);
    IntfList();//find all interface
};

