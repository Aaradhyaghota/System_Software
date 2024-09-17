#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

int main(){
    	struct rlimit limit;

    	if(getrlimit(RLIMIT_NOFILE, &limit) == 0){
        	printf("Current soft limit for open files: %lu\n", (unsigned long)limit.rlim_cur);
        	printf("Current hard limit for open files: %lu\n", (unsigned long)limit.rlim_max);
    	}
	else{
        	perror("Failed to get current resource limit");
        	return 1;
    	}

    	// Set new soft and hard limits for RLIMIT_NOFILE
    	limit.rlim_cur = 2048; 
    	limit.rlim_max = 4096;//to increase the hardlimit then the curent hard limit then sudo  is used can be done by root user only. 
    	
    	if(setrlimit(RLIMIT_NOFILE, &limit) == 0){
        	printf("Successfully set new limits.\n");
    	}
	else{
        	perror("Failed to set new limit");
        	return 1;
    	}

    	if(getrlimit(RLIMIT_NOFILE, &limit) == 0) {
        	printf("New soft limit for open files: %lu\n", (unsigned long)limit.rlim_cur);
        	printf("New hard limit for open files: %lu\n", (unsigned long)limit.rlim_max);
    	}
	else{
        	perror("Failed to get resource limit");
        	return 1;
    	}

    	return 0;
}

