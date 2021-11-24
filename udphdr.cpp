#include "udphdr.h"

buf UdpHdr::parseData(UdpHdr* udpHdr_){
    buf ret;
    ret.size_ = sizeof(udpHdr_) - sizeof(UdpHdr);
    if(ret.size_ > 0){
        ret.data_ = reinterpret_cast<u_char*>(udpHdr_ + sizeof(UdpHdr));
    }else{
        ret.data_ = nullptr;
    }
    return ret;
}
