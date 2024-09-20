/*
========================================================================================================
Name : 22
Author : Aaradhya GHota
Description : Write a program to wait for data to be written into FIFO within 10 seconds, use select system call with FIFO.
Date: 14 Sept, 2024.
========================================================================================================
*/

// this is code to write in the fifo

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

void main()
{
    if(mkfifo("22fifo", 0666) == -1 && errno != EEXIST) {
                perror("mkfifo");
                exit(1);
    }
    int fd = open("22fifo", O_WRONLY | O_NONBLOCK);
        if(fd == -1){
                  perror("open");
                  exit(1);
        }

    char *st = NULL;
    long int size=100;
    printf("Enter message\n");
    int l = getline(&st,&size,stdin);
    write(fd, st, l);
    close(fd);
}
