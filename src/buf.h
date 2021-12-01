#pragma once
#include "ip.h"
#include "mac.h"
struct Buf
{
    Buf(){};
    Buf(unsigned char* data, size_t size);
    byte* data_;
    size_t size_;

    bool valid() {
        return data_ != nullptr;
    }
    void clear(){
        data_= nullptr;
        size_ = 0;
    }
};
