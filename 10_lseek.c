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
