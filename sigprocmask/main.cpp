#include <iostream>
#include <csignal>
#include "apue.h"
void pr_mask(const char* str){
    sigset_t sigset;
    int errno_save;
    errno_save = errno;
    if (sigprocmask(0, nullptr, &sigset) < 0){
        err_ret("sigprocmask error");
    }
    else {
        std::cout << str;

        if (sigismember(&sigset, SIGINT)) {
            std::cout << "SIGINT";
        }
        if (sigismember(&sigset, SIGQUIT)) {
            std::cout << "SIGQUIT";
        }
        if (sigismember(&sigset, SIGUSR1)) {
            std::cout << "SIGUSR1";
        }
        if (sigismember(&sigset, SIGALRM)) {
            std::cout << "SIGALRM";
        }
        std::cout << '\n';
    }
    errno = errno_save; ///restore errno
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
