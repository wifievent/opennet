#include "tcphdr.h"
Buf TcpHdr::parseData(PIpHdr ipHdr_,TcpHdr* tcpHdr_){
    Buf ret;
    ret.size_ = ipHdr_->tlen() - ipHdr_->hlen()*4 - tcpHdr_->off()*4;
    if(ret.size_>0){
        ret.data_ = reinterpret_cast<u_char*>(tcpHdr_ + tcpHdr_->off()*4);
    }else{
        ret.data_ = nullptr;
    }
    return ret;
}

uint16_t TcpHdr::calcChecksum(IpHdr* ipHdr, TcpHdr* tcpHdr) {
    uint32_t res = 0;
    int tcpHdrDataLen = ipHdr->hlen() - sizeof(IpHdr);

    // Add tcpHdr & data buffer as array of uint16_t
    uint16_t* p = reinterpret_cast<uint16_t*>(tcpHdr);
    for (int i = 0; i < tcpHdrDataLen / 2; i++) {
        res += htons(*p);
        p++;
    }

    // If length is odd, add last data(padding)
    if ((tcpHdrDataLen / 2) * 2 != tcpHdrDataLen)
        res += uint32_t(*(reinterpret_cast<uint8_t*>(p)) << 8);

    // Decrease checksum from sum
    res -= tcpHdr->sum();

    // Add src address
    uint32_t src = ipHdr->sip();
    res += ((src & 0xFFFF0000) >> 16) + (src & 0x0000FFFF);

    // Add dst address
    uint32_t dst = ipHdr->dip();
    res += ((dst & 0xFFFF0000) >> 16) + (dst & 0x0000FFFF);

    // Add extra information
    res += uint32_t(tcpHdrDataLen) + IPPROTO_TCP;

    // Recalculate sum
    while (res >> 16) {
        res = (res & 0xFFFF) + (res >> 16);
    }
    res = ~res;

    return uint16_t(res);
}
