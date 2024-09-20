/*
========================================================================================================
Name : 21
Author : Prajyot Shende
Description : Write two programs so that both can communicate by FIFO - Use two way communications.
Date: 12 Sept, 2024.
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
        
        while(1){
                int fd2 = open("21fifo1",O_RDONLY);
                if(fd2 == -1){
                        printf("Error in opening the fifo\n");
                        exit(1);
                } 
                int i=0;
                while (read(fd2, &buf[i], 1) > 0){i++;}
               
                close(fd2);
                
                printf("Message recieved by user2 from user1: %s \n",buf);
                memset(buf, '\0', sizeof(buf));
        
        
        
                printf("Enter the message to send to user1 :\n");
                int l = getline(&arr, &size, stdin);
                //printf("Waiting for reciever to recieve the message...\n");
                int fd1 = open("21fifo2",O_WRONLY);
                if(fd1 == -1){
                        printf("Error in opening the fifo\n");
                        exit(1);
                } 
                write(fd1, arr , l); 
                close(fd1);
        
                printf("Message sent to user1: %s \n",arr);
                    
        }
        free(arr);
        return 0;

}

/*
========================================================================================================
Output:

./a.out
Message recieved by user2 from user1: hi
 
Enter the message to send to user1 :
hello
Message sent to user1: hello
 
Message recieved by user2 from user1: my self aaradhya Ghota
 
Enter the message to send to user1 :
oh! Yes we at ronny party
Message sent to user1: oh! Yes we at ronny party
 

========================================================================================================
*/

