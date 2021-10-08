#include <iostream>
#include <csetjmp>
#include "apue.h"

static sigjmp_buf jmpBuf;
static volatile sig_atomic_t canjump;

void sig_usr1(int signo){
    time_t starttime;
    if (canjump == 0){
        return; /// unexpected signal, ignore
    }
    pr_mask("starting sig_usr1: ");
    alarm(3);
    starttime = time(nullptr);
    for (;;){
        if (time(nullptr) > starttime + 5){
            break;
        }
    }
    pr_mask("finishing sig_usr1: ");
    canjump = 0;
    siglongjmp(jmpBuf, 1);  /// jump back to main, don't return
}

void sig_alrm(int signo){
    pr_mask("in sig_alrm: ");
}

int main() {
    if (signal(SIGUSR1, sig_usr1) == SIG_ERR){
        err_sys("signal SIGUSR1 ERROR");
    }
    if (signal(SIGALRM, sig_alrm) == SIG_ERR){
        err_sys("signal SIGALRM error");
    }
    pr_mask("starting main: ");
    if (sigsetjmp(jmpBuf, 1)){
        pr_mask("ending main: ");
        exit(0);
    }
    canjump = 1; /// now sigsetjmp is ok
    for (;;){
        pause();
    }
    return 0;
}
