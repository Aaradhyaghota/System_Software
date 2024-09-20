
/*
========================================================================================================
Name : 32.a
Author : Aaradhya GHota
Description : Write a program to implement semaphore to protect any critical section.
		a. Rewrite the ticket number creation program using semaphore.
Date: 15 Sept, 2024.
========================================================================================================
*/



#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>

typedef union semp_un{
	int val;
}semun;

int main(){
	semun arg;
	
	key_t key = ftok(".", 123);
	int semid = semget(key, 1, 0);// if semaphore already exists//return -1 when it not exist
	if (semid == -1) {
		semid = semget(key, 1, IPC_CREAT | 0700);
		arg.val = 1;
		semctl(semid, 0, SETVAL, arg);
	}

	struct sembuf op;
	op.sem_num = 0;
	op.sem_flg = 0;

	printf("Entering critical zone......\n");
	//locking
	op.sem_op = -1;
	semop(semid, &op, 1);

	printf("---Inside Critical section---\n");
	printf("Press Enter to unlock\n");
	getchar();
	
	int data; 
	int fd = open("./32a.txt", O_RDWR | O_CREAT, S_IRWXU);
	if (fd == -1) {
		perror("Error while opening file!");
		return 0;
	}
	
	int n = read(fd, &data, sizeof(data));
	if (n == 0) data = 1;
	else{
		data += 1;
		lseek(fd, 0, SEEK_SET);
	}
	
	write(fd, &data, sizeof(data));
	printf("ticket number is : %d\t \n", data);
	printf("ticket number is stored in file\n");

	//unlocking
	op.sem_op = 1;
	semop(semid, &op, 1);
	printf("Out of critical zone\n");
	return 0;
}


/*ipcs to get all the inter process communication
to delte the semaphore $ipcrm -s 3 (3 is idof semaphore)*/

/*
========================================================================================================
Output:

Terminal1:
./a.out
Entering critical zone......
---Inside Critical section---
Press Enter to unlock

Terminal2:
./a.out
Entering critical zone......

//-------------------After removing the lock

Terminal1:
./a.out
Entering critical zone......
---Inside Critical section---
Press Enter to unlock

ticket number is : 3	 
ticket number is stored in file
Out of critical zone

Terminal2:
./a.out
Entering critical zone......
---Inside Critical section---
Press Enter to unlock



========================================================================================================
*/
