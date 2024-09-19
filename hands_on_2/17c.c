/*
========================================================================================================
Name : 17.c
Author : Aaradhya GHota
Description : Write a program to execute ls -l | wc.
		c. Use fcntl
Date: 14 Sept, 2024.
========================================================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main(){

        int pipefd[2];
        int cid;

        if (pipe(pipefd) == -1){
            printf("ERROR");
        }
    
    
        cid = fork();
        if(cid == 0){
            close(0);  //Closing stdin
       
            fcntl(pipefd[0], F_DUPFD, 0); //pipefd[0] --> STDIN
            close(pipefd[1]); 
            execl("/bin/wc", "wc", NULL);
        }
        else{
            close(1); //closing stdout
            
            fcntl(pipefd[1], F_DUPFD, 1); // pipefd[1] --> STDOUT
            close(pipefd[0]); 

            execl("/bin/ls", "ls -l", "-l", NULL);
        }
}

/*
========================================================================================================
Output:

./a.out
     33     290    2174
   // no of lines  // no of words / no of bytes
========================================================================================================
*/
