#include<stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv){
	if(argc !=2 ){
		printf("Error - INvalid Input");
		return 0;
	}

	int fd = open(argv[1], O_CREAT | O_APPEND | O_RDWR, 0644);
	
	int child_pid = fork();
	if(child_pid != 0){
		int wsz = write(fd, "Written From parent process\n",29);
		if(wsz == -1) {
	       		printf("Writting failed from parent process\n");
			return 0;
		}
	}
	else{
		int wsz2 = write(fd, "Written from child process\n",28);
		if(wsz2 == -1) { 
        	        printf("Writting failed from child process\n");
                	return 0;
	        }
	}

}
