#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

int main(int argc, char **argv){

	if(argc < 2)
	{
		printf("Error - give valid Input");
	}	
	int fd = open(argv[1], O_RDONLY);
	if(fd == -1 ){
		printf ("Error in opening the file'%s' ", argv[1]);
		return 0;
	}

	char buff[100];
	int ch;
	int idx=0;
	while(true){
		ch = read(fd, &buff[idx], 1);
		if(ch<0){
			printf("error reading form the file");
		}

		if(ch == 0)break;

		if(buff[idx]=='\n'){
			write(1,buff,idx+1);
			getchar();
			idx=0;
			continue;

		}
		idx++;
	}

	close(fd);
	
	return 0;
}
