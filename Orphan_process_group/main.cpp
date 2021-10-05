#include <iostream>
#include "apue.h"

void sig_hup(int signo){
    std::cout<< "SIGHUP received , pid =" << getpid() << '\n';
}

void pr_ids(const std::string& name){
    printf("%s:pid=%d, ppid=%d, pgrp=%d, tpgrp=%d\n",
           name.c_str(), getpid(), getppid(), getpgrp(), tcgetpgrp(STDIN_FILENO));
    std::fflush(stdout);
}

int main() {
    char c;
    pid_t pid;
    pr_ids("parent");
    if (pid=fork(); pid < 0){
        err_sys("fork error");
    }
    else if (pid > 0){  /// parent
        sleep(5);
    }
    else{/// child
        pr_ids("child");
        signal(SIGHUP, sig_hup);
        kill(getpid(), SIGTSTP);    /// stop ourself
        pr_ids("child");
        if (read(STDIN_FILENO, &c, 1) != 1){
            std::cout << "read error " << errno << " on controlling TTY\n";
        }
    }
    return 0;
}
