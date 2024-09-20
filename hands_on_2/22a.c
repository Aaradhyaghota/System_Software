/*
========================================================================================================
Name : 22
Author : Aaradhya GHota
Description : Write a program to wait for data to be written into FIFO within 10 seconds, use select system call with FIFO.
Date: 14 Sept, 2024.
========================================================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/time.h>
#include <errno.h>

int main(){
        
            if (mkfifo("22fifo", 0666) == -1 && errno != EEXIST) {
                    perror("mkfifo");
                    exit(1);
            } 

            printf("10 second timer will start when writer is available.\n");

            int fd = open("22fifo", O_RDONLY | O_NONBLOCK);
            if(fd == -1){
                      perror("open");
                      exit(1);
            }
            printf("Starting 10 second timer.\n");
            
            fd_set rfds;
            struct timeval tv;
            
            FD_ZERO(&rfds); // Clear the set
            FD_SET(fd, &rfds); // Add fd to the set

            tv.tv_sec = 10;
            tv.tv_usec = 0;

            printf("Waiting...\n");
            int status = select(fd + 1, &rfds, NULL, NULL, &tv);
            printf("Waiting Over\n");

            if (status == -1){
                      perror("select()");
            }
            else if (status){
                      if(FD_ISSET(fd, &rfds)){
                      printf("Data is available now.\n");
                      char buf[100];
                      read(fd, buf, sizeof(buf));
                      printf("Message from writer: %s", buf);
                      }
            }
            else{
                      printf("No data within ten seconds.\n");
            }

            close(fd);
            return 0;
}

/*
========================================================================================================
Output:

Terminal 1: 
./a.out
10 second timer will start when writer is available.
Starting 10 second timer.
Waiting...
Waiting Over
Data is available now.
Message from writer: hello

terminal 2: 
./a.out
Enter message
hello

//----------------------
Terminal 1:
./a.out
10 second timer will start when writer is available.
Starting 10 second timer.
Waiting...
//After 10 sec 
Waiting Over
No data within ten seconds.

========================================================================================================
*/
