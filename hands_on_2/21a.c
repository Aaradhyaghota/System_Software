/*
========================================================================================================
Name : 21
Author : Aaradhya Ghota
Description : Write two programs so that both can communicate by FIFO - Use two way communications.
Date: 14 Sept, 2024.
========================================================================================================
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(){
        char *arr= NULL;
        long int size = 100;
        char buf[100];
        if(mkfifo("21fifo1", 0777) == -1){
                perror("mkfifo1");
        }
        if(mkfifo("21fifo2", 0777) == -1){
                perror("mkfifo2");
        }
        while(1){
                printf("Enter the message to send to user2\n");
                int l = getline(&arr, &size, stdin);
                //printf("Waiting for reciever to recieve the message...\n");
                int fd1 = open("21fifo1",O_WRONLY);
                if(fd1 == -1){
                        printf("Error in opening the fifo\n");
                        exit(1);
                } 
                write(fd1, arr , l); 
                close(fd1);
        
                printf("Message sent : %s \n",arr);
                
                //
                
                int fd2 = open("21fifo2",O_RDONLY);
                if(fd2 == -1){
                        printf("Error in opening the fifo\n");
                        exit(1);
                } 
                int i=0;
                while (read(fd2, &buf[i], 1) > 0){i++;}
               
                close(fd2);
                
                printf("Message recieved from user2 : %s \n",buf);
                memset(arr, '\0', sizeof(arr));
        }
        free(arr);
        return 0;

}


/*
========================================================================================================
Output:


./a.out
mkfifo1: File exists
mkfifo2: File exists
Enter the message to send to user2
hi
Message sent : hi
 
Message recieved from user2 : hello
 
Enter the message to send to user2
my self aaradhya Ghota
Message sent : my self aaradhya Ghota
 
Message recieved from user2 : oh! Yes we at ronny party
 
Enter the message to send to user2


========================================================================================================
*/

