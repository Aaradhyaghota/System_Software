
/*
========================================================================================================
Name : 17.b
Author : Aaradhya Ghota
Description : Write a program to execute ls -l | wc.
		b. Use dup2
Date: 14 Sept, 2024.
========================================================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

void main()
{
    int pipefd[2];
    int cid;

    if (pipe(pipefd )== -1)
    {
        printf("ERROR - PIpe creation failed.");
    }
    
        cid = fork();
        if(cid == 0)
        {   //child process get output of parent which it will read from pipe. 
            
            close(0);
            dup2(pipefd[0], 0);
            close(pipefd[1]); // closing unused pipe writing end
            
            execl("/bin/wc", "wc", NULL);
        }
        else{   
            //parent process give output to child process through pipe by writting into it. 
            close(1);          
            dup2(pipefd[1], 1); 
            close(pipefd[0]); // closing unused reading end

            execl("/bin/ls", "ls -l", "-l", NULL);
        }
}

/*
========================================================================================================
Output:

./a.out
     32     281    2106

========================================================================================================
*/


