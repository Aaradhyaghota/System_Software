/*
========================================================================================================
Name : 31
Author : Aaradhya Ghota
Description : Write a program to create a semaphore and initialize value to the semaphore.
		a. Create a binary semaphore.
		b. create a counting semaphore
Date: 15 Sept, 2024.
========================================================================================================
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>

typedef union {
	int val;
}semun;

int main() {
        
        //Binary semaphore
        semun arg;
	key_t key = ftok(".", 3);
	//int semget(key_t key, int nsems, int semflg);
	int semid = semget(key, 1, IPC_CREAT | 0744);//create or access a semaphore
	arg.val = 1;
	//int semctl(int semid, int semnum(indeex), int cmd, ...);
	semctl(semid, 0, SETVAL, arg);//initialization
	printf("Binary semaphore created\n");
        
        //counting semaphore
        semun args;
	key = ftok(".", 4);
	semid = semget(key, 1, IPC_CREAT | 0744);
	args.val = 10;
	semctl(semid, 0, SETVAL, args);
        printf("Counting semaphore created size is : %d \n", args.val);
	return 0;
}


/*
========================================================================================================
Output:

./a.out
Binary semaphore created
Counting semaphore created size is : 10 

========================================================================================================
*/

