/*
========================================================================================================

Question No.: 28
Author: Aaradhya GHota
Description: Write a program to get maximum and minimum real time priority.
Date: 30th August, 2024

========================================================================================================
*/
#include <stdio.h>
#include <sched.h>

void print_priority_range(int policy) {

    	int min = sched_get_priority_min(policy);
    	int max = sched_get_priority_max(policy);

    	if (min == -1 || max == -1) {
        	printf("Error in getting the min and max priority");
        	return ;
    	} 
    	else {
        	switch (policy) {
            		case SCHED_FIFO:
                		printf("SCHED_FIFO: Min Priority = %d, Max Priority = %d\n", min, max);
                		break;
            		case SCHED_RR:
               			printf("SCHED_RR: Min Priority = %d, Max Priority = %d\n", min, max);
                		break;
            		case SCHED_OTHER:
                		printf("SCHED_OTHER: Min Priority = %d, Max Priority = %d\n", min, max);
                		break;
            		default:
                		printf("Unknown policy\n");
        	}	
    	}
}

int main() {
    	print_priority_range(SCHED_FIFO);
    	print_priority_range(SCHED_RR);
    	print_priority_range(SCHED_OTHER);

    	return 0;
}

/*
========================================================================================================
Output:

./a.out
SCHED_FIFO: Min Priority = 1, Max Priority = 99
SCHED_RR: Min Priority = 1, Max Priority = 99
SCHED_OTHER: Min Priority = 0, Max Priority = 0


========================================================================================================
*/
