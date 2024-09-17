/*
========================================================================================================
Name : 7
Author : Aaradhya Ghota
Description : Write a simple program to print the created thread ids.
Date: 12 Sept, 2024.
========================================================================================================
*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>  

void print() {
    	printf("Thread %ld is running\n", pthread_self());
        sleep(1);  // wait so that we can see tread creation and deletion
        printf("Thread %ld has finished\n", pthread_self());
}

int main() {
    	pthread_t thread[3];
        int tdNum[3];
        int pstatus;

    	for (int i = 0; i < 3; ++i) {
        	pstatus = pthread_create(&thread[i], NULL, (void*)print, NULL);
        	if (pstatus != 0) {
            		printf("Error creating thread %d: %d\n", i, pstatus);
            		return 1;
        	}
    	}
    
        for (int i = 0; i < 3; ++i) {
              pthread_join(thread[i], NULL);
        }

        printf("All threads have finished\n");

    
        return 0;
}
/*
========================================================================================================
Output:
./a.out
Thread 140087341872832 is running
Thread 140087333480128 is running
Thread 140087325087424 is running
Thread 140087341872832 has finished
Thread 140087333480128 has finished
Thread 140087325087424 has finished
All threads have finished

========================================================================================================
*/
