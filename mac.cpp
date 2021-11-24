#include "mac.h"
mac::mac(string str) {
    if(str.length()!=17){
        printf("wrong input");
        return;
    }
    char tmp[17];
    strcpy(tmp,str.c_str());
    char * token = strtok(tmp,":");
    int cnt = 0;
    while(token!=NULL){
        mac_[cnt++] = *token;
        token = strtok(NULL,":");
    }
}

mac::mac(uint32_t mac) {
    int cnt = 0;
    int div = 1000000000;
    while(div>0){
        mac_[cnt++] = mac/div;
        mac %= div;
        div/=100;
    }
}
mac mac::nullMac(){
    return mac("00:00:00:00:00:00");
}
mac mac::broadcastMac(){
    return mac("FF:FF:FF:FF:FF:FF");
}
