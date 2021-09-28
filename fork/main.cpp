#include <iostream>
#include <unistd.h>
#include "apue.h"

int globvar = 6;
char buf[] = "a write to stdout\n";

int main() {
    int var{88};
    pid_t pid;
    if (write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf)-1){
        err_sys("write error");
    }
    std::cout << "before fork\n";
    //std::cout << std::flush;
    // if don't flush, before fork will be written twice when redirect
    if ((pid = fork()) < 0){
        err_sys("fork error");
    }
    else if (pid == 0){///child process
        ++globvar;
        ++var;
    }
    else{
        sleep(2);
    }
    printf("pid=%ld, glob=%d, var=%d, child pid=%d\n", (long)getpid(), globvar, var, pid);
    return 0;
}
