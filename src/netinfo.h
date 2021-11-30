#include "rtm.h"
struct NetInfo // excute only one
{
private:
    NetInfo();
    virtual ~NetInfo(){}
    IntfList intfList_;
    Rtm rtm_;
public:
    IntfList& intfList(){ return intfList_; }
    Rtm& rtm(){ return rtm_; }

    //becase of singleton
    static NetInfo& instance(){
        static NetInfo netInfo;
        return netInfo;
    }
};
