#include "netinfo.h"

NetInfo::NetInfo()
{
    for(RtmEntry& entry : rtm_){
        std::string intfName = entry.intfName_;
        Intf* intf = intfList_.findByName(intfName);
        if(intf == nullptr){
            return;
        }
        entry.intf_ = intf;
    }

    for(Intf& intf: intfList_){
        intf.gateway_ = rtm_.findGateway(intf.name_,intf.ip_);
    }
}
