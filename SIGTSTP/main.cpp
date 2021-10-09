#include <iostream>
#include <csignal>
#include <unistd.h>
#include "apue.h"

constexpr auto BUFFSIZE = 1024;

void sig_tstp(int signo){
    sigset_t mask;
    /// unblock SIGTSTP , since it's blocked while we're handling it.
    sigemptyset(&mask);
    sigaddset(&mask, SIGTSTP);
    sigprocmask(SIG_UNBLOCK, &mask, nullptr);
    signal(SIGTSTP, SIG_DFL);///RESET disposition to default
    kill(getpid(), SIGTSTP);
    /// WE don't return from the kill until we are continued.
    signal(SIGTSTP, sig_tstp);  ///reestablish signal handler
    ///reset tty mode, redraw screenkkk
}
int main() {
    ssize_t n;
    char buf[BUFFSIZE];
    /// only catch SIGTSTP IF we're running with a job-control shell
    if (signal(SIGTSTP, SIG_IGN) == SIG_DFL){
        signal(SIGTSTP, sig_tstp);
    }
    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0){
        if (write(STDOUT_FILENO, buf, n) != n){
            err_sys("write error");
        }
    }

    if (n < 0){
        err_sys("read error");
    }
    return 0;
}
