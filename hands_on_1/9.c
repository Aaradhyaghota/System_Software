/*
========================================================================================================

Question No: 9
Author: Aaradhya Ghota
Description: Write a program to print the following information about a given file.
a. inode
b. number of hard links
c. uid
d. gid
e. size
f. block size
g. number of blocks
h. time of last access
i. time of last modification
j. time of last change
Date: 16th August, 2024

========================================================================================================
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <time.h>

int main(int argc, char *argv[]){
	struct stat file_info;

	if (argc != 2) {
        	printf("Error - Enter valid input");
		exit(EXIT_FAILURE);
	}

   	// Get file information using stat
    	if (stat(argv[1], &file_info) == -1) {
        	printf("Error in accessing the file");
        	exit(EXIT_FAILURE);
    	}

	printf("File: %s\n", argv[1]);
   	printf("Inode: %ld\n", file_info.st_ino);
	printf("Number of hard links: %ld\n", file_info.st_nlink);
    	printf("UID: %d\n", file_info.st_uid);
    	printf("GID: %d\n", file_info.st_gid);
    	printf("Size: %ld bytes\n", file_info.st_size);
    	printf("Block size: %ld bytes\n", file_info.st_blksize);
    	printf("Number of blocks: %ld\n", file_info.st_blocks);
    	printf("Time of last access: %s", ctime(&file_info.st_atime));
    	printf("Time of last modification: %s", ctime(&file_info.st_mtime));
    	printf("Time of last change: %s", ctime(&file_info.st_ctime));
	
	return 0;
}

/*
========================================================================================================
./a.out 8.txt
File: 8.txt
Inode: 8131528
Number of hard links: 1
UID: 1000
GID: 1000
Size: 510 bytes
Block size: 4096 bytes
Number of blocks: 8
Time of last access: Fri Aug 30 19:24:01 2024
Time of last modification: Wed Aug 28 18:31:13 2024
Time of last change: Fri Aug 30 19:23:21 2024



========================================================================================================
*/


