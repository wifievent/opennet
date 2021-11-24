#ifndef BUF_H
#define BUF_H
#include "opennet.h"

class buf
{
public:
    buf(){};
    buf(char* data, int size);
    u_char* data_;
    int size_;
};

#endif // BUF_H
