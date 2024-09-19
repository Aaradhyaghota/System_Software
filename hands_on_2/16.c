
/*
========================================================================================================
Name : 16
Author : Aaradhya Ghota
Description : Write a program to send and receive data from parent to child vice versa. Use two way communication.
Date: 13 Sept, 2024.
========================================================================================================
*/


#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(){
	int fd_pc[2];
	int fd_cp[2];
	
	if (pipe(fd_pc) == -1 || pipe(fd_cp) == -1) {
                printf("Pipe creation failed");
                exit(1);
        }


	if(!fork()){
	        //child
		// Child to parent send.
		int c_sent= 101;
		printf("Child : Data sended - %d\n", c_sent);
		close(fd_cp[0]);
		write(fd_cp[1], &c_sent, sizeof(int));
		close(fd_cp[1]);
		
		// Parent to child receive.
		int c_recieve;
		close(fd_pc[1]);
		read(fd_pc[0], &c_recieve, sizeof(int));
		close(fd_pc[0]);
		printf("Child : Data from parent to child: %d\n", c_recieve);
		
		
	}
	else{
	        //parent
		// Child to parent receive.
		int p_recieve;
		close(fd_cp[1]);
		read(fd_cp[0], &p_recieve, sizeof(int));
		close(fd_cp[0]);
		printf("Parent : Data from child to parent: %d\n", p_recieve);
	
		// Parent to child send.
		int p_sent = 201;
		printf("Parent : Data sended -  %d\n", p_sent);
		close(fd_pc[0]);
		write(fd_pc[1], &p_sent, sizeof(int));
		close(fd_pc[1]);
	}

	return 0;
}

/*
========================================================================================================
Output:

 ./a.out
Child : Data sended - 101
Parent : Data from child to parent: 101
Parent : Data sended -  201
Child : Data from parent to child: 201

========================================================================================================
*/
