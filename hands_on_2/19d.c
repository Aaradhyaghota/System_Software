/*
========================================================================================================
Name : 19.d
Author : Aaradhya GHota
Description : Create a FIFO file using mknod system call.
Date: 14 Sept, 2024.
========================================================================================================
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
     char *fifo_name = "fifoByMknod";

    
    if (mknod(fifo_name, S_IFIFO | 0666, 0) == -1) {
        printf("Error in creating fifo by mknod system call");
        exit(1);
    }

    printf("FIFO '%s' created using mknod.\n", fifo_name);
    return 0;
}

/*
========================================================================================================
Output:

./a.out
FIFO 'fifoByMknod' created using mknod.

========================================================================================================
*/
