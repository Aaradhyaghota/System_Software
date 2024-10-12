#ifndef COMMON_FUNCTIONS
#define COMMON_FUNCTIONS

#include <errno.h>    // Import for `errno`
#include <fcntl.h>    // Import for `open`
#include <stdbool.h>  // Import for `bool` data type
#include <stdio.h>    // Import for `printf` & `perror`
#include <stdlib.h>   // Import for `atoi`
#include <string.h>   // Import for string functions
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>   // Import for `open`
#include <sys/types.h>  // Import for `open`, `lseek`
#include <unistd.h>     // Import for `read`, `write & `lseek`

#include "../record structures/customer.h"
#include "../record structures/employee.h"
#include "../record structures/transaction.h"
#include "./server_constants.h"

void show_transaction(int connFD, int trans_id, int customer_id) {
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Transaction trans;

    int transactionFileDescriptor = open(TRANSACTION_FILE, O_RDONLY);
    if (transactionFileDescriptor == -1) {
        perror("Error opening transaction file in read mode!");
        return;
    }

    off_t offset = lseek(transactionFileDescriptor, trans_id * sizeof(struct Transaction), SEEK_SET);
    if (offset >= 0) {
        struct flock lock = {F_RDLCK, SEEK_SET, trans_id * sizeof(struct Transaction), sizeof(struct Transaction), getpid()};

        int lockingStatus = fcntl(transactionFileDescriptor, F_SETLKW, &lock);
        if (lockingStatus == -1) {
            perror("Error obtaining read lock on customer record!");
            return;
        }

        readBytes = read(transactionFileDescriptor, &trans, sizeof(struct Transaction));
        if (readBytes == -1) {
            perror("Error reading customer record from file!");
        }

        lock.l_type = F_UNLCK;
        fcntl(transactionFileDescriptor, F_SETLK, &lock);

        char operation[10];
        switch (trans.operation) {
            case 0:
                strcpy(operation, "Withdraw");
                break;
            case 1:
                strcpy(operation, "Deposit");
                break;
            case 2:
                strcpy(operation, "Debit");
                break;
            case 3:
                strcpy(operation, "Credit");
                break;
        }
        if (trans.customer_id == customer_id) {
            bzero(writeBuffer, sizeof(writeBuffer));
            sprintf(writeBuffer, "%s\n%s%d\n%s%d\n%s%d\n%s%s\n%s%d\n%s", "---------------Transaction Detail---------------", "Transaction id - ", trans.transactionID, "Customer account no. - ", trans.customer_id + 1000, "Old balance - ", trans.oldBalance, "Operation - ", operation, "New Balance - ", trans.newBalance, "----------------------------------------------------");

            strcat(writeBuffer, "\n^");

            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read

        } else {
            writeBytes = write(connFD, TRANSACTION_NOT_ACCESSABLE, strlen(TRANSACTION_NOT_ACCESSABLE));
        }

        close(transactionFileDescriptor);
    } else {
        writeBytes = write(connFD, TRANSACTION_ID_DOESNT_EXIT, strlen(TRANSACTION_ID_DOESNT_EXIT));
    }
}

void transaction_history(int connFD, int customer_id) {
    ssize_t writeBytes, readBytes;  // Number of bytes read from / written to the client
    char readBuffer[1000], writeBuffer[1000];
    char tempBuffer[1000];

    bzero(readBuffer, sizeof(readBuffer));
    bzero(writeBuffer, sizeof(writeBuffer));

    struct Customer cust;
    int customerFileFD = open(CUSTOMER_FILE, O_RDONLY);
    if (customerFileFD == -1) {
        perror("Error opening employee file in read mode!");
        return;
    }

    int offset = lseek(customerFileFD, customer_id * sizeof(struct Customer), SEEK_SET);
    if (offset == -1) {
        perror("Error while seeking to required account record!");
        return;
    }
    // locking
    struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};

    int lockingStatus = fcntl(customerFileFD, F_SETLKW, &lock);
    if (lockingStatus == -1) {
        perror("Error obtaining read lock on account record!");
        return;
    }
    // reading from file to struct customer
    readBytes = read(customerFileFD, &cust, sizeof(struct Customer));
    if (readBytes == -1) {
        perror("Error reading account record from file!");
        return;
    }

    lock.l_type = F_UNLCK;
    fcntl(customerFileFD, F_SETLK, &lock);
    close(customerFileFD);

    for (int i = 0; cust.transactions[i] != -1 && i < MAX_TRANSACTIONS; i++) {
        show_transaction(connFD, cust.transactions[i], customer_id);
    }
}

#endif