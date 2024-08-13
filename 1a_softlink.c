#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(){
	int flag = symlink("1a_softlink.c","linkto1a");
	if(flag == 0){printf("Successful \n");}
	else {printf("Unsuccessful \n");}
return 0; 	
}
