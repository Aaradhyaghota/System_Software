#include <stdio.h>
#include <sys/select.h>

int main(){
	fd_set fds;          
    	struct timeval tmOut; 

    	FD_ZERO(&fds);
   	FD_SET( 0 , &fds); 
    
    	tmOut.tv_sec = 10;
    	tmOut.tv_usec = 0;

    	printf("Waiting for input on STDIN for up to 10 sec\n");
    	int ck_val = select(1, &fds, NULL, NULL, &tmOut);

	if(ck_val  == -1)
		printf("Error");
	else if(ck_val == 0)
		printf("No data is entered  within 10 seconds.\n");
	else if(ck_val == 1)
		printf("Some data is entered form STD_IN Within 10 Second\n");
}
