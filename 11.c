#include <stdio.h>
#include <fcntl.h>   /* Definition of O_* constants */
#include <unistd.h>

int main(){
	int fd = open("11_file1.txt",O_RDWR| O_CREAT, 0644);
	int dup_fd = dup(fd);
	
	int wr_sz = write(fd , "dup-writting using the older fd\n",32);

	int wr_sz1 = write(dup_fd , "  dup-writtng using the new fd\n",32);
	
	// check the content of fiel using the ca command
	
	int fd1 = open("11_file2.txt", O_RDWR| O_CREAT, 0644);
	int dup2_fd1 = dup2(fd1, 25);

	int wr_sz2 = write(fd1 , "dup2-writting using the older fd\n", 33);

	int wr_sz3 = write(dup2_fd1 , "  dup2-writting using the new fd\n",33);

	// check the content of file using the cat command
	

	int fd2 = open("11_file3.txt", O_RDWR| O_CREAT, 0644);

	int fcntl_fd2 = fcntl(fd2, F_DUPFD);

        int wr_sz4 = write(fd2 , "fcntl-writting using the older fd\n", 34);

        int wr_sz5 = write(fcntl_fd2 , "  fcntl-writting using the new fd\n",34);
	 // check the content of file using the cat command

	
		


}
