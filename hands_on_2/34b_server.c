/*
========================================================================================================
Name : 34.b
Author : Aaradhya Ghota
Description : Write a program to create a concurrent server using pthread_create.
Date: 15 Sept, 2024.
========================================================================================================
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

void comm(int *conn) {
	char buf[100];
	write(*conn, "Hello from server\n", 19);
	read(*conn, buf, 100);
	printf("Data from client: %s\n", buf);
	close(*conn);
}

int main() {
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		perror("Error creating socket");
	}

	printf("Server socket created\n");

	struct sockaddr_in serv, cli;
	serv.sin_addr.s_addr = htonl(INADDR_ANY);
	serv.sin_family = AF_INET;
	serv.sin_port = htons(8080);

	int bindS = bind(sock, (struct sockaddr *)&serv, sizeof(serv));
	if (bindS == -1){
		perror("Error binding socket");
		exit(0);
	}
	printf("Binding successful\n");

	int listenS = listen(sock, 2);
	if (listenS == -1){
		perror("Error listening for connections");
		exit(0);
	}
	printf("Listening for connections\n");

	while (1) {
		int cli_size = sizeof(cli);
		int connfd = accept(sock, (struct sockaddr *)&cli, &cli_size);
		if (connfd == -1) {
			perror("Error accepting connection\n");
			exit(0);
		} 
		else{
			pthread_t tID;
			if (pthread_create(&tID, NULL, (void *)comm, &connfd)) {
				perror("Error creating thread");
			}
		}
	}

	close(sock);
	return 0;
}



/*
========================================================================================================
Output:


Server::

./a.out
Server socket created
Binding successful
Listening for connections
Data from client: hi
Data from client: hello


Client 1::

./a.out
Socket created
Connection established
Data from server: Hello from server

Write message for server: 
hi
Data sent to server


Clent 2:

./a.out
Socket created
Connection established
Data from server: Hello from server

Write message for server: 
hello
Data sent to server


========================================================================================================
*/

