#include <stdio.h>
#include <unistd.h>

int main(){
	char buff[20];
	// ssize_t read(int fd, void buf[.count], size_t count);
	// fd - file discripter (0- keyboard , 1- screen)  buf- uffer n which input is stored
	// and count will store the lenght of char input readed by function.
	
	int size = read(0,buff,20);
	if(size != -1){printf("%d byte was readed.\n", size);}
	else {printf("--Error--\nInput failed\n");}

	int size2 = write(1,buff,20);
	if(size2 == -1)printf("writting failed\n");
	//else printf("output not Printed\n");


return 0;
}
