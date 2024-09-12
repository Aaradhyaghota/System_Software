/*
========================================================================================================

Question No: 3
Author: Aaradhya Ghota
Description: Write a program to create a file and print the file descriptor value. Use creat ( ) system call
Date: 14th August, 2024

========================================================================================================
*/

#include <stdio.h>
#include <fcntl.h>

int main(int argc, char** argv){
	//      int creat(const char *pathname, mode_t mode);
	//
	//
	int fd  = creat(argv[1], 0774);
	if(fd>0)printf("Succesful\n");
	else printf("Unsuccesful\n");
	printf("File discripter value is %d \n", fd);


return 0;
}

/*
========================================================================================================
Output:

./a.out 3.txt
Succesful
File discripter value is 3 

========================================================================================================
*/
