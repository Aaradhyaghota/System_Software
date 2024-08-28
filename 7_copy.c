#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
int main (int argc, char** argv){
	if(argc<3){
		printf("Error- Enter valid Iput");
	}

	int fd = open(argv[1],O_RDWR, 0777);
	if(fd == -1)printf("Error in opening the file '%s' ", argv[1]);

	int fd1 = open(argv[2],O_CREAT | O_RDWR , 0777);
        if(fd1 == -1)printf("Copyfile Not created");
        
	char buff[100];
	int index=0;
	while(true){
		int inputSize = read(fd, &buff[index], 1);
		if(inputSize == -1)printf("Error is reading the file");

		if(inputSize == 0){
			write(fd1, buff, index);  
			break;
		}
		
		if(index==99){
			write(fd1, buff, 100);
			index = 0;
			continue;
		}
		index++;

	}
	
	//you have to close the file buffer so that progrma can terminate.
	close(fd);
	close(fd1);
         
return 0;
}
