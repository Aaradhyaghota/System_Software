/*
========================================================================================================
Name : 6
Author : Aaradhya Ghota
Description : Write a simple program to create three threads.
Date: 12 Sept, 2024.
========================================================================================================
*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>  

void print(void* arg) {
    	int td_num = *((int*)arg);
    	printf("Thread %d is running\n", td_num);
        sleep(1);  // wait so that we can see 
        printf("Thread %d has finished\n", td_num);
}

int main() {
    	pthread_t thread[3];
        int tdNum[3];
        int pstatus;

    	for (int i = 0; i < 3; ++i) {
        	tdNum[i] = i;  // Assign a unique no to each thread
        	pstatus = pthread_create(&thread[i], NULL, (void*)print, (void*)&tdNum[i]);
        	if (pstatus != 0) {
            		printf("Error creating thread %d: %d\n", i, pstatus);
            		return 1;
        	}
    	}
    
        //pthread_exit(NULL);//this will wait for all the tread to fininsh
        //and then end this parent treadhere only after is not code written below will execute.
    
        // Wait for all threads to finish
        for (int i = 0; i < 3; ++i) {
              pthread_join(thread[i], NULL);
        }

        printf("All threads have finished\n");

    
        return 0;
}
/*
========================================================================================================
output:

./a.out
Thread 0 is running
Thread 1 is running
Thread 2 is running
Thread 0 has finished
Thread 1 has finished
Thread 2 has finished
All threads have finished
========================================================================================================
*/
