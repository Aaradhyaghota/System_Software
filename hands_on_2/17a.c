/*
========================================================================================================
Name : 17.a
Author : Aaradhya Ghota
Description : Write a program to execute ls -l | wc.
		a. Use dup
Date: 13 Sept, 2024.
========================================================================================================
*/


#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>

int main(){
    int pipefd[2];
    int cid;

        if(pipe(pipefd)==-1){
                printf("ERROR - PIpe creation failed.");
        }
    
        cid=fork();
        if(cid==0){
                //child will read the output from the parent process
                
                //closing standard input
                close(0);
                // reassigning stdin to pipe reading end;
                // dup will reassign pipefd[0] to 0 as it is lowest avalible fd.so now pipefd[0] will act as STDIN
                dup(pipefd[0]);
                close(pipefd[1]);//closing unused pipe writing end
                execl("/bin/wc", "wc", NULL);
        }else{  
                //parent will send the ouput of first command and that is written to pipe
                
                close(1);//closing stdout 
                dup(pipefd[1]);//reassigning stdout to pipe writing end
                // dup will reassign pipefd[1] to 1(STDOUT) as it is lowes avalible fd. so now pipefd[1] will act as STDOUT
                close(pipefd[0]);//closing unused reading end

                execl("/bin/ls", "ls", "-l", NULL);
        }
}


/*
========================================================================================================
Output:

./a.out
     31     272    2038

========================================================================================================
*/

