#include <fcntl.h>
#include "apue.h"
int main(int argc, char** argv) {
    int fd;
    struct stat statbuf{};
    struct timespec times[2];
    for (int i = 0; i < argc; ++i) {
        if (stat(argv[i], &statbuf)<0){
            ///fetch current time
            err_ret("%s:stat error", argv[i]);
            continue;
        }
        if ((fd = open(argv[i], O_RDWR | O_TRUNC)) < 0){
            err_ret("%s : open error", argv[i]);
            continue;
        }
        times[0] = statbuf.st_atim;     /* Time of last access */
        times[1] = statbuf.st_mtim;     /* Time of last modification */
        if (futimens(fd, times) < 0){
            err_ret("%s:futimens error", argv[i]);
        }
        close(fd);
    }
    return 0;
}
