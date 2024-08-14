#include <stdio.h>
#include <fcntl.h>

int main(){
	
	int fd  = open("nwfile",O_CREAT | O_EXCL|O_RDWR, 0777);   //O_CREAT | O_EXCL -> opens function opens file only when is its not exist  
	if(fd == -1) printf("file not opended");
	else printf("File Opened");
return 0;
}

