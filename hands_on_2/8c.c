/*
========================================================================================================
Name : 8.c
Author : Aaradhya Ghota
Description : Write a separate program using signal system call to catch the following signals.
		c. SIGFPE
Date: 12 Sept, 2024.
========================================================================================================
*/
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

void handler(){
        printf("Floating point error caught\n");
        exit(0);
}

int main() {
    
        int x = 3;
        int y = 0;
        signal(SIGFPE, handler);
        printf("%d\n", x / y);
	return 0;
}







/*
========================================================================================================
Output:

./a.out
Floating point error caught

========================================================================================================
*/
