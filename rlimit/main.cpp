#include <iostream>
#include "apue.h"
#include <sys/resource.h>
#define doit(name) pr_limits(#name, name)

void pr_limits(const std::string& name, int resource){
    struct rlimit limit{};
    unsigned long long lim;
    if (getrlimit(resource, &limit) < 0){
        err_sys("getrlimit error for %s", name.c_str());
    }
    printf("%-14s ", name.c_str());
    if (limit.rlim_cur == RLIM_INFINITY){
        std::cout << "infinite ";
    }
    else{
        lim = limit.rlim_cur;
        printf("%10lld ", lim);
    }
    if (limit.rlim_max == RLIM_INFINITY){
        std::cout << "infinite ";
    }else{
        lim = limit.rlim_max;
        printf("%10lld ", lim);
    }
    putchar((int)'\n');
}

int main() {
#ifdef RLIMIT_AS
    doit(RLIMIT_AS);
#endif
    doit(RLIMIT_CORE);
    doit(RLIMIT_CPU);
    doit(RLIMIT_DATA);
    doit(RLIMIT_FSIZE);
    doit(RLIMIT_MEMLOCK);
    doit(RLIMIT_MSGQUEUE);
    doit(RLIMIT_NICE);
    doit(RLIMIT_NPROC);
#ifdef RLIMIT_NPTS
    doit(RLIMIT_NPTS);
#endif
    doit(RLIMIT_RSS);
#ifdef RLIMIT_SBSIZE
    doit(RLIMIT_SBSIZE);
#endif
    doit(RLIMIT_SIGPENDING);
    doit(RLIMIT_STACK);
#ifdef RLIMIT_SWAP
    doit(RLIMIT_SWAP);
#endif
#ifdef RLIMIT_VMEM
    doit(RLIMIT_VMEM);
#endif
    doit(RLIMIT_RTTIME);
    doit(RLIMIT_FSIZE);
    doit(RLIMIT_NOFILE);
    doit(RLIMIT_NPROC);
    doit(RLIMIT_LOCKS);
    return 0;
}
