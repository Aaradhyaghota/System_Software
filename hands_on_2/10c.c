/*
========================================================================================================
Name : 10.c
Author : Aaradhya Ghota
Description : Write a separate program using sigaction system call to catch the following signals.
		c. SIGFPE
Date: 12 Sept, 2024.
========================================================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void handler(){
      printf("Caught SIGFPE Signal\n");
      exit(0);
}
int main(){

      int status;
      struct sigaction act;
    
      act.sa_handler=handler;//function that replace default action of signal SIGSEGV
      act.sa_flags=0;
      sigemptyset(&act.sa_mask);//masking set of signal which we want to ignore
      
      status = sigaction(SIGFPE,&act,NULL);
      if(status == -1){
          perror("Error while assigning signal handler!");
      }
      int x = 1;
      int y = 0;
      printf("%d\n", x / y);
      return 0;
    
}



/*
========================================================================================================
Output:

./a.out
Caught SIGFPE Signal

========================================================================================================
*/
