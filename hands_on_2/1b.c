/*
========================================================================================================
Name : 1.b
Author : Aaradhya GHota
Description : Write a separate program (for each time domain) to set a interval timer in 10sec and 10micro second
 		b. ITIMER_VIRTUAL
Date: 12 Sept, 2024.
========================================================================================================
*/

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

void timer_handler(int signum){
        printf("ITIMER_VIRTUAL: Timer expired\n");
        exit(0);
}

int main() {
        struct itimerval tm;

        signal(SIGVTALRM, timer_handler);

        tm.it_value.tv_sec = 10;
        tm.it_value.tv_usec = 10;

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
ITIMER_VIRTUAL: Timer expired

========================================================================================================
*/

