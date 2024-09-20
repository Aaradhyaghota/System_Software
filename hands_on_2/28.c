/*
========================================================================================================
Name : 28
Author : Aaradhya Ghota
Description : Write a program to change the existing message queue permission (use msqid_ds structure).
Date: 15 Sept, 2024.
========================================================================================================
*/
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

int main(){
	key_t k = ftok(".", 1);
        if(k == -1){
                perror("ftok");
                exit(1);
        }

        int mqid = msgget(k, 0666 | IPC_CREAT);
        if(mqid == -1){
                perror("msgget");
                exit(1);
        }
        
	struct msqid_ds buf;
      
	msgctl(mqid, IPC_STAT, &buf);
	printf("Current permissions: %o\n", buf.msg_perm.mode);

	buf.msg_perm.mode = 0777;
        
        // Apply the changes using IPC_SET
	msgctl(mqid, IPC_SET, &buf);
	
	msgctl(mqid, IPC_STAT, &buf);
	printf("Updated permissions: %o\n", buf.msg_perm.mode);

	return 0;
}

/*
========================================================================================================
Output:

./a.out
Current permissions: 666
Updated permissions: 777

========================================================================================================
*/
