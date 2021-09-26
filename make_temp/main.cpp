#include <iostream>
#include "apue.h"

void make_temp(char* temp){
    int fd;
    struct stat sbuf{};
    if ((fd = mkstemp(temp)) < 0){
        err_sys("can't create temp file");
    }
    std::cout << "temp name = " << temp << '\n';
    close(fd);
    if (stat(temp, &sbuf) < 0){
        if (errno == ENOENT){
            std::cout << "file doesn't exist\n";
        }
        else{
            err_sys("stat failed");
        }
    }
    else{
        std::cout << "file exists\n";
        unlink(temp);
    }
}
int main() {
    char good[] = "./dirXXXXXX";        /// allocate memory in stack. it is ok
    char const *bad = "./dirXXXXXX";          /// only point is in stack. wrong way
    std::cout << "trying to create first temp file ... \n";
    make_temp(good);
    std::cout << "trying to create second temp file ... \n";
    make_temp(const_cast<char*>(bad));
    return 0;
}
