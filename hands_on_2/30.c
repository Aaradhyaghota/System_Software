/*
========================================================================================================
Name : 30.a
Author : Aaradhya Ghota
Description : Write a program to create a shared memory.
		a. Write some data to the shared memory
		b. attach with O_RDONLY and check whether you are able to overwrite.
                c. detach the shared memory
                d. remove the shared memory
		
Date: 15 Sept, 2024.
========================================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

int main(){

        key_t key = ftok(".", 2); 
        int shmid = shmget(key, 1024 , 0666 | IPC_CREAT);
        if (shmid == -1) {
                perror("shmget");
                exit(1);
        }
        
        // attach shared memory to process adress space with read-write flag
        //third argument repesent 0 ->read-write flag
        char* data = (char *)shmat(shmid, (void *)0, 0);
        if (data == (char *)(-1)) {
                perror("shmat");
                exit(1);
        }

        printf("Writing data to shared memory...\n");
        strcpy(data, "Hello, Shared Memory!");

        printf("Data written to shared memory: %s\n", data);
        shmdt(data);

        //Attach  shared memory to process adress space with read-only flag
        data = (char *)shmat(shmid, NULL, SHM_RDONLY);
        if (data == (char *)(-1)) {
                perror("shmat");
                exit(1);
        }

        printf("Trying to overwrite in read-only mode...\n");
        //strncpy also can be used.
        //strcpy(data, "how are you, shared memory");//fail to write bcz is in read-mode.
        //this will give segmentation fault.
        
        printf("Data in shared memory after attempt: %s\n", data);

        if (shmdt(data) == -1) {
                perror("shmdt");
                exit(1);
        }

        if (shmctl(shmid, IPC_RMID, NULL) == -1) {
                perror("shmctl");
                exit(1);
        }

        printf("Shared memory removed successfully.\n");

        return 0;
}


/*
========================================================================================================
Output:

 ./a.out
Writing data to shared memory...
Data written to shared memory: Hello, Shared Memory!
Trying to overwrite in read-only mode...
Segmentation fault (core dumped)

//
./a.out
Writing data to shared memory...
Data written to shared memory: Hello, Shared Memory!
Trying to overwrite in read-only mode...
Data in shared memory after attempt: Hello, Shared Memory!
Shared memory removed successfully.



========================================================================================================
*/
