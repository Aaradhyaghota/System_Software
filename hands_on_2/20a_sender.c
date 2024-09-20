/*
========================================================================================================
Name : 20
Author : Aaradhya GHota
Description : Write two programs so that both can communicate by FIFO - Use one way communication.
Date: 14 Sept, 2024.
========================================================================================================
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
        char *arr= NULL;
        long int size = 100;
        if(mkfifo("20fifo", 0777) == -1){
                perror("mkfifo");     
        }
        while(1){
                printf("Enter the message to send to other program\n");
                int l = getline(&arr, &size, stdin);
                //printf("Waiting for reciever to recieve the message...\n");
                int fd = open("20fifo",O_WRONLY);
                if(fd == -1){
                        printf("Error in opening the fifo\n");
                        exit(1);
                } 
                write(fd, arr , l); 
                close(fd);
        
                printf("Message sent : %s \n",arr);
                
        }
        free(arr);
        return 0;

}

       

/*
========================================================================================================
Output:

./a.out
mkfifo: File exists

Enter the message to send to other program
Hello myself aaradhya ghota

Message sent : Hello myself aaradhya ghota
 
Enter the message to send to other program
How are you?

Message sent : How are you?
 
Enter the message to send to other program


========================================================================================================
*/

