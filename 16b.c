/*
========================================================================================================

Question No: 16b
Author: Aaradhya Ghota
Description: Write a program to perform mandatory locking.
	a. Implement write lock
	b. Implement read lock
Date: 29th August, 2024

========================================================================================================
*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
//#include <stdlib.h>

int main() {
        struct flock lk;
	int fd = open("16db", O_RDWR);
        lk.l_type = F_RDLCK;
        lk.l_whence = SEEK_SET;
        lk.l_start = 0;
        lk.l_len = 0;
        lk.l_pid = getpid();

	
        printf("Reader acquiring lock\n");

        fcntl(fd,F_SETLKW, &lk);

        printf("Reader - Inside the critical section\n");
        printf("Enter to unlock\n");
        getchar();

        printf("Unlocked\n");
        lk.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lk);

        printf("Finished\n");

}

/*
========================================================================================================
Output:

gcc -o 16reader 16b.c
gcc -o 16writer 16a.c

terminal 1:
./16reader 
Reader acquiring lock
Reader - Inside the critical section
Enter to unlock


terminal 2:
./16reader 
Reader acquiring lock
Reader - Inside the critical section
Enter to unlock



terminal 3:
./16writer
Acquiring Write lock


========================================================================================================
*/

