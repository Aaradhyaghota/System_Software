/*
========================================================================================================

Question no.: 29
Author: Aaradhya Ghota
Description: Write a program to get scheduling policy and modify the scheduling policy (SCHED_FIFO, SCHED_RR).
Date: 30th August, 2024

========================================================================================================
*/

#include <stdio.h>
#include <sched.h>
#include <unistd.h>

void print_policy(int policy) {
    	if (policy == SCHED_FIFO)
        	printf("Current scheduling policy: SCHED_FIFO \n");
    	else if (policy == SCHED_RR)
        	printf("Current scheduling policy: SCHED_RR \n");
    	else if (policy == SCHED_OTHER)
        	printf("Current scheduling policy: SCHED_OTHER \n");
    	else 
        	printf("Unknown scheduling policy \n");
}

int main(){
	
    	int pid = getpid();
    
    	int policy = sched_getscheduler(pid);
    	if (policy == -1){
        	printf("Error in getting the current scheduler\n");
        	return 0;
    	}
    
   	print_policy(policy);
    
    	int choice;
    	printf("To change scheduling policy select \n1:SCHED_FIFO \n2:SCHED_RR \n3:SCHED_OTHER \n");
    	scanf("%d", &choice);
    
    	struct sched_param p;
    	switch (choice){
    		case 1:
        		p.sched_priority = 15;
        		sched_setscheduler(pid, SCHED_FIFO, &p);
       			break;
    		case 2:
        		p.sched_priority = 25;
        		sched_setscheduler(pid, SCHED_RR, &p);
        		break;
    		case 3:
        		p.sched_priority = 0;
        		sched_setscheduler(pid, SCHED_OTHER, &p);
        		break;
    		default:
        		printf("Invalid choice");
       			 break;
    	}

    	policy = sched_getscheduler(pid);
    
   	 if (policy == -1){
        	printf("Error in getting the current scheduler\n");
        	return 0;
    	}
    
    	print_policy(policy);
}


//while runnig the ./a.out use sudo because you are doing some authorised work-->>sudo ./a.out
 
*
========================================================================================================
Output:

sudo ./a.out
Current scheduling policy: SCHED_OTHER 
To change scheduling policy select 
1:SCHED_FIFO 
2:SCHED_RR 
3:SCHED_OTHER 
2
Current scheduling policy: SCHED_RR 

sudo ./a.out
Current scheduling policy: SCHED_OTHER 
To change scheduling policy select 
1:SCHED_FIFO 
2:SCHED_RR 
3:SCHED_OTHER 
2
Current scheduling policy: SCHED_RR 


========================================================================================================
*/
