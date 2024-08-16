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
