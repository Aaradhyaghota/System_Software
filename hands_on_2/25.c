
/*
========================================================================================================
Name : 25
Author : Aaradhya ghota
Description : Write a program to print a message queue's (use msqid_ds and ipc_perm structures):
		a. access permission
		b. uid, gid
		c. time of last message sent and received
		d. time of last change in the message queue
		e. size of the queue
		f. number of messages in the queue
		g. maximum number of bytes allowed
		h. pid of the msgsnd and msgrcv
Date: 15 Sept, 2024.
========================================================================================================
*/

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <time.h>

int main(){
        key_t key;
        int mqid;

        key = ftok(".", 1);
        if(key == -1){
                perror("ftok");
                exit(1);
        }

        mqid = msgget(key, 0666 | IPC_CREAT);
        if(mqid == -1){
                perror("msgget");
                exit(1);
        }

        struct msqid_ds buf;
      
        if(msgctl(mqid, IPC_STAT, &buf) == -1){
                perror("msgctl");
                exit(1);
        }

        printf("Access Permissions: %o\n", buf.msg_perm.mode);
        printf("Owner UID: %u\n", buf.msg_perm.uid);
        printf("Owner GID: %u\n", buf.msg_perm.gid);
        printf("Last Message Sent: %s", ctime(&buf.msg_stime));
        printf("Last Message Received: %s", ctime(&buf.msg_rtime));
        printf("Last Change: %s", ctime(&buf.msg_ctime));
        printf("Current size of Queue(bytes): %lu\n", buf.__msg_cbytes);
        printf("Number of Messages in Queue: %lu\n", buf.msg_qnum);
        printf("Max Bytes Allowed in Queue: %lu\n", buf.msg_qbytes);
        printf("PID of Last msgsnd: %d\n", buf.msg_lspid);
        printf("PID of Last msgrcv: %d\n", buf.msg_lrpid);

        return 0;
}


/*
========================================================================================================
Output:
./a.out
Access Permissions: 666
Owner UID: 1000
Owner GID: 1000
Last Message Sent: Thu Jan  1 05:30:00 1970
Last Message Received: Thu Jan  1 05:30:00 1970
Last Change: Fri Sep 20 11:24:05 2024
Current size of Queue(bytes): 0
Number of Messages in Queue: 0
Max Bytes Allowed in Queue: 16384
PID of Last msgsnd: 0
PID of Last msgrcv: 0

========================================================================================================
*/
