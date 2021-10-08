#include <iostream>
#include <csignal>
#include "apue.h"
void sig_quit(int signo){
    std::cout << "caught SIGQUIT\n";
    if (signal(SIGQUIT, SIG_DFL) == SIG_ERR){
        err_sys("can't reset SIGQUIT");
    }
}

int main() {
    sigset_t newmask, oldmask, pendmask;
    if (signal(SIGQUIT, sig_quit) == SIG_ERR){
        err_sys("can't catch SIGQUIT");
    }
    /// block SIGQUIT AND save current signal mask
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGQUIT);
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0){
        err_sys("SIG_BLOCK error");
    }
    sleep(5);   /// SIGQUIT here will remain pending
    if (sigpending(&pendmask) < 0){
        err_sys("sigpending error");
    }
    if (sigismember(&pendmask, SIGQUIT)){
        std::cout << "\nSIGQUIT pending\n";
    }

    ///restore signal mask which unblocks SIGQUIT
    if (sigprocmask(SIG_SETMASK, &oldmask, nullptr) < 0){
        err_sys("SIG_SETMASK error");
    }
    std::cout << "SIG_SETMASK SIGQUIT unblocked\n";
    sleep(5); ///sigquit here will terminate with core file
    return 0;
}
