/*
========================================================================================================

Question No: 1c
Author: Aaradhya Ghota
Description: Create the following types of a files using (i) shell command (ii) system call
                c. FIFO (mkfifo Library Function or mknod system call)
Date: 14th August, 2024

========================================================================================================
*/



#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv){
	if(argc != 2){
		printf("Error- Input Valid input");
		exit(0);
	}
	int flag = mknod(argv[1], S_IFIFO | 0666, 0);
	if(flag == 0)printf("------------- Succesfully Created fifo named '%s' ------------\n",argv[1]);
	else printf("Fifo creation Unsuccesful\n");
	

return 0;
}

/*
========================================================================================================
Output:

./a.out 1cfifo
------------- Succesfully Created fifo named '1cfifo' ------------

========================================================================================================
*/
