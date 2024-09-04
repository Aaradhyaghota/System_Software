/*
========================================================================================================

Question No: 7
Author: Aaradhya Ghota
Description: Write a program to copy file1 into file2 ($cp file1 file2).
Date: 16th August, 2024

========================================================================================================
*/


#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include<stdbool.h>

int main (int argc, char** argv){
	if(argc<3){
		printf("Error- Enter valid Input");
	}

	int fd = open(argv[1],O_RDWR);
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

/*
========================================================================================================
Output:

./a.out 7_file1.txt 7_file2.txt

7_file1.txt before copy 
cat 7_file1.txt 
But back to that “aha moment.” Park quickly called his friend and colleague Eric Friedman. In 2002, the two, both computer scientists by training, had started a photo-sharing company called HeyPix, which they sold to CNET in 2005. They were still working for CNET in 2006, but it wasn’t a bad time to think about doing something different.



After copy from 7_file1.txt to 7_file2.txt: 

cat 7_file1.txt 
But back to that “aha moment.” Park quickly called his friend and colleague Eric Friedman. In 2002, the two, both computer scientists by training, had started a photo-sharing company called HeyPix, which they sold to CNET in 2005. They were still working for CNET in 2006, but it wasn’t a bad time to think about doing something different.

cat 7_file2.txt 
But back to that “aha moment.” Park quickly called his friend and colleague Eric Friedman. In 2002, the two, both computer scientists by training, had started a photo-sharing company called HeyPix, which they sold to CNET in 2005. They were still working for CNET in 2006, but it wasn’t a bad time to think about doing something different. 


========================================================================================================
*/
