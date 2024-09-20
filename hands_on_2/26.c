
/*
========================================================================================================
Name : 26
Author : Aaradhya Ghota
Description : Write a program to send messages to the message queue. Check $ipcs -q.
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

        key_t k = ftok("queuefile", 1);
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
        
        msg.mtype = 1;
        printf("Enter a message to send: ");
        fgets(msg.mtext, 100 , stdin);
        msg.mtext[strcspn(msg.mtext, "\n")] = '\0';  // Remove newline character

        // Send the message to the message queue
        if(msgsnd(mqid, &msg, sizeof(msg.mtext), 0) == -1){
                perror("msgsnd");
                exit(1);
        }

        printf("Message sent: %s\n", msg.mtext);

        return 0;
}


/*
========================================================================================================
Output:

./a.out
Key: 1068daa
Message Queue ID: 1
Enter a message to send: hello
Message sent: hello




ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x01068d52 0          aaradhya-g 666        100          1            // ftok(".", 1);
0x01068daa 1          aaradhya-g 666        100          1           


========================================================================================================
*/
