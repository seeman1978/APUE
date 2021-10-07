#include <csetjmp>
#include "apue.h"
static jmp_buf env_alrm;

void sig_alrm(int signo){
    longjmp(env_alrm, 1);
}

int main() {
    ssize_t n;
    char line[MAXLINE];
    if (signal(SIGALRM, sig_alrm) == SIG_ERR){
        err_sys("signal (SIGALRM) error");
    }
    if (setjmp(env_alrm) != 0){
        err_quit("read timeout");
    }
    alarm(10);
    if (n = read(STDIN_FILENO, line, MAXLINE); n < 0){
        err_sys("read error");
    }
    alarm(0);
    write(STDOUT_FILENO, line, n);
    return 0;
}
