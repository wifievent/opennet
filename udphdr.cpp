#include "udphdr.h"

Buf UdpHdr::parseData(UdpHdr* udpHdr_){
    Buf ret;
    ret.size_ = sizeof(udpHdr_) - sizeof(UdpHdr);
    if(ret.size_ > 0){
        ret.data_ = reinterpret_cast<u_char*>(udpHdr_ + sizeof(UdpHdr));
    }else{
        ret.data_ = nullptr;
    }
    return ret;
}
