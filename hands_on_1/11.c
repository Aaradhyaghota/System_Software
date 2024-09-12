/*
========================================================================================================

Question No: 11
Author: Aaradhya Ghota
Description:Write a program to open a file, duplicate the file descriptor and append the file with both the descriptors and check whether the file is updated properly or not.
	a. use dup
	b. use dup2
	c. use fcntl 
Date: 28th August, 2024

========================================================================================================
*/
#include <stdio.h>
#include <fcntl.h>   /* Definition of O_* constants */
#include <unistd.h>

int main(){
	int fd = open("11_file1.txt",O_RDWR| O_CREAT, 0644);
	int dup_fd = dup(fd);
	
	printf("pid from dup :%d \n",dup_fd);
	
	int wr_sz = write(fd , "dup-writting using the older fd\n",32);

	int wr_sz1 = write(dup_fd , "  dup-writtng using the new fd\n",32);
	
	// check the content of fiel using the ca command
	
	int fd1 = open("11_file2.txt", O_RDWR| O_CREAT, 0644);
	int dup2_fd1 = dup2(fd1, 25);
	printf("pid from dup2 :%d \n",dup2_fd1);

	int wr_sz2 = write(fd1 , "dup2-writting using the older fd\n", 33);

	int wr_sz3 = write(dup2_fd1 , "  dup2-writting using the new fd\n",33);

	// check the content of file using the cat command
	

	int fd2 = open("11_file3.txt", O_RDWR| O_CREAT, 0644);

	int fcntl_fd2 = fcntl(fd2, F_DUPFD);
	printf("pid from fcntl :%d \n",fcntl_fd2);

        int wr_sz4 = write(fd2 , "fcntl-writting using the older fd\n", 34);

        int wr_sz5 = write(fcntl_fd2 , "  fcntl-writting using the new fd\n",34);
	 // check the content of file using the cat command
	
}

/*
========================================================================================================
Output:

./a.out 
pid from dup :4 
pid from dup2 :25 
pid from fcntl :7 

cat 11_file1.txt 
dup-writting using the older fd
  dup-writtng using the new fd

cat 11_file2.txt 
dup2-writting using the older fd
  dup2-writting using the new fd

cat 11_file3.txt 
fcntl-writting using the older fd
  fcntl-writting using the new fd

========================================================================================================
*/
