#include <iostream>
#include "apue.h"

pthread_t ntid;

void printids(const std::string& s){
    pid_t pid;
    pthread_t tid;
    pid = getpid();
    tid = pthread_self();
    printf("%s pid %lu tid %lu (0x%lx)\n", s.c_str(), (unsigned long)pid, (unsigned long)tid, (unsigned long)tid);
}

void* thr_fn(void *arg){
    printids("new thread: ");
    return nullptr;
}

int main() {
    int err;
    err = pthread_create(&ntid, nullptr, thr_fn, nullptr);
    if (err != 0){
        err_exit(err, "can't create thread");
    }
    std::cout << "sub thread id " << ntid << '\n';
    printids("main thread: ");
    sleep(1);
    return 0;
}
