#include <iostream>
#include <sys/wait.h>
#include "apue.h"

constexpr auto DEF_PAGER = "more";

int main(int argc, char** argv) {
    size_t n;
    int fd[2];
    pid_t pid;
    const char *pager, *argv0;
    char line[MAXLINE];
    FILE *fp;

    if (argc != 2){
        err_quit("usage: a.out <pathname>");
    }
    if (fp = fopen(argv[1], "r"); fp == nullptr){
        err_sys("can't open %s", argv[1]);
    }
    if (pipe(fd) < 0){
        err_sys("pipe error");
    }

    if (pid = fork(); pid < 0){
        err_sys("fork error");
    }
    else if (pid > 0){  ///parent
        close(fd[0]);   ///close read end
        ///parent copies argv[1] to pipe
        while(fgets(line, MAXLINE, fp) != nullptr){
            n = strlen(line);
            if (write(fd[1], line, n) != n){
                err_sys("write error to pipe");
            }
        }
        if (ferror(fp)){
            err_sys("fgets error");
        }
        close(fd[1]);   ///close write end of pipe for reader
        if (waitpid(pid, nullptr, 0) < 0){
            err_sys("waitpid error");
        }
        exit(0);
    }
    else{   ///child
        close(fd[1]);   ///close write end
        if (fd[0] != STDIN_FILENO){
            if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO){
                err_sys("dup2 error to stdin");
            }
            close(fd[0]);   ///don't need this after dup2
        }
        /// get auguments for execl()
        if (pager = getenv("PAGER"); pager == nullptr){
            pager = DEF_PAGER;
        }
        if (argv0 = strrchr(pager, '/'); argv0 != nullptr){
            argv0++;    ///step past rightmost slash
        }
        else{
            argv0 = pager;
        }
        if (execl(pager, argv0, (char*) nullptr) < 0){
            err_sys("execl error for %s", pager);
        }
    }

    return 0;
}
