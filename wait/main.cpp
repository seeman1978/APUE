#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include "apue.h"
void pr_exit(int status){
    if (WIFEXITED(status)){
        std::cout << "normal terminate, exit status= " << WEXITSTATUS(status) << '\n';
    }
    else if (WIFSIGNALED(status)){
        std::cout << "abnormal terminate, signal number = " << WTERMSIG(status);
        if (WCOREDUMP(status)){
            std::cout << " CORE FILE GENERATED \n";
        }
    }
    else if (WIFSTOPPED(status)){
        std::cout << "child stopped, signal nunmber = " << WSTOPSIG(status) << '\n';
    }
}

int main() {
    pid_t pid;
    int status;
    if ((pid = fork()) < 0){
        err_sys("fork error");
    }
    else if (pid == 0){ /// child
        exit(7);
    }

    if (wait(&status) != pid){
        err_sys("wait error");
    }
    pr_exit(status);

    if ((pid = fork()) < 0){
        err_sys("fork error");
    }
    else if (pid == 0){ /// child
        abort();
    }

    if (wait(&status) != pid){
        err_sys("wait error");
    }
    pr_exit(status);

    if ((pid = fork()) < 0){
        err_sys("fork error");
    }
    else if (pid == 0){ /// child
        status /= 0;
    }

    if (wait(&status) != pid){
        err_sys("wait error");
    }
    pr_exit(status);
    return 0;
}
