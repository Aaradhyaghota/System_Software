#include <stdio.h>
#include <unistd.h> //header file for this fuvntion of system call
#include <fcntl.h> //contains constructs that refer to file control, e.g. opening a file,
	// retrieving and changing the permissions of file, locking a file for edit, etc.//
#include <stdlib.h> // for using exit function.
int main (int argc , char** argv){
	if(argc != 3){
		printf("Error - Invalid input");
		exit(EXIT_FAILURE);
	}
	int hd_lk = link(argv[1],argv[2]);
	if( hd_lk == 0)printf("Successful created hardlink\n");
	else printf("Unsuccesful- Hardlink not created. \n");
return 0;
}

