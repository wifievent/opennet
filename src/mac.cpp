#include "mac.h"
Mac::Mac(const string& r) {
    std::string s;
    for (char ch : r) {
        if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f'))
            s += ch;
    }
    int res = sscanf(s.c_str(), "%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx", &mac_[0], &mac_[1], &mac_[2], &mac_[3], &mac_[4], &mac_[5]);
    if (res != SIZE) {
        memset(mac_, 0, SIZE);
    }
}
