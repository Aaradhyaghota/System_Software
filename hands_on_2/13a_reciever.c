/*
========================================================================================================
Name : 13
Author : Aaradhya Ghota
Description : Write two programs: first program is waiting to catch SIGSTOP signal, the second program will send the signal (using kill system call). Find out whether the first program is able to catch the signal or not.
Date: 12 Sept, 2024.
========================================================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdbool.h>


bool flag = false;
void handler(int sig){    
    if (sig == SIGSTOP) {
        printf("Caught SIGSTOP, process stopped.\n");
    }
    if (sig == SIGCONT) {
        printf("Caught SIGCONT.\n");
        flag = true;
    }   
}

int main(){

    signal(SIGSTOP, handler);
    signal(SIGCONT, handler);
    
    while (1) {
        printf("Waiting for signals...\n");
        sleep(3);  // Sleep to make it easier to observe
        if(flag){
            printf("SIGCONT signal caught. Program resumes.\n");
            break;
        }
    }
    printf("Exiting current process (reciever)\n"); 
}


/*
========================================================================================================
Output:

Program is not able to catch SIGSTOP signal.

./a.out &
[1] 4531
Waiting for signals...
Waiting for signals...
Waiting for signals...
Waiting for signals...
Waiting for signals...
//wait of 10 sec
Caught SIGCONT, process resumed after being stopped.
SIGCONT signal caught. Program resumes.
Exiting current process (reciever)



========================================================================================================
*/
