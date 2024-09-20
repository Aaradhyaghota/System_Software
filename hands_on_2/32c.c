/*
========================================================================================================
Name : 32.c
Author : Aaradhya GHota
Description : Write a program to implement semaphore to protect any critical section.
		c. Protect multiple pseudo resources (may be two) using counting semaphore.
Date: 15 Sept, 2024.
========================================================================================================
*/

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

typedef union semp_un{
        int val;   
}semun;
        
int main(){
        semun arg;
        
        key_t Key = ftok(".", 333);
        if (Key == -1){
            perror("Error while computing key!");
            exit(1);
        }

        int semid = semget(Key, 1, 0);
        if (semid == -1){
            semid = semget(Key, 1, IPC_CREAT | 0700); 
            if (semid == -1){
                perror("Error while creating semaphore!");
                exit(1);
            }

            arg.val = 2; 
            
            if (semctl(semid , 0, SETVAL, arg) == -1){
                perror("Error while initializing a binary sempahore!");
                exit(1);
            }
        }

        struct sembuf op; 
        op.sem_num = 0;
        op.sem_flg = 0;

        printf("Entering to obtain lock on the critical section......\n");

        // locking
        op.sem_op = -1;
        if (semop(semid, &op, 1) == -1){
            perror("Error while operating on semaphore!");
            exit(1);
        }

        printf("Obtained lock on critical section!\n");
        // Critical Section 

        printf("Inside Critical section\n");
        printf("Press Enter to unlock\n");
        getchar();
        int data;
        int fd = open("./32c.txt", O_RDWR | O_CREAT, S_IRWXU);
        if (fd == -1){
            perror("Error while opening file!");
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

        //end of critical seciton

        // unlocking
        op.sem_op = 1;
        if(semop(semid, &op, 1) == -1){
            perror("Error while operating on semaphore!");
            exit(1);
        }
        printf("Out of critical zone\n");
	return 0;
}

/*
========================================================================================================
Output:

Terminal 1:
./a.out
Entering to obtain lock on the critical section......
Obtained lock on critical section!
Inside Critical section
Press Enter to unlock

Terminal 2:
./a.out
Entering to obtain lock on the critical section......
Obtained lock on critical section!
Inside Critical section
Press Enter to unlock


Terminal 3:
./a.out
Entering to obtain lock on the critical section......




========================================================================================================
*/

