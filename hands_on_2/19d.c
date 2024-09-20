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
     char *path = "./fifoByMknod";

    
    if (mknod(path, S_IFIFO | 0666, 0) == -1) {
        perror("mknod");
    }
    else
        printf("FIFO '%s' created using mknod.\n", path);
    return 0;
}

/*
========================================================================================================
Output:

./a.out
FIFO 'fifoByMknod' created using mknod.

========================================================================================================
*/
