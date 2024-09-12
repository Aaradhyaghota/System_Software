/*
========================================================================================================

Question No.: 21
Author: Aaradhya Ghota
Description: Write a program, call fork and print the parent and child process id.
Date: 29th August, 2024

========================================================================================================
*/
#include <stdio.h>
#include <unistd.h>

int main(){

	int child_pid = fork();
	if(child_pid !=0){
		printf("Process Id of parent process : %d \n", getpid() );
	}
	else {//this if condition will ensure that the child id is printed from parent process not from chld process
		printf("Process Id of child process : %d of parent process : %d\n",getpid(),getppid());
	}
	return 0;
}

/*
========================================================================================================
Output:

./a.out
Process Id of parent process : 19853 
Process Id of child process : 19854 of parent process : 19853

========================================================================================================
*/
