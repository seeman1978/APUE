#include <iostream>
#include "apue.h"
#include <sys/shm.h>
constexpr auto ARRAY_SIZE = 40000;
constexpr auto MALLOC_SIZE = 100000;
constexpr auto SHM_SIZE = 100000;
#define SHM_MODE 0600
char array[ARRAY_SIZE];
int main() {
    int shmid;
    void *ptr, *shmptr;
    printf("array [] from %p to %p \n", (void *)&array[0], (void*)&array[ARRAY_SIZE]);
    printf("stack arount %p\n", (void*)&shmid);
    if (ptr = malloc(MALLOC_SIZE); ptr == nullptr){
        err_sys("malloc error");
    }
    printf("malloced from %p to %p\n", (void*)ptr, ptr+MALLOC_SIZE);
    if (shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE); shmid < 0){
        err_sys("shmget error");
    }
    if (shmptr = shmat(shmid, nullptr, nullptr); shmptr == (void*)-1){
        err_sys("shmat error");
    }
    printf("shared memory attached from %p to %p\n", (void*)shmptr, (void*)shmptr+SHM_SIZE);
    if (shmctl(shmid, IPC_RMID, 0) < 0){
        err_sys("shmctl error");
    }
    return 0;
}
