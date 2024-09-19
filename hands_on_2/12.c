/*
========================================================================================================
Name : 12
Author : Aaradhya Ghota
Description : Write a program to create an orphan process. Use kill system call to send SIGKILL signal to the parent process from the child process.
Date: 13 Sept, 2024.
========================================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(){
	int pid = fork();
	if(pid < 0){
        	printf("Fork failed\n");
                exit(1);
        }
        if(pid){
		printf("Parent process PID:%d\n", getpid());
                sleep(10);
        }
        else{
                printf("Child process created PID: %d of parent process PID: %d\n", getpid(),getppid());
                int status=kill(getppid(),SIGKILL);
                if(status==0){
                        printf("Parent is killed Child is now orphen\n");   
                }
                else{
                        printf("Error while killing parent process!");
                }
		sleep(1);
                printf("Child process PID: %d is still running after parent was killed and now has parent ID : %d \n", getpid(),getppid());
      	}
    	return 0;
}


/*
========================================================================================================
Output:

./a.out
Parent process PID:5326
Child process created PID: 5327 of parent process PID: 5326
Parent is killed Child is now orphen
Killed
Child process PID: 5327 is still running after parent was killed and now has parent ID : 1378 
========================================================================================================
*/
