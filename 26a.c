/*
========================================================================================================

Question No.: 26a
Author: Aaradhya Ghota
Description: Write a program to execute an executable program.
        a. use some executable program
        b. pass some input to an executable program. (for example execute an executable of $./a.out name)
Date: 30th August, 2024

========================================================================================================
*/

#include<stdio.h>
#include <unistd.h>

int main (){
	printf("Executing executable program\n");

    	if (execl("./20priority","./20priority", NULL) == -1) {
        	printf("Error - in execl fnction");
        	return 1;
    	}
	
	printf("This will be printed if execl doesn't run executable file\n");
	
	return 0;
}

/*
========================================================================================================
Output:

./a.out

Executing executable program
The priority (nice value) of the running process id: 21179 is 0
The new priority (nice value) of process id: 21179 after changing its priority is 5


========================================================================================================
*/
