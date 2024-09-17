/*
========================================================================================================
Name : 8.e
Author : Aaradhya Ghota
Description : Write a separate program using signal system call to catch the following signals.
		e. SIGALRM (use setitimer system call)
Date: 12 Sept, 2024.
========================================================================================================
*/

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

void handler(){
    	printf("SIGALRM Signal received\n");
        exit(0);
}


int main() {
	struct itimerval tm;
    
    	signal(SIGALRM, handler);

    	tm.it_value.tv_sec = 3;
    	tm.it_value.tv_usec = 0;
    
    	tm.it_interval.tv_sec = 0;
    	tm.it_interval.tv_usec = 0;

   	setitimer(ITIMER_REAL, &tm, NULL);

    	while(1){}

    	return 0;
}


/*
========================================================================================================
Output:

./a.out
SIGALRM Signal received

========================================================================================================
*/
