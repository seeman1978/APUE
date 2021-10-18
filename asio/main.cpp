#include <iostream>
#include <aio.h>
#include "apue.h"

constexpr auto BSZ = 4096;
constexpr auto NBUF = 8;

enum rwop{
    UNUSED = 0,
    READ_PENDING = 1,
    WRITE_PENDING = 2
};

struct  buf{
    enum rwop op;
    int last;
    struct aiocb aiocb;
    unsigned char data[BSZ];
};

struct buf bufs[NBUF];
unsigned char translate(unsigned char c){
    if (isalpha(c)){
        if (c >= 'n'){
            c -= 13;
        }
        else if (c >= 'a'){
            c += 13;
        }
        else if (c >= 'N'){
            c -= 13;
        }
        else {
            c += 13;
        }
    }
    return c;
}

int main() {

    return 0;
}
