#include <iostream>
#include <fcntl.h>
#include "apue.h"
int main(int argc, char** argv) {
    std::cout << "Hello, World!" << std::endl;
    if (argc != 2){
        err_quit("usage: a.out <pathname>");
    }
    if (access(argv[1], R_OK | W_OK) < 0){
        err_ret("access error for %s", argv[1]);
    }
    else{
        std::cout << "read access ok\n";
    }
    if (open(argv[1], O_RDONLY) < 0){
        err_ret("open error for %s", argv[1]);
    }
    else{
        std::cout << "open for reading OK\n";
    }
    return 0;

}
