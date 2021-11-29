#include "opennet.h"

struct Buf
{
    Buf(){};
    Buf(unsigned char* data, size_t size);
    gbyte* data_;
    size_t size_;
    bool valid() {
        return data_ != nullptr;
    }
    void clear(){
        data_= nullptr;
        size_ = 0;
    }
};
