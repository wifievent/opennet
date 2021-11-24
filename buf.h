#ifndef BUF_H
#define BUF_H


class buf
{
public:
    buf(char* packet, int size);
private:
    char* packet;
    int size;
};

#endif // BUF_H
