/*
========================================================================================================
Name : 32.b
Author : Aaradhya GHota
Description : Write a program to implement semaphore to protect any critical section.
		b. Protect shared memory from concurrent write access.
Date: 15 Sept, 2024.
========================================================================================================
*/

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>

typedef union semun{
        int val;               
}semun;

int main(){
	key_t skey = ftok(".", 20);
	int sid = shmget(skey, 1024, IPC_CREAT | 0744);
	
	char *sptr = shmat(sid, 0, 0);

	semun semset;
	key_t semkey = ftok(".", 332);
	int semid = semget(semkey, 1, 0);
	if (semid == -1) {
		semid = semget(semkey, 1, IPC_CREAT | 0700);
		semset.val = 1;
		if (semctl(semid, 0, SETVAL, semset) == -1){
		          perror("error while initializing the semaphore");
	                  exit(1);
		} 
	}

	struct sembuf sop;
	sop.sem_num = 0;
	sop.sem_flg = 0;

	printf("Entering to obtain lock.....\n");
	sop.sem_op = -1;
	if (semop(semid, &sop, 1) == -1) {
	        perror("error while operating the semaphore");
	        exit(1);
	}
	printf("Obtained lock!\nEntering critical section\n");
	
	//Critical Section
	printf("Write in shared memory\n");
	scanf("%[^\n]", sptr);
	printf("Data from shared memory: %s\n", sptr);

	printf("Exiting critical section\n");
	//Ending critical section
	
	
	//unlocking
	sop.sem_op = 1;
	if (semop(semid, &sop, 1) == -1){
	          perror("Error while operating on semaphore!");
	          exit(1);
        }
	return 0;
}


/*
========================================================================================================
Output:

Terminal1:
./a.out
Entering to obtain lock.....
Obtained lock!
Entering critical section
write in shared memory


Terminal2:
./a.out
Entering to obtain lock.....


//After entering something into shared mamerory

Terminal1:
./a.out
Entering to obtain lock.....
Obtained lock!
Entering critical section
write in shared memory
Hello
Data from shared memory: Hello
Exiting critical section


Terminal2:
./a.out
Entering to obtain lock.....
Obtained lock!
Entering critical section
write in shared memory


========================================================================================================
*/
