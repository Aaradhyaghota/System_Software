
/*
========================================================================================================
Name : 32.d
Author : Aaradhya Ghota
Description : Write a program to implement semaphore to protect any critical section.
		d. Remove the created semaphore.
Date: 15 Sept, 2024.
========================================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/ipc.h>

typedef union semp_un{
    int val;
}senum;


int main(){
    senum arg;
    key_t key = ftok(".", 3);
    int semid = semget(key, 1, 0);
    if (semid == -1) {
	    semid = semget(key, 1, IPC_CREAT | 0700);
	    arg.val = 1;
	    semctl(semid, 0, SETVAL, arg);
    }
    
    semctl(semid, 0, IPC_RMID, arg);
    printf("Semaphore deleted successfully\n");
    return 0;
}

/*
========================================================================================================
Output:

./a.out
Semaphore deleted successfully

========================================================================================================
*/

