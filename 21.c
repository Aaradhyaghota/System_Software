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
