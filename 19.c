/*
========================================================================================================

Question No.: 19
Author: Aaradhya Ghota
Description: Write a program to find out time taken to execute getpid system call. Use time stamp counter.
Date: 30th August, 2024

========================================================================================================
*/
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

unsigned long long rdtsc()
{
    unsigned long long dst;
    __asm__ __volatile__("rdtsc": "=A"(dst));
    return dst;
}

int main(){
	
        unsigned long long int nano;
	unsigned long long int start, end;

    	start = rdtsc();
	
	int getpid();

    	end = rdtsc();
	nano = (end - start)/2.3;
	printf("The time taken by getpid() function takes %lld nano second\n", nano);
	
}

/*
========================================================================================================
Output:

./a.out
The time taken by getpid() function takes 40 nano second


========================================================================================================
*/
