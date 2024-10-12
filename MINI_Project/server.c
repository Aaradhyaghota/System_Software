#include <errno.h>
#include <fcntl.h>
#include <netinet/ip.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "./functions/adminF.h"
#include "./functions/commonF.h"
#include "./functions/customerF.h"
#include "./functions/employeeF.h"
// #include "./CUSTOMER.h"

void connection_handler(int connFd) {
    printf("Client is connected to the server!\n");
    char rBuffer[1000], wBuffer[1000];
    ssize_t rBytes, wBytes;

    wBytes = write(
        connFd,
        "Welcome to bank!\nSelect user\n1. Customer\t2. Bank Employee\t3. "
        "Manager\t4. Administrator\nPress any other number to exit\nEnter "
        "the number corresponding to the choice!",
        strlen(
            "Welcome to bank!\nselect user\n1. Customer\t2. Bank Employee\t3. "
            "Manager\t4. Administrator\nPress any other number to exit\nEnter "
            "the number corresponding to the choice!"));

    if (wBytes == -1)
        perror("Error while sending message to the user!");
    else {
        bzero(rBuffer, sizeof(rBuffer));
        rBytes = read(connFd, rBuffer, sizeof(rBuffer));
        if (rBytes == -1)
            perror("Error while reading from client");
        else if (rBytes == 0)
            printf("No data was sent by the client");
        else {
            int choice = atoi(rBuffer);
            switch (choice) {
                case 1:
                    // Customer
                    printf("customer\n");
                    customer_operation(connFd);
                    break;
                case 2:
                    // Bank Employee
                    printf("Employee trying to login!\n");
                    printf("emplyoee\n");
                    employee_operation(connFd);
                    break;
                case 3:
                    // Manager
                    printf("manager\n");
                    // manager_operation(connFd);
                    break;
                case 4:
                    // Administrator
                    printf("Admin trying to login!\n");
                    admin_operation(connFd);
                    break;
                default:
                    // Exit
                    break;
            }
        }
    }
    printf("Terminating connection to client!\n");
}

void main() {
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd == -1) {
        perror("Error while creating server socket!");
        _exit(0);
    }

    struct sockaddr_in serverAddress, clientAddress;
    serverAddress.sin_family = AF_INET;    // IPv4
    serverAddress.sin_port = htons(8080);  // Server will listen to port 8080
    serverAddress.sin_addr.s_addr =
        htonl(INADDR_ANY);  // Binds the socket to all interfaces

    int socketBindStatus = bind(socketFd, (struct sockaddr *)&serverAddress,
                                sizeof(serverAddress));
    if (socketBindStatus == -1) {
        perror("Error while binding to server socket!");
        _exit(0);
    }

    int socketListenStatus = listen(socketFd, 10);
    if (socketListenStatus == -1) {
        perror("Error while listening for connections on the server socket!");
        close(socketFd);
        _exit(0);
    }

    int clientSize;
    int connFd;
    while (1) {
        printf("Waiting for client..........\n");
        clientSize = (int)sizeof(clientAddress);
        connFd =
            accept(socketFd, (struct sockaddr *)&clientAddress, &clientSize);
        if (connFd == -1) {
            perror("Error while connecting to client!");
            close(socketFd);
        } else {
            if (!fork()) {
                // Child will enter this branch
                connection_handler(connFd);
                close(connFd);
                _exit(0);
            }
        }
    }

    close(socketFd);
}

////to close a port ---  in terminal write -->>> fuser -k 8080/tcp
