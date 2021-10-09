#include <iostream>
#include "apue.h"
void* thr_fn1(void * arg){
    std::cout << "thread 1 returning\n";
    return (void*)1;
}

void* thr_fn2(void * arg){
    std::cout << "thread 2 returning\n";
    return (void*)2;
}
int main() {
    int err;
    pthread_t tid1, tid2;
    void *tret;

    err = pthread_create(&tid1, nullptr, thr_fn1, nullptr);
    if (err != 0){
        err_exit(err, "can't create thread 1");
    }
    err = pthread_create(&tid2, nullptr, thr_fn2, nullptr);
    if (err != 0){
        err_exit(err, "can't create thread 2");
    }

    err = pthread_join(tid1, &tret);
    if (err != 0){
        err_exit(err, "can't join with thread 1");
    }
    std::cout << "thread 1 exit code " << tret << "\n";
    err = pthread_join(tid2, &tret);
    if (err != 0){
        err_exit(err, "can't join with thread 2");
    }
    std::cout << "thread 2 exit code " << tret << "\n";
    return 0;
}
