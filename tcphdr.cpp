#include "tcphdr.h"

buf TcpHdr::parseData(PIpHdr ipHdr_,TcpHdr* tcpHdr_){
    buf ret;
    ret.size_ = ipHdr_->len() - ipHdr_->hl()*4 - tcpHdr_->off()*4;
    if(ret.size_>0){
        ret.data_ = reinterpret_cast<u_char*>(tcpHdr_ + tcpHdr_->off()*4);
    }else{
        ret.data_ = nullptr;
    }
    return ret;
}
