#pragma once

#include "ip.h"
#include "mac.h"

struct Buf final {
    unsigned char* data_;
    size_t size_;

    Buf() {}
    Buf(unsigned char* data, size_t size) : data_(data), size_(size) {}

    void clear() {
        data_ = nullptr;
        size_ = 0;
    }

    bool valid() {
        return data_ != nullptr;
    }
};
