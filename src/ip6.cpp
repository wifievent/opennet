#include "ip6.h"
#include <arpa/inet.h>
Ip6::Ip6(string& str) {
    char* p = const_cast<char*>(str.data());
    int res = inet_pton(AF_INET6, p, &ip6_);//p change to ipv6, AF_INET6 == frame of IPv6
    if (res == 1) { // succeed
        printf("succeed");
    } else { // fail
        memset(ip6_, 0, SIZE);
    }
}

Ip6::Ip6(uint32_t ip){
    int cnt = 0;
    long div = 10000000000000;
    while(div>0){
        ip6_[cnt++] = ip/div;
        ip %= div;
        div/=100;
    }
}
