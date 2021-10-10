#include <iostream>
#include <pthread.h>
int main() {
    int err;
    struct timespec tout;
    struct tm* tmp;
    char buf[64];
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&lock);
    std::cout << "mutex is locked\n";
    clock_gettime(CLOCK_REALTIME, &tout);
    tmp = localtime(&tout.tv_sec);
    strftime(buf, sizeof buf, "%r", tmp);
    std::cout << "current time is " << buf << '\n';
    tout.tv_sec += 10;
    /// caution: this could lead to deadlock
    err = pthread_mutex_timedlock(&lock, &tout);
    clock_gettime(CLOCK_REALTIME, &tout);
    tmp = localtime(&tout.tv_sec);
    strftime(buf, sizeof buf, "%r", tmp);
    std::cout << "the time is now " << buf << '\n';
    if (err == 0){
        std::cout << "mutex locked again!\n";
    }
    else{
        std::cout << "can't lock mutex again:" << strerror(err) << '\n';
    }
    return 0;
}
