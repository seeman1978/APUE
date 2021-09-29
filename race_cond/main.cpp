#include <iostream>
#include <unistd.h>
#include "apue.h"

void charatatime(const std::string& str){
    setbuf(stdout, nullptr);/// set unbuffered
    for (const auto c : str) {
        putc(c, stdout);
    }
}

int main() {
    pid_t pid;
    if ((pid = fork()) < 0){
        err_sys("fork error");
    }
    else if (pid == 0){///parent
        charatatime("out put from child\n");
    }
    else {
        charatatime("out put from parent\n");
    }
    return 0;
}
