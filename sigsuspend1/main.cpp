#include <iostream>
#include "apue.h"

void sig_int(int signo){
    pr_mask("\nin sig_int: ");
}

int main() {
    sigset_t newmask, oldmask, waitmask;
    pr_mask("program start: ");
    if (signal(SIGINT, sig_int) == SIG_ERR){
        err_sys("signal (SIGINT) ERROR");
    }
    sigemptyset(&waitmask);
    sigaddset(&waitmask, SIGUSR1);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGINT);

    ///BLOCK SIGINT and save current signal mask
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0){
        err_sys("SIG_BLOCK ERROR");
    }
    /// critical region of code.
    pr_mask("in critical region: ");
    /// pause , allowing all signals except SIGUSR1
    if (sigsuspend(&waitmask) != -1){
        err_sys("sigsuspend error");
    }
    pr_mask("after return from sigsuspend: ");
    ///reset signal mask which unblocks SIGINT
    if (sigprocmask(SIG_SETMASK, &oldmask, nullptr) < 0){
        err_sys("SIG_SETMASK ERROR");
    }
    /// and continue processing
    pr_mask("program exit: ");
    return 0;
}
