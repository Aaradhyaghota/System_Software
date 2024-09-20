/*
========================================================================================================
Name : 29
Author : Aaradhya Ghota
Description : Write a program to remove the message queue.
Date: 12 Sept, 2024.
========================================================================================================
*/

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

int main(){

        key_t k = ftok("queuefile", 1);
        printf("Key: %x\n", k);
        int mqid = msgget(k, 0666 | IPC_CREAT);
        printf("Message Queue ID: %d\n", mqid);
                
        if (msgctl(mqid, IPC_RMID, NULL) == -1) {
                perror("msgctl");
                exit(1);
        }

        printf("Message queue removed successfully.\n");

        return 0;
}


/*
========================================================================================================
Output:

ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x01068d52 0          aaradhya-g 777        0            0           
0x01068daa 1          aaradhya-g 666        100          1         


./a.out
Key: 1068daa
Message Queue ID: 1
Message queue removed successfully.

ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x01068d52 0          aaradhya-g 777        0            0           

========================================================================================================
*/
