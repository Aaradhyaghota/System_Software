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
		printf("Parent Process terminaed\n");
	}
	return 0;

}
