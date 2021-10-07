#include <iostream>
#include <csignal>
#include <unistd.h>
#include <csetjmp>
#include "apue.h"
void sig_alrm1(int signo){
    /// noting to do, just return to wake up the pause
}

unsigned int sleep1(unsigned int seconds){
    if (signal(SIGALRM, sig_alrm1) == SIG_ERR){
        return seconds;
    }
    alarm(seconds);
    pause();
    return alarm(0);/// turn off timer, return unslept time
}

static jmp_buf env_alrm;

void sig_alrm2(int signo){
    longjmp(env_alrm, 1);
}

unsigned int sleep2(unsigned int seconds){
    if (signal(SIGALRM, sig_alrm2) == SIG_ERR){
        return seconds;
    }
    if (setjmp(env_alrm) == 0){
        alarm(seconds);
        pause();
    }

    return alarm(0);/// turn off timer, return unslept time
}

void sig_int(int signo){
    volatile int k{0};
    std::cout << "\nsig_int starting\n";
    for (int i = 0; i < 300000; ++i) {
        for (int j = 0; j < 400000; ++j) {
            k += i*j;
        }
    }
    std::cout << "sig_int finished\n";
}

int main() {
    unsigned int unslept;
    if (signal(SIGINT, sig_int) == SIG_ERR){
        err_sys("signal (SIGINaT) error");
    }
    unslept = sleep2(5);
    std::cout << "sleep2 returned : " << unslept << '\n';
    return 0;
}
