#include "ip.h"
#include <cstdlib>
Ip::Ip(const string& s) {
    pbyte p = pbyte(&ip_);
    int res = sscanf(s.c_str(), "%hhu.%hhu.%hhu.%hhu", &p[0], &p[1], &p[2], &p[3]);
    if (res != SIZE) {
        ip_ = 0;
        return;
    }
    ip_ = ntohl(ip_);
}

Ip::operator string() const {
    char s[INET_ADDRSTRLEN];
    sprintf(s, "%d.%d.%d.%d",
        (ip_ & 0xFF000000) >> 24,
        (ip_ & 0x00FF0000) >> 16,
        (ip_ & 0x0000FF00) >> 8,
        (ip_ & 0x000000FF) >> 0);
    return string(s);
}
