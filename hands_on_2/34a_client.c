/*
========================================================================================================
Name : 34.a
Author : Aaradhya GHota
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
		_exit(0);
	}
	printf("Socket created\n");

	struct sockaddr_in addr;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8080);

	int conn = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
	if (conn == -1) {
		perror("Error establishing connection\n");
		_exit(0);
	}
	printf("Connection with server established\n");

	char buf[100];
	read(sock, buf, 100);
	printf("Data from server: %s\n", buf);

	printf("Write message for server: \n");
	scanf("%[^\n]", buf);

	write(sock, buf, sizeof(buf));
	printf("Data sent to server\n");

	close(sock);
	return 0;
}


