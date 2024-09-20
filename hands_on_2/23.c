/*
========================================================================================================
Name : 23
Author : Aaradhya GHota
Description : Write a program to print the maximum number of files that can be opened within a process and size of a pipe (circular buffer).
Date: 14 Sept, 2024.
========================================================================================================
*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdio.h>

int main(){
	long maxOpenFls;   // Maximum number of files that can be opened within process
	long maxSizePipe; // The maximim size of a FIFO file

        char *path = "23fifo";
        mkfifo(path, S_IRWXU);//(path , 0777)

        maxOpenFls = sysconf(_SC_OPEN_MAX);

        printf("Maximum number of files that can be opened within a process is: %ld\n", maxOpenFls);

        //maxSizePipe = pathconf(path, _PC_PIPE_BUF); //for size of fifo file 's' buffer
        //printf("%ld\n",maxSizePipe);
        printf("Maximum size of pipe: %d\n", PIPE_BUF);
    
}

/*
========================================================================================================
Output:

Maximum number of files that can be opened within a process is: 1024
Maximum size of pipe: 4096

========================================================================================================
*/


