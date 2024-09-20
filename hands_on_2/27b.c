/*
========================================================================================================
Name : 27.b
Author : Aaradhya Ghota
Description : Write a program to receive messages from the message queue.
		b. With IPC_NOWAIT as a flag
Date: 15 Sept, 2024.
========================================================================================================
*/

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>

struct msg_buffer{
        long mtype;
        char mtext[100];
};

int main(){

        key_t k = ftok(".", 1);
        if(k == -1){
                perror("ftok");
                exit(1);
        }
        printf("Key: %x\n", k);

        int mqid = msgget(k, 0666 | IPC_CREAT);
        if(mqid == -1){
                perror("msgget");
                exit(1);
        }
        printf("Message Queue ID: %d\n", mqid);
        
        struct msg_buffer msg;
        
        // // Receive a message from the queue (this will NOT block, and will return immediately if no message is available)
        // Attempt to receive a message of type 1 using IPC_NOWAIT to avoid blocking
        if(msgrcv(mqid, &msg, sizeof(msg.mtext), 1, IPC_NOWAIT) == -1){
                perror("msgrcv");
                exit(1);
        }

        printf("Message recieved : %s\n", msg.mtext);

        return 0;
}

/*
========================================================================================================
Output:

ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x01068d52 0          aaradhya-g 666        100          1           
0x01068daa 1          aaradhya-g 666        100          1       

./a.out
Key: 1068d52
Message Queue ID: 0
Message recieved : hello

ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x01068d52 0          aaradhya-g 666        0            0           
0x01068daa 1          aaradhya-g 666        100          1           

./a.out
Key: 1068d52
Message Queue ID: 0
msgrcv: No message of desired type

========================================================================================================
*/
