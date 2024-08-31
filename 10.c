/*
========================================================================================================

Question No: 10
Author: Aaradhya Ghota
Description: Write a program to open a file with read write mode, write 10 bytes, move the file pointer by 10 bytes (use lseek) and write again 10 bytes.
	a. check the return value of lseek
	b. open the file with od and check the empty spaces in between the data.
Date: 16th August, 2024

========================================================================================================
*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char** argv){
	if(argc != 2){
		printf("Error - Invalid Input");
		exit(EXIT_FAILURE);
	}

	int fd = open(argv[1], O_RDWR | O_CREAT , 0644);
	if(fd == -1){printf("Error");}

	int i = lseek(fd, 0 , SEEK_END);
	printf("lseek position before write : %d\n",i);
		
	int sz1 = write(fd, "firstwrite",10);
	int l = lseek(fd , 0 , SEEK_CUR);
	printf("lseek position after first write : %d\n",l);

	int j = lseek(fd , 10 , SEEK_CUR);
	printf("lseek position after moving 10 byte forward : %d\n",j);
        
	int sz2 = write(fd, "secondwrit",10);
	int k = lseek(fd , 0 , SEEK_CUR);
	printf("lseek position after Second write : %d\n",k);
	
	close(fd);
	return 0;
}
// 10(b) use od -c filename in the terminal to find the spaces in the files , -c will give the asccii values of all the character in file.


/*
========================================================================================================
Output:

./a.out 10.txt
lseek position before write : 0
lseek position after first write : 10
lseek position after moving 10 byte forward : 20
lseek position after Second write : 30

inside the file -- 
firstwrite\00\00\00\00\00\00\00\00\00\00secondwrit

od -c 10.txt
0000000   f   i   r   s   t   w   r   i   t   e  \0  \0  \0  \0  \0  \0
0000020  \0  \0  \0  \0   s   e   c   o   n   d   w   r   i   t
0000036

========================================================================================================
*/

