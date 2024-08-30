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
	int i ;
        unsigned long long int nano;
	unsigned long long int start, end;

    	start = rdtsc();
	
    	for(int i =0;i<=100;i++)
		getppid();

    	end = rdtsc();
	nano = end - start/2.3;
	printf("The function takes %lld nano second\n", nano);
	
}
