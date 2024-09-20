
/*
========================================================================================================
Name : 19.e
Author : Aaradhya GHota
Description : Create a FIFO file using mkfifo library function.
Date: 14 Sept, 2024.
========================================================================================================
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

int main() {
        char *fifo_name = "fifoByMkfifo";

        if(mkfifo(fifo_name, 0777) == -1) {
                perror("mkfifo");      
        }

        printf("FIFO '%s' created using mkfifo.\n", fifo_name);
        return 0;
}


/*
========================================================================================================
Output:

./a.out
FIFO 'fifoByMkfifo' created using mkfifo.

========================================================================================================
*/
