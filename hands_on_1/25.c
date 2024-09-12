/*
========================================================================================================

Question No.: 25
Author: Aaradhya Ghota
Description: Write a program to create three child processes. The parent should wait for a particular child (use waitpid system call).
Date: 30th August, 2024

========================================================================================================
*/

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

/*
========================================================================================================
Output:

./a.out
Child-1 called
parent Processs waiting for child - 2 to terminate
Child-2 called
Child-3 called
----after 3sec ---------
Child-1 terminated and exit
----after 6 sec --------
Child-3 terminated and exit
--after 15 sec ---------
Child-2 terminated and exit
Parent process terminated and exit

