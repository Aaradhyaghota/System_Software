#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
	int i , j, k;
    	if(!(i=fork()) ){
		printf("Child-1 called\n");
      		sleep(3);
		printf("Child-1 terminated and exit\n");
	}
    	else{
          	if(!(j=fork()) ){
                	printf("Child-2 called\n");
                	sleep(15);
                	printf("Child-2 terminated and exit\n");
        	}
		else{ 
                	if(!(k=fork()) ){
                		printf("Child-3 called\n");
               	 		sleep(6);
               			printf("Child-3 terminated and exit\n");
       			}      		
                	else{ 
				printf("parent Processs waiting for child -2 to terminate\n");
                      		waitpid(j,NULL , 0);
				printf("Parent process terminated and exit\n"); 
			}	
		}
	}
}	
