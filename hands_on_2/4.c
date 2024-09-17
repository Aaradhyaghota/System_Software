#include <stdio.h>
#include <unistd.h>  // for getppid()
#include <stdint.h>  // for uint64_t

// Function to read the time stamp counter (TSC)
static inline uint64_t rdtsc() {
    	unsigned int lo, hi;
    
    	__asm__ volatile ("rdtsc" : "=a" (lo), "=d" (hi));
    	return ((uint64_t)hi << 32) | lo;
}

int main() {
    	uint64_t start_tsc, end_tsc;
    
    	start_tsc = rdtsc();
    
    	for(int i = 0; i < 100; ++i){
        	getppid(); 
    	}

    	end_tsc = rdtsc();

    	uint64_t time_diff = (end_tsc - start_tsc)/2.3;
    	printf("The time taken by 100 getppid() function to execute is :  %lu nano second\n", time_diff);

    return 0;
}

