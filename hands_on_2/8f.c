/*
========================================================================================================
Name : 8.f
Author : Aaradhya Ghota
Description : Write a separate program using signal system call to catch the following signals.
		f. SIGVTALRM (use setitimer system call)
Date: 12 Sept, 2024.
========================================================================================================
*/

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

void handler(){
        printf("SIGVTALRM Signal received\n");
        exit(0);
}

int main() {
        struct itimerval tm;

        signal(SIGVTALRM, handler);

        tm.it_value.tv_sec = 4;
        tm.it_value.tv_usec = 0;

        tm.it_interval.tv_sec = 0;
        tm.it_interval.tv_usec = 0;

        setitimer(ITIMER_VIRTUAL, &tm, NULL);
        
        
        while(1){}

        return 0;
}

/*
========================================================================================================
Output:

./a.out
SIGVTALRM Signal received
========================================================================================================
*/
