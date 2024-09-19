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
        
        int fd = open("20fifo",O_RDONLY);
        if (fd == -1) {
                printf("Error in opening the fifo\n");
                exit(1);
        } 
        
        int n = read(fd, arr , 10);
        if (n == -1){
                printf("Error in reading from fifo");
                exit(1);
        }
        close(fd);
        
        printf("Message recieved : %s \n",arr);
        return 0;

}


/*
========================================================================================================
Output:

./a.out
Message recieved : Aaradhya 

========================================================================================================
*/

