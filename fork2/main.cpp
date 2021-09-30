#include <iostream>
#include <unistd.h>
#include "apue.h"
int main() {
    pid_t pid;
    if (pid = fork(); pid < 0){
        err_sys("fork error");
    }
    else if (pid != 0) {/// parent
        sleep(2);
        exit(2);
    }

    if (pid = fork(); pid < 0){
        err_sys("fork error");
    }
    else if (pid != 0) {/// parent
        sleep(4);
        abort();
    }

    if (pid = fork(); pid < 0){
        err_sys("fork error");
    }
    else if (pid != 0) {/// parent
        execl("/bin/dd", "dd", "if=/etc/passwd", "of=/dev/null", nullptr);
        exit(7);
    }

    if (pid = fork(); pid < 0){
        err_sys("fork error");
    }
    else if (pid != 0) {/// parent
        sleep(8);
        exit(0);
    }

    sleep(6);   /// fourth child
    kill(getpid(), SIGKILL);    /// terminage signal, no core dump
    return 6;
}
