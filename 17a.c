
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
	struct{
		int ticket_no;
		
	}db;
	db.ticket_no = 0;
	int fd = open ("17db.txt",O_CREAT|O_RDWR, 0744);
        write(fd, &db, sizeof(db));
	close(fd);

	fd = open("17db.txt", O_RDONLY);
	read(fd, &db,sizeof(db));
	printf("Ticket no. %d \n",db.ticket_no);
	close(fd);
}
