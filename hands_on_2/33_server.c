/*
========================================================================================================
Name : 33
Author : Aaradhya GHota
Description : Write a program to communicate between two machines using socket.
Date: 12 Sept, 2024.
========================================================================================================
*/
//Server side 

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1){
		perror("Error creating socket");
	}
	printf("Server socket created\n");

	struct sockaddr_in srv, cl;
	srv.sin_addr.s_addr = htonl(INADDR_ANY);
	srv.sin_family = AF_INET;
	srv.sin_port = htons(8080);

	int b = bind(sock, (struct sockaddr *)&srv, sizeof(srv));
	if(b == -1){
		perror("Error binding socket");
		exit(0);
	}
	printf("Binding to server socket successful\n");

	int l = listen(sock, 2);
	if(l == -1){
		perror("Error listening for connections");
		exit(0);
	}
	printf("Listening for connections\n");

	int cl_size = sizeof(cl);
	int connfd = accept(sock, (struct sockaddr *)&cl, &cl_size);
	if(connfd == -1){
		perror("Error accepting connection");
		_exit(0);
	} 
	else{
		char buf[100];
		printf("Write message from server to client: ");
		scanf("%[^\n]", buf);

		write(connfd, buf, sizeof(buf));
		read(connfd, buf, 100);
		printf("Data from client: %s\n", buf);
	}

	close(sock);
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
