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
#include <string.h>


int main(){
        char arr[100];
        while(1){
                int fd = open("20fifo",O_RDONLY);
                if (fd == -1) {
                        printf("Error in opening the fifo\n");
                        exit(1);
                } 
                
                char buf;
                int i=0;
                while (read(fd, &arr[i], 1) > 0){i++;}
               
                close(fd);
                
                printf("Message recieved : %s \n",arr);
                memset(arr, '\0', sizeof(arr));
        }
        return 0;

}


/*
========================================================================================================
Output:

./a.out
Message recieved : Hello myself aaradhya ghota
 
Message recieved : How are you?
 



========================================================================================================
*/

