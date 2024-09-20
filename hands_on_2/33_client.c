/*
========================================================================================================
Name : 33
Author : Aaradhya GHota
Description : Write a program to communicate between two machines using socket.
Date: 15 Sept, 2024.
========================================================================================================
*/

//Client side

#include <sys/types.h>  
#include <sys/socket.h> 
#include <netinet/ip.h> 
#include <stdio.h>      
#include <unistd.h>     

int main() {
	int stDesc = socket(AF_INET, SOCK_STREAM, 0);
	if (stDesc == -1) {
		perror("Error when creating socket");
	}
	printf("Socket created\n");

	struct sockaddr_in svrAddr;
	svrAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	svrAddr.sin_family = AF_INET;
	svrAddr.sin_port = htons(8080);

	int connecStat = connect(stDesc , (struct sockaddr *)&svrAddr, sizeof(svrAddr));

	if (connecStat == -1) {
		perror("Error while establishing connection");
	}

	printf("Connection with server established\n");

	char buffer[100];
	read(stDesc, buffer, 100);
	printf("Data from server: %s\n", buffer);

	printf("Write message for server: ");
	scanf("%[^\n]", buffer);

	write(stDesc, buffer, sizeof(buffer));
	printf("Data sent to server\n");

	close(stDesc);
	return 0;
}



/*
========================================================================================================
Output:

Terminal 1: 

./a.out
Server socket created
Binding to server socket successful
Listening for connections
Write message from server to client: 

Terminal 2:
./a.out
Socket created
Connection with server established

-----------------------------------
Terminal 1:
./a.out
Server socket created
Binding to server socket successful
Listening for connections
Write message from server to client: hi

TErminal 2:
./a.out
Socket created
Connection with server established
Data from server: hi
Write message for server: 


========================================================================================================
*/
