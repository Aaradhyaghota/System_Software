/*
========================================================================================================

Question No: 4
Author: Aaradhya Ghota
Description: Write a program to open an existing file with read write mode. Try O_EXCL flag also.
Date: 14th August, 2024

========================================================================================================
*/



#include <stdio.h>
#include <fcntl.h>

int main(){
	
	int fd  = open("nwfile",O_CREAT | O_EXCL|O_RDWR, 0777);   //O_CREAT | O_EXCL -> opens function opens file only when is its not exist  
	if(fd == -1) printf("file not opened");
	else printf("File Opened\n");
return 0;
}

/*
========================================================================================================
Output:

File Opened

========================================================================================================
*/
