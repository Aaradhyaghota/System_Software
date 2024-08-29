#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
//#include <stdlib.h>

int main() {
	struct flock lk;
	int fd = open("16db",O_RDWR);

	lk.l_type = F_WRLCK;
	lk.l_whence = SEEK_SET;
	lk.l_start = 0;
	lk.l_len = 0;
	lk.l_pid = getpid();
	
	printf("Writer want to enter \n");
	printf("Before entering into critical section\n");

	fcntl(fd,F_SETLKW, &lk);

	printf("Writer - Inside the critical section \n");
	printf("Enter to unlock\n");
	getchar();
	
	printf("Unlocked\n");
	lk.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lk);

	printf("Finished\n");

}
