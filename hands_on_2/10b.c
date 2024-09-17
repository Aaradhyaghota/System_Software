/*
========================================================================================================
Name : 10.b
Author : Aaradhya Ghota
Description : Write a separate program using sigaction system call to catch the following signals.
		b. SIGINT
Date: 12 Sept, 2024.
========================================================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void handler(){
    	printf("\nCaught Interrupt SIGINT signal\n");
    	exit(0);
}

int main() {
    
    	int status;
        struct sigaction act;
    
        act.sa_handler=handler;//function that replace default action of signal SIGSEGV
        act.sa_flags=0;
        sigemptyset(&act.sa_mask);//masking set of signal which we want to ignore
      
        status = sigaction(SIGINT,&act,NULL);
        if(status == -1){
                perror("Error while assigning signal handler!");
        }
        printf("Press CTRL+C to generate an interupt tp process and it will trigger intrupt signal.\n");
        while(1){}

    	return 0;
}


/*
========================================================================================================
Output:

./a.out
Press CTRL+C to generate an interupt tp process and it will trigger intrupt signal.
^C
Caught Interrupt SIGINT signal

========================================================================================================
*/
