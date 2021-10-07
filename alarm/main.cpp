#include <iostream>
#include <csignal>
#include <unistd.h>
#include <csetjmp>

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

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
