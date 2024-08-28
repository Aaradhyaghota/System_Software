#include <stdio.h>
#include <fcntl.h>

int main (int argc, char ** argv){
	if(argc !=2){
		printf("Error - invalid INput\n");
		return 0;
	}
	int fd = open(argv[1],O_RDWR | O_CREAT , 0644);
       	if( fd == -1 ){ printf("Error\n"); return 0;}

	int status_flag = fcntl(fd, F_GETFL);
	if (status_flag == -1){ printf("Error\n"); return 0;}

	int acc_md = status_flag & O_ACCMODE;

	if(acc_md == 0)
	    printf("The file is opened in read-only mode.\n");
        else if(acc_md == 1)
            printf("The file is opened in write-only mode.\n");
        else if(acc_md == 2)
            printf("The file is opened in read-write mode.\n");
    	else
            printf("Unknown file mode.\n");    
}
