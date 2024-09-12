/*
========================================================================================================

Question No: 14
Author: Aaradhya Ghota
Description: Write a program to find the type of a file.
	a. Input should be taken from command line.
	b. program should be able to identify any type of a file.
Date: 29th August, 2024

========================================================================================================
*/
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char** argv){
	if(argc != 2){
		printf("Error - Invalid input");
		return 0;
	}	
	struct stat filetype;
	int a = lstat(argv[1], &filetype);
	if(a == -1){
               	printf("Error\n");
		return 0;
        }

	 printf("File : ");

         switch (filetype.st_mode & S_IFMT) {
           case S_IFBLK:
		   printf("%s  is a block device\n",argv[1]);
       		   break;
           case S_IFCHR:
		   printf("%s  is a character device\n",argv[1]);   
	      	   break;
           case S_IFDIR: 
		   printf("%s  is a directory\n",argv[1]);
    		   break;
           case S_IFIFO: 
		   printf("%s  is a FIFO/pipe\n",argv[1]); 
     		   break;
           case S_IFLNK: 
		   printf("%s  is a symlink\n",argv[1]);  
    		   break;
           case S_IFREG:  
		   printf("%s  is a regular file\n",argv[1]); 
		   break;
           case S_IFSOCK:
		   printf("%s  is a socket\n",argv[1]);   
    		   break;
           default:
	    	   printf("%s  is a unknown?\n",argv[1]); 
    		   break;
         }

}

/*
========================================================================================================
Output:

./a.out 8.txt
File : 8.txt  is a regular file

./a.out linkto1a
File : linkto1a  is a symlink

./a.out 20priority 
File : 20priority  is a regular file


========================================================================================================
*/
