#include <fcntl.h>
#include "apue.h"
#include <sys/mman.h>
constexpr auto COPYINCR = (1024*1024*1024);    /// 1GB

int main(int argc, char** argv) {
    int fdin, fdout;
    long copysz;
    struct stat sbuf{};
    off_t fsz = 0;
    if (argc != 3){
        err_quit("usage: %s <fromfile> <tofile>", argv[0]);
    }
    if (fdin = open(argv[1], O_RDONLY); fdin < 0){
        err_sys("can't open %s for reading", argv[1]);
    }
    if (fdout = open(argv[2], O_RDWR|O_CREAT|O_TRUNC, FILE_MODE); fdout < 0){
        err_sys("can't creat %s for writing", argv[2]);
    }
    if (fstat(fdin, &sbuf) < 0){
        err_sys("fstat error");
    }
    if (ftruncate(fdout, sbuf.st_size) < 0){
        err_sys("ftruncate error");
    }

    void *src, *dst;

    while (fsz < sbuf.st_size){
        if ((sbuf.st_size - fsz) > COPYINCR){
            copysz = COPYINCR;
        }
        else{
            copysz = sbuf.st_size - fsz;
        }
        if (src = mmap(nullptr, copysz, PROT_READ, MAP_SHARED, fdin, fsz); src == MAP_FAILED){
            err_sys("mmap error for input");
        }
        if (dst = mmap(nullptr, copysz, PROT_READ|PROT_WRITE, MAP_SHARED, fdout, fsz); dst == MAP_FAILED){
            err_sys("mamp error for output");
        }

        memcpy(dst, src, copysz);
        munmap(src, copysz);
        munmap(dst, copysz);
        fsz += copysz;
    }
    return 0;
}
