/*
========================================================================================================

Question No: 17a
Author: Aaradhya Ghota
Description: Write a program to simulate online ticket reservation. Implement write lock Write a program to open a file, store a ticket number and exit. Write a separate program, to open the file, implement write lock, read the ticket number, increment the number and print the new ticket number then close the file.
Date: 29th August, 2024

========================================================================================================
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
	struct{
		int ticket_no;
		
	}db;
	db.ticket_no = 10;
	int fd = open ("17db.txt",O_CREAT|O_RDWR, 0744);
        write(fd, &db, sizeof(db));
	close(fd);

	fd = open("17db.txt", O_RDONLY);
	read(fd, &db,sizeof(db));
	printf("Ticket no. %d \n",db.ticket_no);
	close(fd);
}

/*
========================================================================================================
Output:

./a.out
Ticket no. 10 

========================================================================================================
*/
