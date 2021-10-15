#include <iostream>
#include "apue.h"
#include <fcntl.h>
#include <cerrno>
char buf[500000];

int main() {
    ssize_t ntowrite, nwrite;
    char *ptr;

    ntowrite = read(STDIN_FILENO, buf, sizeof buf);
    fprintf(stderr, "read %ld bytes\n", ntowrite);
    set_fl(STDOUT_FILENO, O_NONBLOCK); ///SET NONBLOCKING

    ptr = buf;
    while (ntowrite > 0){
        errno = 0;
        nwrite = write(STDOUT_FILENO, ptr, ntowrite);
        fprintf(stderr, "nwrite = %ld, errno = %d\n", nwrite, errno);
        if (nwrite > 0){
            ptr += nwrite;
            ntowrite -= nwrite;
        }
    }
    clr_fl(STDOUT_FILENO, O_NONBLOCK);
    return 0;
}
