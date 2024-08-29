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
