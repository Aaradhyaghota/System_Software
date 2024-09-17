/*
========================================================================================================
Name : 9
Author : Aaradhya Ghota
Description : Write a program to ignore a SIGINT signal then reset the default action of the SIGINT signal - Use signal system call.
Date: 12 Sept, 2024.
========================================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(){
        printf("Ignoring SIGINT signal for 5 sec\n");
    	signal(SIGINT, SIG_IGN);
    	printf("Press Ctrl+C.\n");
    	// CTRL + C is ignored.
    	sleep(5);
    	printf("\nResetting Default action of SIGINT signal\n");
    	signal(SIGINT, SIG_DFL);
    	printf("Press Ctrl+C.\n");
    	// CTRL + C is no longer ignored.
    	while(1){}
    	return 0;
}

/*
========================================================================================================
Output:

./a.out
Ignoring SIGINT signal for 5 sec
Press Ctrl+C.
^C^C^C
Resetting Default action of SIGINT signal
Press Ctrl+C.
^C

========================================================================================================
*/



