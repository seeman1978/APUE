#include <iostream>
#include "apue.h"
int main() {
    ssize_t n;
    int fd[2];
    pid_t pid;
    char line[MAXLINE];
    if (pipe(fd) < 0){
        err_sys("pipe error");
    }
    if (pid = fork(); pid < 0){
        err_sys("fork error");
    }
    else if (pid > 0){///parent
        close(fd[0]);/// close read endpoint of pipe
        write(fd[1], "hello world\n", 12);
    }
    else{///child
        close(fd[1]);   ///close write endpoint of pipe
        n = read(fd[0], line, MAXLINE);
        write(STDOUT_FILENO, line, n);
    }
    return 0;
}
