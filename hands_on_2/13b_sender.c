/*
========================================================================================================
Name : 13
Author : Aaradhya Ghota
Description : Write two programs: first program is waiting to catch SIGSTOP signal, the second program will send the signal (using kill system call). Find out whether the first program is able to catch the signal or not.
Date: 13 Sept, 2024.
========================================================================================================
*/

#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
int main()
{
    int pid;
    printf("Enter process id\n");
    scanf("%d",&pid);
    int status = kill(pid, SIGSTOP);
    if (status == -1)
    {
        perror("Error while sending kill signal!");
    }
    printf("SIGSTOP signal sent to process %d\n", pid);
    
    sleep(10); 
    
    printf("Sending SIGCONT signal to process %d\n", pid);
    status = kill(pid, SIGCONT);
    if (status == -1)
    {
        perror("Error while sending kill signal!");
    }
    
    printf("Exiting current process (sender)\n");
    return 0;
}


/*
========================================================================================================
Output:

./a.out
Enter process id
4531
SIGSTOP signal sent to process 4531
// wait of 10 sec
Sending SIGCONT signal to process 4531
Exiting current process (sender)

========================================================================================================
*/
