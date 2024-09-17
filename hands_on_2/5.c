/*
========================================================================================================
Name : 5
Author : Aaradhya Ghota
Description : Write a program to print the system limitation of:
        a. maximum length of the arguments to the exec family of functions
        b. maximum number of simultaneous process per user id
        c. number of clock ticks (jiffy) per second
        d. maximum number of open files
        e. size of a page
        f. total number of pages in the physical memory
        g. number of currently available pages in the physical memory
Date: 12 Sept, 2024.
========================================================================================================
*/
#include <stdio.h>
#include <unistd.h>   // for sysconf()
#include <limits.h>   // for _POSIX_ARG_MAX and other constants
#include <sys/sysinfo.h>  // for sysinfo()

int main() {
  
    	long arg_max = sysconf(_SC_ARG_MAX);
    	if (arg_max != -1) {
        	printf("Maximum length of arguments to exec() family: %ld bytes\n", arg_max);
    	}
	else{
        	perror("sysconf(_SC_ARG_MAX) failed");
    	}

       	long mnp = sysconf(_SC_CHILD_MAX);
    	if (mnp != -1) {
        	printf("Maximum number of simultaneous processes per user id: %ld\n", mnp);
    	}
	else{
        	perror("sysconf(_SC_CHILD_MAX) failed");
    	}

    	long clk_tck = sysconf(_SC_CLK_TCK);
   	if (clk_tck != -1) {
        	printf("Number of clock ticks per second: %ld\n", clk_tck);
        	// A "jiffy" is the time between two successive clock ticks, and 
        	// the number of ticks per second is retrieved using sysconf(_SC_CLK_TCK).
    	}
	else{
        	perror("sysconf(_SC_CLK_TCK) failed");
    	}

	long opmax = sysconf(_SC_OPEN_MAX);
    	if (opmax != -1) {
        	printf("Maximum number of open files: %ld\n", opmax);
    	}
	else{
        	perror("sysconf(_SC_OPEN_MAX) failed");
    	}

    	long pgSz = sysconf(_SC_PAGESIZE);
   	if (pgSz != -1) {
        	printf("Size of a page: %ld bytes\n", pgSz);
    	}
	else{
        	perror("sysconf(_SC_PAGESIZE) failed");
    	}

    	long tp = sysconf(_SC_PHYS_PAGES);
   	if(tp != -1) {
        	printf("Total number of pages in physical memory: %ld\n", tp);
    	}
	else{
        	perror("sysconf(_SC_PHYS_PAGES) failed");
    	}

    	long ap = sysconf(_SC_AVPHYS_PAGES);
    	if(ap != -1) {
        	printf("Number of currently available pages in physical memory: %ld\n", ap);
    	}
	else{
        	perror("sysconf(_SC_AVPHYS_PAGES) failed");
    	}

    	return 0;
}

/*
========================================================================================================
Output:

./a.out

Maximum length of arguments to exec() family: 2097152 bytes
Maximum number of simultaneous processes per user id: 23022
Number of clock ticks per second: 100
Maximum number of open files: 1024
Size of a page: 4096 bytes
Total number of pages in physical memory: 1492202
Number of currently available pages in physical memory: 587924



========================================================================================================
*/
