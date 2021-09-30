#include <iostream>
#include <sys/time.h>
#include "apue.h"
unsigned long long count;
struct timeval end;

void checktime(const std::string& str){
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    if (tv.tv_sec >= end.tv_sec && tv.tv_usec >= end.tv_usec){
        printf("%s count = %lld\n", str.c_str(), count);
        exit(0);
    }
}

int main(int argc, char** argv) {
    pid_t pid;
    std::string s;
    int nzero, ret, adj=0;
    setbuf(stdout, nullptr);
#if defined(NZERO)
    nzero = NZERO;
#elif defined(_SC_NZERO)
    nzero = sysconf(_SC_NZERO);
#else
#error NZERO undefined
#endif
    printf("NZERO=%d\n", nzero);
    if (argc == 2){
        adj = strtol(argv[1], nullptr, 10);
    }
    gettimeofday(&end, nullptr);
    end.tv_sec += 10;   ///run for 10 seconds

    if (pid=fork(); pid < 0){
        err_sys("fork failed");
    }
    else if (pid == 0){///child
        s = "child";
        printf("current nice value in child is %d, adjusting by %d\n",
               nice(0)+nzero, adj);
        errno = 0;
        if ((ret = nice(adj)) == -1 && errno != 0){
            err_sys("child set scheduling priority");
        }
        printf("now child nice value is %d\n", ret + nzero);
    }
    else{
        s = "parent";
        printf("current nice value in parent is %d\n", nice(0)+nzero);
    }
    for(;;) {
        if (++count == 0){
            err_quit("%s counter wrap", s.c_str());
        }
        checktime(s);
    }
    return 0;
}
