#include "mac.h"
mac::mac(string str) {
    if(str.length()!=11){
        printf("wrong input");
        return;
    }
    char tmp[11];
    strcpy(tmp,str.c_str());
    char * token = strtok(tmp,":");
    int cnt = 0;
    while(token!=NULL){
        mac_[cnt++] = *token;
        token = strtok(NULL,":");
    }
}

mac::mac(uint32_t mac) {

}
