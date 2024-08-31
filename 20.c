/*
========================================================================================================

Question No.: 20
Author: Aaradhya Ghota
Description: Find out the priority of your running program. Modify the priority with nice command.
Date: 30th August, 2024

========================================================================================================
*/
#include<stdio.h>
#include <unistd.h>
//#include <sys/time.h>
#include <sys/resource.h>//for getpriority & set
#include <stdlib.h>
#include <errno.h>

int main(){
 	  
	int pid = getpid();
  
 	errno = 0;  // Clear errno before calling getpriority
  	//Since a successful call to getpriority() can  legitimately  return  the
  	// value  -1, it is necessary to clear errno prior to the call, then check
  	// errno afterward to determine if -1 is an error or a legitimate value.

  	int priority = getpriority(PRIO_PROCESS, pid);
	if (priority == -1 && errno != 0) {
      		printf("Error in calling getpriority\n");
      		return 1;
  	}
  	printf("The priority (nice value) of the running process id: %d is %d\n", pid, priority);
  	//setting the non real time priority - Increase priority by decreasing nice value by 5
  	
	errno = 0;	
	int increment = 5;//if you want to add negative no. then run the exe by sudo 
  	int new_priority = nice(increment);
  	
	if (new_priority == -1 && errno != 0) {
        	printf("Error calling nice\n");
        	return 1;
 	   }
  
 	printf("The new priority (nice value) of process id: %d after changing its priority is %d\n",pid ,new_priority);
    
}

/*
========================================================================================================
Output:

./a.out
The priority (nice value) of the running process id: 19691 is 0
The new priority (nice value) of process id: 19691 after changing its priority is 5


========================================================================================================
*/
