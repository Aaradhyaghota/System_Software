/*
========================================================================================================
Name : 11
Author : Aaradhya Ghota
Description : Write a program to ignore a SIGINT signal then reset the default action of the SIGINT signal - use sigaction system call.
Date: 12 Sept, 2024.
========================================================================================================
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(){
        int status;
        struct sigaction act;
    
        //memset(&act,0,sizeof(act));
        act.sa_handler = SIG_IGN;
        act.sa_flags=0;
        sigemptyset(&act.sa_mask);
        printf("Ignoring SIGINT\n");
        status = sigaction(SIGINT,&act,NULL);
        if(status == -1){
                perror("Error while assigning signal handler!");
        }
        printf("Press CTRL+C to generate an interupt.\n");
        
        sleep(7);
        
        //memset(&act,0,sizeof(act));
        act.sa_handler = SIG_DFL;
        printf("\nSetting Default action of SIGINT\n");
        sigaction(SIGINT,&act,NULL);
        printf("Press CTRL+C to generate an interupt.\n");
        while(1){}
    
        return 0;
}

/*
========================================================================================================
Output:

./a.out
Ignoring SIGINT
Press CTRL+C to generate an interupt.
^C^C^C^C^C
Setting Default action of SIGINT
Press CTRL+C to generate an interupt.
^C

========================================================================================================
*/
