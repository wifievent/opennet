#include "ip.h"

Ip::Ip(string& str) {
    char tmp[14];
    strcpy(tmp,str.c_str());
    char * token = strtok(tmp,".");
    int cnt = 0;
    while(token!=NULL){
        ip_[cnt++] = *token;
        token = strtok(NULL,".");
    }
}

Ip::Ip(uint32_t ip){
    int cnt = 0;
    int div = 100000;
    while(div>0){
        ip_[cnt++] = ip/div;
        ip %= div;
        div/=100;
    }
}
