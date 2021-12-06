#include "mac.h"

Mac::Mac(const std::string& r) {
    std::string s;
    for(char ch: r) {
        if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f'))
            s += ch;
    }
    int res = sscanf(s.c_str(), "%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx", &mac_[0], &mac_[1], &mac_[2], &mac_[3], &mac_[4], &mac_[5]);
    if (res != SIZE) {
        fprintf(stderr, "Mac::Mac sscanf return %d r=%s\n", res, r.c_str());
        return;
    }
}

Mac::operator std::string() const {
    char buf[20]; // enough size
    sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", mac_[0], mac_[1], mac_[2], mac_[3], mac_[4], mac_[5]);
    return std::string(buf);
}

Mac Mac::randomMac() {
    Mac res;
    for (int i = 0; i < SIZE; i++)
        res.mac_[i] = uint8_t(rand() % 256);
    res.mac_[0] &= 0x7F;
    return res;
}

Mac& Mac::nullMac() {
    static uint8_t _value[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    static Mac res(_value);
    return res;
}

Mac& Mac::broadcastMac() {
    static uint8_t _value[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    static Mac res(_value);
    return res;
}
