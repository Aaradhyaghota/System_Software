/*
========================================================================================================
Name : 14
Author : Aaradhya Ghota
Description : Write a simple program to create a pipe, write to the pipe, read from pipe and display on the monitor.
Date: 13 Sept, 2024.
========================================================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
    int pipefd[2]; // pipefd[0] for reading end and pipefd[1] is writing end
    char inputbuf[10];
    char readPipeBuff[10];
    

    // creating pipe
    if (pipe(pipefd) == -1)
    {
        printf("pipe error");
        exit(1);
    }
    printf("Enter string of size 10 character\n");
    scanf("%s", inputbuf);

    write(pipefd[1], inputbuf, 10);
    close(pipefd[1]);

    read(pipefd[0], readPipeBuff, 10);
    close(pipefd[0]);
    
    printf("Display on Monitor\n");
    write(1, readPipeBuff, 10);
    printf("\n");
    
    return 0;
}



/*
========================================================================================================
Output:

./a.out
Enter string of size 10 character
Aaradhya
Display on Monitor
Aaradhya

========================================================================================================
*/
