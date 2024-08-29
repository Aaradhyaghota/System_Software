#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
	//A Zombie process is a process that has completed execution but still has 
	//an entry in the process table. This happens when the parent process hasn't 
	//yet read the exit status of the child process using the wait() system call.
 	int child_pid = fork();
	if(child_pid == 0 )
		printf("Child Process pid = %d executed\n",getpid() );
	else {
		printf("Parent process pid = %d called a child process = %d \n", getpid(), child_pid);
		printf("use this command in another terminal to see zombie process - ps aux | grep Z\n");

		printf("press the enter to terminate the parent process\n");
		getchar();
		printf("Parent process terminated"\n);
		// Parent process can now call wait() to clean up the zombie

	}	
}

