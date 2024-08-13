#include <stdio.h>
#include <fcntl.h>

int main(){
	//      int creat(const char *pathname, mode_t mode);
	//
	//
	int fd  = creat("newfile", 0774);
	if(fd>0)printf("Succesful\n");
	else printf("Unsuccesful\n");
	printf("%d \n", fd);


return 0;
}

