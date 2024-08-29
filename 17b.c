#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
//#include <stdlib.h>

int main() {
	//Struct to store the ticket no readed from the file 17db.txt
	struct{
		int ticket_value;
	}tk;
        
        int fd = open("17db.txt",O_RDWR);

	struct flock lk;
        lk.l_type = F_WRLCK;
        lk.l_whence = SEEK_SET;
        lk.l_start = 0;
        lk.l_len = 0;
        lk.l_pid = getpid();

	printf("Accuiring lock\n");
	//locking
        fcntl(fd,F_SETLKW, &lk);
	//reading the database after locking because other process should not
	// read the db before one process is updating.
	read(fd,&tk,sizeof(tk));// reading from db of 17db.txt to struct tk  
    	
	//
	printf("Current ticket number : %d \n",tk.ticket_value);

        printf("Inside the critical section- Updating the ticket no. value \n");
        tk.ticket_value++;
	lseek(fd,0,SEEK_SET);// setting the lseek to start of struct tk
	write(fd,&tk,sizeof(tk));//writting from struct tk to 17db.txt struct db 

	printf("Enter to unlock\n");
        getchar();

        lk.l_type = F_UNLCK;
	printf("Unlocked\n");
        fcntl(fd, F_SETLK, &lk);

	printf("Updated ticket number %d \n",tk.ticket_value);

}

