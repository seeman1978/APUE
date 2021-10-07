#include "apue.h"

void sig_alrm(int signo){
    /// nothing to do, just return to interrupt the read
}

int main() {
    ssize_t n;
    char line[MAXLINE];
    if (signal(SIGALRM, sig_alrm) == SIG_ERR){
        err_sys("signal(SIGALRM) error");
    }
    alarm(10);
    if (n = read(STDIN_FILENO, line, MAXLINE); n < 0){
        err_sys("read error");
    }
    alarm(0);
    write(STDOUT_FILENO, line , n);
    return 0;
}
