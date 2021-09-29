#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include "apue.h"
char* env_init[] {"USER=unknown", "PATH=/tmp", nullptr};
int main() {
    pid_t pid;
    if (pid=fork(); pid < 0){
        err_sys("fork error");
    }
    else if (pid == 0){/// child.specify pathname , specify environment
        if (execle("/home/sar/bin/echoall", "echoall", "myarg1",
                   "MY ARG2", (char*) nullptr, env_init) < 0){
            err_sys("execle error");
        }
    }
    if (waitpid(pid, nullptr, 0) < 0){
        err_sys("wait error");
    }
    if (pid=fork();pid < 0){
        err_sys("fork error");
    }
    else if(pid == 0){
        if (execlp("echoall", "echoall", "only 1 arg", (char*) nullptr) < 0){
            err_sys("execlp error");
        }
    }
    return 0;
}
