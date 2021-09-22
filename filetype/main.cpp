#include <iostream>
#include "apue.h"
#include "argv_range.h"
int main(int argc, char** argv) {
    struct stat buf{};
    std::string str;
    for (const auto param : argv_range(argc, argv)) {
        std::cout << param << ": ";
        if (lstat(param, &buf) < 0){
            err_ret("lstat error");
            continue;
        }
        if (S_ISREG(buf.st_mode)){
            str = "regular";
        }
        else if (S_ISDIR(buf.st_mode)){
            str = "directory";
        }
        else if (S_ISCHR(buf.st_mode)){
            str = "character special";
        }
        else if (S_ISBLK(buf.st_mode)){
            str = "block special";
        }
        else if (S_ISFIFO(buf.st_mode)){
            str = "fifo, named pipe";
        }
        else if (S_ISLNK(buf.st_mode)){
            str = "symbolic link";
        }
        else if (S_ISSOCK(buf.st_mode)){
            str = "socket";
        }
        else{
            str = "unknown mode";
        }
        std::cout << str << "\n";
    }
    exit(0);
}
