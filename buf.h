#ifndef BUF_H
#define BUF_H
#include "opennet.h"

class buf
{
public:
    buf(){};
    buf(unsigned char* data, size_t size);
    gbyte* data_;
    size_t size_;
    bool valid() {
        return data_ != nullptr;
    }
};

#endif // BUF_H
