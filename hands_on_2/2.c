#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

void print(int resource){
    	struct rlimit limit;
    	if (getrlimit(resource, &limit) == 0){
        	if (limit.rlim_cur == RLIM_INFINITY)
            		printf("  Current limit: infinite\n");
        	else
            		printf("  Current limit: %lu\n", (unsigned long)limit.rlim_cur);

        	if (limit.rlim_max == RLIM_INFINITY)
            		printf("  Maximum limit: infinite\n");
        	else
            		printf("  Maximum limit: %lu\n", (unsigned long)limit.rlim_max);
    	}
       	else{
        	perror("getrlimit failed");
    	}
}

int main(){
    	printf("System Resource Limits:\n\n");

    	// CPU time limit
    	printf("RLIMIT_CPU (CPU time in seconds)\n");
    	print(RLIMIT_CPU);

    	// File size limit
    	printf("RLIMIT_FSIZE (Maximum file size)\n");
    	print(RLIMIT_FSIZE);

    	// Data segment size limit
    	printf("RLIMIT_DATA (Maximum data segment size)\n");
    	print(RLIMIT_DATA);

    	// Stack size limit
    	printf("RLIMIT_STACK (Maximum stack size)\n");
    	print(RLIMIT_STACK);

    	// Core file size limit
    	printf("RLIMIT_CORE (Maximum core file size)\n");
    	print(RLIMIT_CORE);

    	// Resident set size limit
    	printf("RLIMIT_RSS (Maximum resident set size)\n");
    	print(RLIMIT_RSS);

    	// Open file descriptors limit
    	printf("RLIMIT_NOFILE (Maximum number of open file descriptors)\n");
    	print(RLIMIT_NOFILE);

    	// Maximum number of processes
    	printf("RLIMIT_NPROC (Maximum number of processes)\n");
    	print(RLIMIT_NPROC);

    	// Address space limit
    	printf("RLIMIT_AS (Address space limit)\n");
    	print(RLIMIT_AS);

    	return 0;
}

