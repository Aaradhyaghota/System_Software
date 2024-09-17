/*
========================================================================================================
Name : 8.a
Author : Aaradhya Ghota
Description : Write a separate program using signal system call to catch the following signals.
		a. SIGSEGV
Date: 12 Sept, 2024.
========================================================================================================
*/
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

void handler(){
    	printf("Segmentation fault caught\n");
    	exit(0);
}

int main() {
    
    	signal(SIGSEGV, handler);

        char *str;
        str = NULL;
        printf("%c\n", *(str + 10));

        printf("No segmentation fault\n");
        
    	

    	return 0;
}

/*
========================================================================================================
Output:

./a.out
Segmentation fault caught
========================================================================================================
*/


