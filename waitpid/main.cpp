#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include "apue.h"
int main() {
    std::cout << "my pid is " << getpid() << '\n';
    pid_t pid;
    if ((pid = fork()) < 0){
        err_sys("fork error");
    }
    else if (pid == 0){ /// first child
        if ((pid = fork()) < 0){
            err_sys("fork error");
        }
        else if (pid > 0){
            exit(0);    ///parent from second fork == first child
        }
        /// the second child
        sleep(2);
        std::cout << "second child , parent pid = " << getppid() << '\n';
        exit(0);
    }
    if (waitpid(pid, nullptr, 0) != pid){// wait for first child
        err_sys("waitpid error");
    }
    /// we are parent.
    sleep(4);
    std::cout << "parent , child pid = " << pid << '\n';
    return 0;
}
