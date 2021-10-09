#include <iostream>
#include "apue.h"

void sig_int(int signo){
    std::cout << "caught SIGINT\n";
}

void sig_chld(int signo){
    std::cout << "caught SIGCHLD\n";
}

int main() {
    if (signal(SIGINT, sig_int) == SIG_ERR){
        err_sys("signal(SIGINT) ERROR");
    }
    if (signal(SIGCHLD, sig_chld) == SIG_ERR){
        err_sys("signal(SIGCHLD) ERROR");
    }
    if (system("/usr/bin/ed") < 0){
        err_sys("system error");
    }
    return 0;
}
