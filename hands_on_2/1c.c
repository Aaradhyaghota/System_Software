/*
========================================================================================================
Name : 1.c
Author : Aaradhya Ghota
Description : Write a separate program (for each time domain) to set a interval timer in 10sec and 10micro second
 		c. ITIMER_PROF
Date: 12 Sept, 2024.
========================================================================================================
*/

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

void timer_handler(int signum){
        printf("ITIMER_PROF: Timer expired\n");
        exit(0);
}

int main() {
        struct itimerval tm;

        signal(SIGPROF, timer_handler);

        tm.it_value.tv_sec = 10;
        tm.it_value.tv_usec = 10;

        tm.it_interval.tv_sec = 0;
        tm.it_interval.tv_usec = 0;

        setitimer(ITIMER_PROF, &tm, NULL);

        while(1){}

        return 0;
}


/*
========================================================================================================
Output:
./a.out
ITIMER_PROF: Timer expired

========================================================================================================
*/

