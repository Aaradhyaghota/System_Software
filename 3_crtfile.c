#include <stdio.h>
#include <fcntl.h>

int main(int argc, char** argv){
	//      int creat(const char *pathname, mode_t mode);
	//
	//
	int fd  = creat(argv[1], 0774);
	if(fd>0)printf("Succesful\n");
	else printf("Unsuccesful\n");
	printf("File discripter value is %d \n", fd);


return 0;
}

