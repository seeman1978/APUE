#include <iostream>
#include <sys/shm.h>
#include <sys/sem.h>
#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                                   } while (0)

#define MEM_SIZE 4096

int main() {
    int semid, shmid;
    union semun arg, dummy;
    struct sembuf sop;
    void *addr;
    /* Create shared memory and semaphore set containing one semaphore. */
    if (shmid = shmget(IPC_PRIVATE, MEM_SIZE, IPC_CREAT|0600); shmid == -1){
        errExit("shmget");
    }
    if(semid = semget(IPC_PRIVATE, 1, IPC_CREAT|0600); semid == -1){
        errExit("semget");
    }
    /* Attach shared memory into our address space. */
    addr = shmat(shmid, nullptr, SHM_RDONLY);
    if (addr == (void*) -1){
        errExit("shmat");
    }
    /* Initialize semaphore 0 in set with value 1. */
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1){
        errExit("semctl");
    }
    printf("shmid = %d; semid = %d\n", shmid, semid);
    /* Wait for semaphore value to become 0. */
    sop.sem_num = 0;
    sop.sem_op = 0;
    sop.sem_flg = 0;
    if (semop(semid, &sop, 1) == -1){
        errExit("semop");
    }
    /* Print the string from shared memory. */
    printf("%s\n", addr);
    /* Remove shared memory and semaphore set. */
    if (shmctl(shmid, IPC_RMID, nullptr) == -1){
        errExit("shmctl");
    }
    if (semctl(semid, 0, IPC_RMID, dummy) == -1){
        errExit("semctl");
    }
    return 0;
}
