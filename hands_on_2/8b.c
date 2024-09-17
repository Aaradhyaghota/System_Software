/*
========================================================================================================
Name : 8.b
Author : Aaradhya Ghota
Description : Write a separate program using signal system call to catch the following signals.
		b. SIGINT
Date: 12 Sept, 2024.
========================================================================================================
*/

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

void handler(){
        printf("\nSIGINT received\n");
        exit(0);
}

int main() {
    
    	signal(SIGINT, handler);
    	printf("Press CTRL+C to generate an interupt tp process and it will trigger intrupt signal.\n");
    	while(1);//infinite loop waiting to terminate
    	return 0;
}

/*
========================================================================================================
Output:

./a.out
Press CTRL+C to generate an interupt tp process and it will trigger intrupt signal.
^C
SIGINT received

========================================================================================================
*/
