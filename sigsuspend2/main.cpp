#include <iostream>
#include "apue.h"

volatile sig_atomic_t quitflag;

void sig_int(int signo){
    if (signo == SIGINT){
        std::cout << "\ninterrupt\n";
    }
    else if (signo == SIGQUIT){
        quitflag = 1;   /// set flag for main loop
    }
}

int main() {
    sigset_t newmask, oldmask, zeromask;
    if (signal(SIGINT, sig_int) == SIG_ERR){
        err_sys("signal(SIGINT) error");
    }
    if (signal(SIGQUIT, sig_int) == SIG_ERR){
        err_sys("signal(SIGQUIT) error");
    }
    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGQUIT);

    ///Block SIGQUIT AND SAVE CURRENT SIGNAL MASK
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0){
        err_sys("SIG_BLOCK ERROR");
    }
    while (quitflag == 0){
        sigsuspend(&zeromask);
    }
    ///SIGQUIT has been caught and is now blocked ; do whatever
    quitflag = 0;
    ///reset signal mask which unblocks SIGQUIT
    if (sigprocmask(SIG_SETMASK, &oldmask, nullptr) < 0){
        err_sys("SIG_SETMASK ERROR");
    }
    return 0;
}
