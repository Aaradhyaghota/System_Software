/*
========================================================================================================
Name : 34.a
Author : Aaradhya Ghota
Description : Write a program to create a concurrent server using fork.
Date: 15 Sept, 2024.
========================================================================================================
*/


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <unistd.h>

int main() {
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		perror("Error creating socket");
	}
	printf("Server socket created\n");

	struct sockaddr_in srv, cl;
	srv.sin_addr.s_addr = htonl(INADDR_ANY);
	srv.sin_family = AF_INET;
	srv.sin_port = htons(8080);

	int b = bind(sock, (struct sockaddr *)&srv, sizeof(srv));
	if (b == -1) {
		perror("Error binding socket");
		_exit(0);
	}
	printf("Binding to server socket successful\n");

	int l = listen(sock, 3);
	if (l == -1) {
		perror("Error listening for connections");
		_exit(0);
	}
	printf("Listening for connections\n");

	while (1) {
		int cl_size = sizeof(cl);
		int connfd = accept(sock, (struct sockaddr *)&cl, &cl_size);
		if (connfd == -1) {
			perror("Error accepting connection\n");
			_exit(0);
		} else {
			if (fork() == 0) {
				char buf[100];
				printf("Write message from server to client: \n");
				scanf("%[^\n]", buf);

				write(connfd, buf, sizeof(buf));
				read(connfd, buf, 100);
				printf("Data from client: %s\n", buf);
			} else {
				close(connfd);
			}
		}
	}

	close(sock);
	return 0;
}



/*


Server::

./a.out
Server socket created
Binding to server socket successful
Listening for connections
Write message from server to client: 
hi to client 1
Data from client: hello from client 1
Write message from server to client: 
hi to client 2
Data from client: hello from client 2


Client 1:
./a.out
Socket created
Connection with server established
Data from server: hi to client 1
Write message for server: 
hello from client 1
Data sent to server

Client 2:
./a.out
Socket created
Connection with server established
Data from server: hi to client 2
Write message for server: 
hello from client 2
Data sent to server

*/
