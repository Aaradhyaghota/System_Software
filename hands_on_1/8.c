/*
========================================================================================================

Question No: 8
Author: Aaradhya Ghota
Description: Write a program to open a file in read only mode, read line by line and display each line as it is read. Close the file when end of file is reached.
Date: 16th August, 2024

========================================================================================================
*/



#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h> // for using true in while(true)

int main(int argc, char **argv){

	if(argc < 2)
	{
		printf("Error - give valid Input");
	}	
	int fd = open(argv[1], O_RDONLY);
	if(fd == -1 ){
		printf ("Error in opening the file'%s' ", argv[1]);
		return 0;
	}

	char buff[100];
	int ch;
	int idx=0;
	while(true){
		ch = read(fd, &buff[idx], 1);
		if(ch<0){
			printf("error reading form the file");
		}

		if(ch == 0)break;

		if(buff[idx]=='\n'){
			write(1,buff,idx+1);
			getchar();
			idx=0;
			continue;

		}
		idx++;
	}

	close(fd);
	
	return 0;
}

/*
========================================================================================================
Output:

cat 8.txt
Twenty-nine-year-old entrepreneur James Park had just purchased a Wii game system.
It included the Wii Nunchuk, a US $29 handheld controller with motion sensors that 
let game players interact by moving their bodies—swinging at a baseball, say, 
or boxing with a virtual partner.
Park became obsessed with his Wii.
“I was a tech-gadget geek,” he says.
“Anyone holding that nunchuk was fascinated by how it worked.
It was the first time that I had seen a compelling consumer use for accelerometers.”





./a.out 8.txt
Twenty-nine-year-old entrepreneur James Park had just purchased a Wii game system.

It included the Wii Nunchuk, a US $29 handheld controller with motion sensors that 

let game players interact by moving their bodies—swinging at a baseball, say, 

or boxing with a virtual partner.

Park became obsessed with his Wii.

“I was a tech-gadget geek,” he says.

“Anyone holding that nunchuk was fascinated by how it worked.

It was the first time that I had seen a compelling consumer use for accelerometers.”



========================================================================================================
*/
