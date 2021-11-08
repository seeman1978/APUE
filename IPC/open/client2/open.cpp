#include "open.h"
#include <sys/uio.h>    ///struct iovec

/// open the file by sending the 'name' and 'oflag' to the connection server and reading a file descriptor back.
int csopen(char* name, int oflag){
    int len;
    static int csfd{-1};
    size_t len{0};
    char buf[12];
    struct iovec iov[3];

    if (csfd < 0){ /// open connection to conn server
        if (csfd = cli_conn(CS_OPEN); csfd < 0){
            err_ret("cli_conn error");
            return -1;
        }
    }
    sprintf(buf, " %d", oflag); ///oflag to ascii
    iov[0].iov_base = (void *) CL_OPEN " ";  ///hat' s this? string concatanation
    iov[0].iov_len = strlen(CL_OPEN) + 1;
    iov[1].iov_base = name;
    iov[1].iov_len = strlen(name);
    iov[2].iov_base = buf;
    iov[2].iov_len = strlen(buf) + 1;///+1 for null at end of buf
    for (auto item: iov) {
        len += item.iov_len;
    }
    if (writev(fd[0], &iov[0], 3) != len){
        err_ret("writev error");
        return -1;
    }
    /// read descriptor , returned errors handled by write()
    return recv_fd(csfd, write);
}