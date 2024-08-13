#include <stdio.h>
#include <unistd.h> //header file for this fuvntion of system call
#include <fcntl.h> //contains constructs that refer to file control, e.g. opening a file,
	// retrieving and changing the permissions of file, locking a file for edit, etc.//

int main (){
	int flag = link("1b_hardlink.c","linkto1b");
	if(flag == 0)printf("Successful \n");
	else printf("Unsuccesful \n");
return 0;
}

