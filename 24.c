/*
========================================================================================================

Question No.: 24
Author: Aaradhya Ghota
Description: Write a program to create an orphan process.
Date: 29th August, 2024

========================================================================================================
*/
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
	int child_pid = fork();
	if(child_pid ==0){
		printf("CHild Process : %d before being orphan has parent id: %d \n",getpid(),getppid());
		sleep(20);
		printf("Child Process : %d after being orphan has parent id: %d \n", getpid(),getppid() );

	}
	else {
		printf("Parent Process terminated\n");
	}
	return 0;

}

/*
========================================================================================================
Output:

./a.out
Parent Process terminated
CHild Process : 20478 before being orphan has parent id: 20477 

ater 20sec

Child Process : 20478 after being orphan has parent id: 1381

========================================================================================================
*/
