#include <iostream>
#include "apue.h"
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
void prepare(){
    int err;
    std::cout << "preparing locks...\n";
    if (err = pthread_mutex_lock(&lock1); err != 0){
        err_cont(err, "can't lock lock1 in prepare handler");
    }
    if (err = pthread_mutex_lock(&lock2); err != 0){
        err_cont(err, "can't lock lock2 in prepare handler");
    }
}

void parent(){
    int err;
    std::cout << "parent unlocking locks...\n";
    if (err = pthread_mutex_unlock(&lock1); err != 0){
        err_cont(err, "can't unlock lock1 in prepare handler");
    }
    if (err = pthread_mutex_unlock(&lock2); err != 0){
        err_cont(err, "can't unlock lock2 in prepare handler");
    }
}

void child(){
    int err;
    std::cout << "child unlocking locks...\n";
    if (err = pthread_mutex_unlock(&lock1); err != 0){
        err_cont(err, "can't unlock lock1 in child handler");
    }
    if (err = pthread_mutex_unlock(&lock2); err != 0){
        err_cont(err, "can't unlock lock2 in child handler");
    }
}

void *thr_fn(void *arg){
    std::cout << "thread started...\n";
    pause();
    return nullptr;
}

int main() {
    int err;
    pid_t pid;
    pthread_t tid;
    if (err = pthread_atfork(prepare, parent, child); err != 0){
        err_exit(err, "can't install fork handlers");
    }
    if (err = pthread_create(&tid, nullptr, thr_fn, nullptr); err != 0){
        err_exit(err, "can't create thread");
    }
    sleep(2);
    std::cout << "parent about to fork...\n";
    if (pid = fork(); pid < 0){
        err_quit("fork failed");
    }
    else if (pid == 0){///child
        std::cout << "child returned form fork\n";
    }
    else{///parent
        std::cout << "parent returned from fork\n";
    }
    return 0;
}
