/*
========================================================================================================

Question No: 6
Author: Aaradhya Ghota
Description: Write a program to take input from STDIN and display on STDOUT. Use only read/write system calls
Date: 16th August, 2024

========================================================================================================
*/

#include <stdio.h>
#include <unistd.h>

int main(){
	char buff[20];
	// ssize_t read(int fd, void buf[.count], size_t count);
	// fd - file discripter (0- keyboard , 1- screen)  buf- uffer n which input is stored
	// and count will store the lenght of char input readed by function.
	
	int size = read(0,buff,20);
	if(size != -1){printf("%d byte was readed.\n", size);}
	else {printf("--Error--\nInput failed\n");}

	// to avoid garbage while writting only read only how much you read
	// give size to write functoin  
	int size2 = write(1,buff,size);
	if(size2 == -1)printf("writting failed\n");
	//else printf("output not Printed\n");


return 0;
}

/*
========================================================================================================
Output:

./a.out
Hello, I am Aaradhya
20 byte was readed.
Hello, I am Aaradhya

========================================================================================================
*/
