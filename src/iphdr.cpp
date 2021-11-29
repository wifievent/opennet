#include "iphdr.h"

uint16_t IpHdr::calcChecksum(IpHdr* ipHdr) {
    uint32_t res = 0;
    uint16_t *p;

    // Add ipHdr buffer as array of uint16_t
    p = reinterpret_cast<uint16_t*>(ipHdr);
    for (int i = 0; i < int(sizeof(IpHdr)) / 2; i++) {
        res += ntohs(*p);
        p++;
    }

    // Do not consider padding because ip header length is always multilpe of 2.

    // Decrease checksum from sum
    res -= ipHdr->sum();

    // Recalculate sum
    while (res >> 16) {
        res = (res & 0xFFFF) + (res >> 16);
    }
    res = ~res;

    return uint16_t(res);
}
