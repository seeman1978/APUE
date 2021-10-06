#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include "apue.h"
void sig_cld(int signo){
    pid_t pid;
    int status;
    std::cout << "SIGCLD received\n";
    if (signal(SIGCLD, sig_cld) == SIG_ERR){
        perror("signal error");
    }
    if (pid = wait(&status); pid < 0){
        perror("wait error");
    }
    std::cout << "pid = " << pid << '\n';
}
int main() {
    pid_t pid;
    if (signal(SIGCLD, sig_cld) == SIG_ERR){
        perror("signal error");
    }
    if (pid = fork(); pid < 0){
        perror("fork error");
    }
    else if (pid == 0){ /// child
        sleep(2);
        _exit(0);
    }
    pause();    ///parent
    return 0;
}
