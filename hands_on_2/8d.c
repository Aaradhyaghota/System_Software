/*
========================================================================================================
Name : 8.d
Author : Aaradhya Ghota
Description : Write a separate program using signal system call to catch the following signals.
		d. SIGALRM (use alarm system call)
Date: 12 Sept, 2024.
========================================================================================================
*/
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

void handler(){
        printf("Signal received (alarm related)\n");
        //printf("waiting for sleep to end\n");
        //exit(0);
}

int main() {
    
    	signal(SIGALRM, handler);
    	printf("Setting an alarm for 3 seconds!!!!\n");
        alarm(3);
        sleep(10);
        printf("Sleep end\n");
    	return 0;
}



/*
========================================================================================================
Output:

./a.out
Setting an alarm for 3 seconds!!!!
Signal received (alarm related)
Sleep end
========================================================================================================
*/

