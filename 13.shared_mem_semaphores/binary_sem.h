// binary_sem.h
#ifndef BINARY_SEM_H
#define BINARY_SEM_H

#include <sys/sem.h>
#include "semun.h"

int binary_sem_initialize(int semid, int semnum) {
    union semun arg;
    arg.val = 1;
    return semctl(semid, semnum, SETVAL, arg) != -1;
}

int binary_sem_wait(int semid, int semnum) {
    struct sembuf sop;
    sop.sem_num = semnum;
    sop.sem_op = -1;
    sop.sem_flg = 0;
    return semop(semid, &sop, 1) != -1;
}

int binary_sem_post(int semid, int semnum) {
    struct sembuf sop;
    sop.sem_num = semnum;
    sop.sem_op = 1;
    sop.sem_flg = 0;
    return semop(semid, &sop, 1) != -1;
}

#endif
