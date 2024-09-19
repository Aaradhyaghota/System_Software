/*
========================================================================================================
Name : 15
Author : Aaradhya Ghota
Description : Write a simple program to send some data from parent to the child process.
Date: 13 Sept, 2024.
========================================================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>


int main() {
    int pipefds[2];        
    int cpid;            
    char writeb[] = "Hello from parent to child through pipe!";
    char readb[100];    // Buffer to store the data read from the pipe

    if (pipe(pipefds) == -1) {
        printf("Pipe creation failed");
        exit(1);
    }

    cpid = fork();
    if(cpid < 0) {
        printf("Fork failed");
        exit(1);
    }

    if(cpid > 0) {
        
        printf("Passing message from parent to child through pipe\n");
        write(pipefds[1], writeb, strlen(writeb) + 1);
        close(pipefds[1]);

        wait(NULL);
        printf("Parent process exits.\n");
    }
    else{
        printf("Child Process called\n");
        
        read(pipefds[0], readb, sizeof(readb));
        close(pipefds[0]);

        // Display the message received from the pipe
        printf("Child received: %s\n", readb);
        printf("Child process exits.\n");
    }
    
    return 0;
}


/*
========================================================================================================
Output:

./a.out
Passing message from parent to child through pipe
Child Process called
Child received: Hello from parent to child through pipe!
Child process exits.
Parent process exits.


========================================================================================================
*/
