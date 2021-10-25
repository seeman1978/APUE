#include <iostream>
#include <sys/shm.h>
#include <sys/sem.h>
#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                                   } while (0)

#define MEM_SIZE 4096

int main(int argc, char* argv[]) {
    int semid, shmid;
    struct sembuf sop;
    char *addr;
    size_t len;
    if (argc != 4){
        fprintf(stderr, "usage : %s shmid semid string\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    len = strlen(argv[3]) + 1;
    if (len > MEM_SIZE){
        fprintf(stderr, "String is too big!\n");
        exit(EXIT_FAILURE);
    }
    /* Get object IDs from command-line. */
    shmid = atoi(argv[1]);
    semid = atoi(argv[2]);
    /* Attach shared memory into our address space and copy string
                  (including trailing null byte) into memory. */
    addr = static_cast<char *>(shmat(shmid, nullptr, 0));
    if (addr == (void*)-1){
        errExit("shmat");
    }
    memcpy(addr, argv[3], len);
    /* Decrement semaphore to 0. */
    sop.sem_num = 0;
    sop.sem_op = -1;
    sop.sem_flg = 0;
    if (semop(semid, &sop, 1) == -1){
        errExit("semop");
    }
    exit(EXIT_SUCCESS);
}
