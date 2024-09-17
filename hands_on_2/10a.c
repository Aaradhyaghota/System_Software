/*
========================================================================================================
Name : 10.a
Author : Aaradhya Ghota
Description : Write a separate program using sigaction system call to catch the following signals.
                a. SIGSEGV
Date: 12 Sept, 2024.
========================================================================================================
*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void handler(){
    	printf("Segmentation fault SIGSEGV signal\n");
    	exit(0);
}

int main() {
    
    	int status;
        struct sigaction act;
        //memset(&ac, 0, sizeof(ac));   // Zero out the sigaction struct
    
        act.sa_handler=handler;//function that replace default action of signal SIGSEGV
        act.sa_flags=0;
        sigemptyset(&act.sa_mask);//masking set of signal which we want to ignore
      
        status = sigaction(SIGSEGV,&act,NULL);
        if(status == -1){
                perror("Error while assigning signal handler!");
        }

        char *str;
        str = NULL;
        printf("%c\n", *(str + 1));

        printf("No segmentation fault\n");
    	return 0;
}

/*
========================================================================================================
Output:

./a.out
Segmentation fault SIGSEGV signal

========================================================================================================
*/

