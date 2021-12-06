#pragma once

#include "ip.h"
#include "mac.h"

struct Buf final {
    uint8_t* data_;
    size_t size_;

    Buf() {}
    Buf(uint8_t* data, size_t size) : data_(data), size_(size) {}

    void clear() {
        data_ = nullptr;
        size_ = 0;
    }

    bool valid() {
        return data_ != nullptr;
    }
};
