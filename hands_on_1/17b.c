/*
========================================================================================================

Question No: 17b
Author: Aaradhya Ghota
Description: Write a program to simulate online ticket reservation. Implement write lock Write a program to open a file, store a ticket number and exit. Write a separate program, to open the file, implement write lock, read the ticket number, increment the number and print the new ticket number then close the file.
Date: 29th August, 2024

========================================================================================================
*/
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
        lk.l_len = 0;//length = 0 takes the full length of file
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

/*
========================================================================================================
Output:

Terminal 1:
./17upd_tk
Accuiring lock
Current ticket number : 10 
Inside the critical section- Updating the ticket no. value 
Enter to unlock

Terminal 2:
./17upd_tk 
Accuiring lock

Terminal3:
./17upd_tk 
Accuiring lock

>>after 1st person exit

Terminal 1:
Accuiring lock
Current ticket number : 10 
Inside the critical section- Updating the ticket no. value 
Enter to unlock

Unlocked
Updated ticket number 11 

Terminal 2:
./17upd_tk 
Accuiring lock
Current ticket number : 11 
Inside the critical section- Updating the ticket no. value 
Enter to unlock


Terminal3:
./17upd_tk 
Accuiring lock


>>after 2st person exit

Terminal 1:
Accuiring lock
Current ticket number : 10 
Inside the critical section- Updating the ticket no. value 
Enter to unlock

Unlocked
Updated ticket number 11 

Terminal 2:
 ./17upd_tk 
Accuiring lock
Current ticket number : 11 
Inside the critical section- Updating the ticket no. value 
Enter to unlock

Unlocked
Updated ticket number 12 



Terminal3:
./17upd_tk 
Accuiring lock
Current ticket number : 12 
Inside the critical section- Updating the ticket no. value 
Enter to unlock

========================================================================================================
*/


