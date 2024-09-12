/*
========================================================================================================

Question No.: 23
Author: Aaradhya Ghota
Description: Write a program to create a Zombie state of the running program.
Date: 29th August, 2024

========================================================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(){
	//A Zombie process is a process that has completed execution but still has 
	//an entry in the process table. This happens when the parent process hasn't 
	//yet read the exit status of the child process using the wait() system call.
 	int child_pid = fork();
	if(child_pid == 0 ){
		printf("Child Process pid = %d executed and exited\n",getpid() );
		exit(0);
	}	
	else {
		printf("Parent process pid = %d called a child process = %d \n", getpid(), child_pid);
		printf("use this command in another terminal to see zombie process - ps aux | grep Z\n");
                
                sleep(4);
		printf("press the enter to terminate the parent process\n");
		getchar();
		printf("Parent process terminated\n");
		// Parent process can now call wait() to clean up the zombie

	}	
}

/*
========================================================================================================
Output:

terminal1:
./a.out
Parent process pid = 20228 called a child process = 20229 
use this command in another terminal to see zombie process - ps aux | grep Z
Child Process pid = 20229 executed and exited
press the enter to terminate the parent process


terminal 2:
ps aux | grep Z-->

USER         PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
aaradhy+    4164  0.0  0.0      0     0 ?        Z    10:14   0:00 [sd_espeak-ng-mb] <defunct>
aaradhy+   20229  0.0  0.0      0     0 pts/0    Z+   20:50   0:00 [a.out] <defunct>

terminal 1:
Pressed Enter...
Parent process terminated


========================================================================================================
*/
