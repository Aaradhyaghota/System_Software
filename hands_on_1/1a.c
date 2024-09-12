/*
========================================================================================================

Question No: 1a
Author: Aaradhya Ghota
Description: Create the following types of a files using (i) shell command (ii) system call
		a. soft link (symlink system call)
Date: 14th August, 2024

========================================================================================================
*/
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(){
	int flag = symlink("1a.c","linkto1a");
	if(flag == 0){printf("Successful created Softlink \n");}
	else {printf("Unsuccessful \n");}
return 0; 	
}

/*
========================================================================================================
Output:

Successful created Softlink

========================================================================================================
*/
