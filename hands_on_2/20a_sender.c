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
        char arr[10];
        printf("Enter the 10 character message to send to other program\n");
        scanf("%s",arr);
        if(mkfifo("20fifo", 0777) == -1){
                printf("Error in creating the fifo with mkfifo\n");
                exit(1);
        }
        printf("Waiting for reciever to recieve the message...\n");
        int fd = open("20fifo",O_WRONLY);
        if (fd == -1) {
                printf("Error in opening the fifo\n");
                exit(1);
        } 
        write(fd, arr , sizeof(arr)); 
        close(fd);
        
        printf("Message sent : %s \n",arr);
        return 0;

}

       

/*
========================================================================================================
Output:

./a.out
Enter the 10 character message to send to other program
Aaradhya
Waiting for receiver to receive the message...

//after receiver receiving the message

Message sent : Aaradhya 
========================================================================================================
*/

