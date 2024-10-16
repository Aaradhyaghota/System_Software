#ifndef CUSTOMER_FUNCTIONS
#define CUSTOMER_FUNCTIONS

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
#include "../record structures/feedback.h"
#include "../record structures/loan.h"
#include "../record structures/transaction.h"
#include "./commonF.h"
#include "./server_constants.h"

// Function Prototypes =================================
bool Clogin_handler(int connFD);
void view_balance(int connFD);
void deposit_money(int connFD);
bool customer_operation(int connFD);

// function definaation

struct Customer customer;

bool Clogin_handler(int connFD) {
    ssize_t readBytes, writeBytes;             // Number of bytes written to / read from the socket
    char readBuffer[1000], writeBuffer[1000];  // Buffer for reading from / writing to the client
    char tempBuffer[1000];

    bzero(readBuffer, sizeof(readBuffer));
    bzero(writeBuffer, sizeof(writeBuffer));

    // Get login message for respective user type
    strcpy(writeBuffer, CUSTOMER_LOGIN_WELCOME);

    // Append the request for LOGIN ID message
    strcat(writeBuffer, "\n");
    strcat(writeBuffer, LOGIN_ID);
    // writeto client
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1) {
        perror("Error writing WELCOME & LOGIN_ID message to the client!");
        return false;
    }
    // read id from client
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error reading login ID from client!");
        return false;
    }

    bool userFound = false;
    // extracting customer id
    bzero(tempBuffer, sizeof(tempBuffer));
    strcpy(tempBuffer, readBuffer);
    strtok(tempBuffer, "-");
    int ID = atoi(strtok(NULL, "-"));
    int customerFileFD = open(CUSTOMER_FILE, O_RDONLY);
    if (customerFileFD == -1) {
        perror("Error opening employee file in read mode!");
        return false;
    }
    // reading data from customer.txt to check user existance
    off_t offset = lseek(customerFileFD, ID * sizeof(struct Customer), SEEK_SET);
    if (offset >= 0) {
        struct flock lock = {F_RDLCK, SEEK_SET, ID * sizeof(struct Customer), sizeof(struct Customer), getpid()};

        int lockingStatus = fcntl(customerFileFD, F_SETLKW, &lock);
        if (lockingStatus == -1) {
            perror("Error obtaining read lock on employee record!");
            return false;
        }

        readBytes = read(customerFileFD, &customer, sizeof(struct Customer));
        if (readBytes == -1) {
            perror("Error reading customer record from file!");
        }

        lock.l_type = F_UNLCK;
        fcntl(customerFileFD, F_SETLK, &lock);

        if (strcmp(customer.login, readBuffer) == 0)
            userFound = true;

        close(customerFileFD);
    } else {
        writeBytes = write(connFD, CUSTOMER_LOGIN_ID_DOESNT_EXIT, strlen(CUSTOMER_LOGIN_ID_DOESNT_EXIT));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
    }

    if (userFound) {
        // ask for password
        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connFD, PASSWORD, strlen(PASSWORD));
        if (writeBytes == -1) {
            perror("Error writing PASSWORD message to client!");
            return false;
        }
        // take password
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == 1) {
            perror("Error reading password from the client!");
            return false;
        }
        // check for correct passoword
        char hashedPassword[1000];
        strcpy(hashedPassword, crypt(readBuffer, SALT_BAE));

        if (strcmp(hashedPassword, customer.password) == 0) {
            return true;
        }

        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connFD, INVALID_PASSWORD, strlen(INVALID_PASSWORD));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
    } else {
        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connFD, INVALID_LOGIN, strlen(INVALID_LOGIN));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
    }

    return false;
}

void view_balance(int connFD) {
    ssize_t writeBytes, readBytes;  // Number of bytes read from / written to the client
    char readBuffer[1000], writeBuffer[1000];
    char tempBuffer[1000];

    bzero(readBuffer, sizeof(readBuffer));
    bzero(writeBuffer, sizeof(writeBuffer));

    // int ID = customerID_global;

    // opening customer.txt
    int customerFileDescriptor = open(CUSTOMER_FILE, O_RDONLY);
    if (customerFileDescriptor == -1) {
        // Account record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, ACCOUNT_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        perror("Error opening account file in get_account_details!");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1) {
            perror("Error while writing ACCOUNT_ID_DOESNT_EXIT message to client!");
            return;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        return;
    }
    // seeking to offset of given account no.
    int offset = lseek(customerFileDescriptor, customer.id * sizeof(struct Customer), SEEK_SET);
    if (offset == -1 && errno == EINVAL) {
        // Account record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, ACCOUNT_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        perror("Error seeking to account record in get_account_details!");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1) {
            perror("Error while writing ACCOUNT_ID_DOESNT_EXIT message to client!");
            return;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        return;
    } else if (offset == -1) {
        perror("Error while seeking to required account record!");
        return;
    }
    // locking
    struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};

    int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1) {
        perror("Error obtaining read lock on account record!");
        return;
    }
    // reading from file to struct customer
    readBytes = read(customerFileDescriptor, &customer, sizeof(struct Customer));
    if (readBytes == -1) {
        perror("Error reading account record from file!");
        return;
    }

    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLK, &lock);
    close(customerFileDescriptor);

    bzero(writeBuffer, sizeof(writeBuffer));
    sprintf(writeBuffer, "Account Details - \n\tAccount Number : %d\n\tAccount Status : %s", customer.id + 1000, (customer.active) ? "Active" : "Deactived");
    if (customer.active) {
        sprintf(tempBuffer, "\n\tAccount Balance:₹ %d", customer.balance);
        strcat(writeBuffer, tempBuffer);
    }
    strcat(writeBuffer, "\n^");

    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read

    return;
}

int write_transaction_to_file(int customer_id, long int oldBalance, long int newBalance, int operation) {
    struct Transaction newTransaction;
    newTransaction.customer_id = customer_id;
    newTransaction.oldBalance = oldBalance;
    newTransaction.newBalance = newBalance;
    newTransaction.operation = operation;
    newTransaction.transactionTime = time(NULL);

    ssize_t readBytes, writeBytes;

    int transactionFileDescriptor = open(TRANSACTION_FILE, O_CREAT | O_APPEND | O_RDWR, S_IRWXU);

    // Get most recent transaction number
    off_t offset = lseek(transactionFileDescriptor, -sizeof(struct Transaction), SEEK_END);
    if (offset >= 0) {
        // There exists at least one transaction record
        struct Transaction prevTransaction;
        readBytes = read(transactionFileDescriptor, &prevTransaction, sizeof(struct Transaction));

        newTransaction.transactionID = prevTransaction.transactionID + 1;
    } else
        // No transaction records exist
        newTransaction.transactionID = 0;

    writeBytes = write(transactionFileDescriptor, &newTransaction, sizeof(struct Transaction));

    return newTransaction.transactionID;
}

void write_transaction_to_array(int* transactionArray, int transaction_ID) {
    // Check if there's any free space in the array to write the new transaction ID
    int iter = 0;
    while (transactionArray[iter] != -1)
        iter++;

    if (iter >= MAX_TRANSACTIONS) {
        // No space
        for (iter = 1; iter < MAX_TRANSACTIONS; iter++) {
            // Shift elements one step back discarding the oldest transaction
            transactionArray[iter - 1] = transactionArray[iter];
        }
        transactionArray[iter - 1] = transaction_ID;
    } else {
        // Space available
        transactionArray[iter] = transaction_ID;
    }
}

void deposit_money(int connFD) {
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    int depositAmount = 0;
    // // Lock the critical section
    // struct sembuf semOp;
    // lock_critical_section(&semOp);

    int customerFileDescriptor = open(CUSTOMER_FILE, O_RDWR);
    if (customerFileDescriptor == -1) {
        // Account record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, ACCOUNT_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        perror("Error opening account file in get_account_details!");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1) {
            perror("Error while writing ACCOUNT_ID_DOESNT_EXIT message to client!");
            return;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        return;
    }
    int offset = lseek(customerFileDescriptor, customer.id * sizeof(struct Customer), SEEK_SET);
    if (offset == -1 && errno == EINVAL) {
        // Account record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, ACCOUNT_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        perror("Error seeking to account record in get_account_details!");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1) {
            perror("Error while writing ACCOUNT_ID_DOESNT_EXIT message to client!");
            return;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        return;
    } else if (offset == -1) {
        perror("Error while seeking to required account record!");
        return;
    }

    struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};

    int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1) {
        perror("Error obtaining read lock on account record!");
        return;
    }

    readBytes = read(customerFileDescriptor, &customer, sizeof(struct Customer));
    if (readBytes == -1) {
        perror("Error reading account record from file!");
        return;
    }

    if (customer.active) {
        writeBytes = write(connFD, DEPOSIT_AMOUNT, strlen(DEPOSIT_AMOUNT));
        if (writeBytes == -1) {
            perror("Error writing DEPOSIT_AMOUNT to client!");
            // unlock_critical_section(&semOp);
            lock.l_type = F_UNLCK;
            fcntl(customerFileDescriptor, F_SETLK, &lock);
            close(customerFileDescriptor);
            return;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1) {
            perror("Error reading deposit money from client!");
            // unlock_critical_section(&semOp);
            lock.l_type = F_UNLCK;
            fcntl(customerFileDescriptor, F_SETLK, &lock);
            close(customerFileDescriptor);
            return;
        }

        depositAmount = atoi(readBuffer);
        if (depositAmount != 0) {
            // transation wala part
            int newTransactionID = write_transaction_to_file(customer.id, customer.balance, customer.balance + depositAmount, 1);
            write_transaction_to_array(customer.transactions, newTransactionID);

            // changing the account balance
            customer.balance = customer.balance + depositAmount;

            int offset = lseek(customerFileDescriptor, customer.id * sizeof(struct Customer), SEEK_SET);
            writeBytes = write(customerFileDescriptor, &customer, sizeof(struct Customer));
            if (writeBytes == -1) {
                perror("Error storing updated deposit money in account record!");
                // unlock_critical_section(&semOp);
                lock.l_type = F_UNLCK;
                fcntl(customerFileDescriptor, F_SETLK, &lock);
                close(customerFileDescriptor);
                return;
            }
            write(connFD, DEPOSIT_AMOUNT_SUCCESS, strlen(DEPOSIT_AMOUNT_SUCCESS));
            read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read

        } else {
            writeBytes = write(connFD, DEPOSIT_AMOUNT_INVALID, strlen(DEPOSIT_AMOUNT_INVALID));
            read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        }
    } else {
        write(connFD, ACCOUNT_DEACTIVATED, strlen(ACCOUNT_DEACTIVATED));
        read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
    }

    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLK, &lock);
    close(customerFileDescriptor);

    view_balance(connFD);

    // unlock_critical_section(&semOp);
}

void withdraw_money(int connFD) {
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    int withdrawAmount = 0;
    // Lock the critical section
    // struct sembuf semOp;
    // lock_critical_section(&semOp);

    int customerFileDescriptor = open(CUSTOMER_FILE, O_RDWR);
    if (customerFileDescriptor == -1) {
        // Account record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, ACCOUNT_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        perror("Error opening account file in get_account_details!");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1) {
            perror("Error while writing ACCOUNT_ID_DOESNT_EXIT message to client!");
            return;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        return;
    }
    int offset = lseek(customerFileDescriptor, customer.id * sizeof(struct Customer), SEEK_SET);
    if (offset == -1 && errno == EINVAL) {
        // Account record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, ACCOUNT_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        perror("Error seeking to account record in get_account_details!");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1) {
            perror("Error while writing ACCOUNT_ID_DOESNT_EXIT message to client!");
            return;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        return;
    } else if (offset == -1) {
        perror("Error while seeking to required account record!");
        return;
    }

    struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};

    int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1) {
        perror("Error obtaining readiwrite lock on account record!");
        return;
    }

    readBytes = read(customerFileDescriptor, &customer, sizeof(struct Customer));
    if (readBytes == -1) {
        perror("Error reading account record from file!");
        return;
    }
    if (customer.active) {
        writeBytes = write(connFD, WITHDRAW_AMOUNT, strlen(WITHDRAW_AMOUNT));
        if (writeBytes == -1) {
            perror("Error writing WITHDRAW_AMOUNT to client!");
            // unlock_critical_section(&semOp);
            lock.l_type = F_UNLCK;
            fcntl(customerFileDescriptor, F_SETLK, &lock);
            close(customerFileDescriptor);
            return;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1) {
            perror("Error reading withdraw money from client!");
            // unlock_critical_section(&semOp);
            lock.l_type = F_UNLCK;
            fcntl(customerFileDescriptor, F_SETLK, &lock);
            close(customerFileDescriptor);
            return;
        }

        withdrawAmount = atol(readBuffer);
        if (withdrawAmount != 0 && withdrawAmount <= customer.balance) {
            // transation wala part
            int newTransactionID = write_transaction_to_file(customer.id, customer.balance, customer.balance - withdrawAmount, 0);
            write_transaction_to_array(customer.transactions, newTransactionID);

            // withdrawing amount from account
            customer.balance = customer.balance - withdrawAmount;

            int offset = lseek(customerFileDescriptor, customer.id * sizeof(struct Customer), SEEK_SET);
            writeBytes = write(customerFileDescriptor, &customer, sizeof(struct Customer));
            if (writeBytes == -1) {
                perror("Error storing updated withdraw money in account record!");
                // unlock_critical_section(&semOp);
                lock.l_type = F_UNLCK;
                fcntl(customerFileDescriptor, F_SETLK, &lock);
                close(customerFileDescriptor);
                return;
            }
            write(connFD, WITHDRAW_AMOUNT_SUCCESS, strlen(WITHDRAW_AMOUNT_SUCCESS));
            read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read

        } else {
            writeBytes = write(connFD, WITHDRAW_AMOUNT_INVALID, strlen(WITHDRAW_AMOUNT_INVALID));
            read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        }
    } else {
        write(connFD, ACCOUNT_DEACTIVATED, strlen(ACCOUNT_DEACTIVATED));
        read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
    }

    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLK, &lock);
    close(customerFileDescriptor);

    view_balance(connFD);

    // unlock_critical_section(&semOp);
}

void transfer_funds(int connFD) {
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    int depositAmount = 0;
    // Lock the critical section
    // struct sembuf semOp;
    // lock_critical_section(&semOp);

    int customerFileDescriptor = open(CUSTOMER_FILE, O_RDWR);
    if (customerFileDescriptor == -1) {
        // Account record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, ACCOUNT_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        perror("Error opening account file in get_account_details!");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1) {
            perror("Error while writing ACCOUNT_ID_DOESNT_EXIT message to client!");
            // unlock_critical_section(&semOp);
            return;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        // unlock_critical_section(&semOp);  // Dummy read
        return;
    }
    // go to current customer data
    int offset = lseek(customerFileDescriptor, customer.id * sizeof(struct Customer), SEEK_SET);
    if (offset == -1 && errno == EINVAL) {
        // Account record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, ACCOUNT_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        perror("Error seeking to account record in get_account_details!");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1) {
            perror("Error while writing ACCOUNT_ID_DOESNT_EXIT message to client!");
            // unlock_critical_section(&semOp);
            return;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        // unlock_critical_section(&semOp);
        return;
    } else if (offset == -1) {
        perror("Error while seeking to required account record!");
        // unlock_critical_section(&semOp);
        return;
    }
    // asking for account no.
    bzero(writeBuffer, sizeof(writeBuffer));
    writeBytes = write(connFD, ACOUNT_TRANSFER, strlen(ACOUNT_TRANSFER));
    if (writeBytes == -1) {
        perror("Error while writing ACOUNT_TRANSFER message to client!");
        // unlock_critical_section(&semOp);
        return;
    }
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error while reading message from client!");
        // unlock_critical_section(&semOp);
        return;
    }

    struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};

    int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1) {
        perror("Error obtaining read lock on account record!");
        // unlock_critical_section(&semOp);
        return;
    }

    int custID2 = atoi(readBuffer);
    custID2 = custID2 - 1000;
    struct Customer customer2;

    // going to reciever(customer2) data in file
    int offset2 = lseek(customerFileDescriptor, custID2 * sizeof(struct Customer), SEEK_SET);
    if (offset2 == -1 && errno == EINVAL) {
        // Account record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, ACCOUNT_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        perror("Error seeking to account record in get_account_details!");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1) {
            perror("Error while writing ACCOUNT_ID_DOESNT_EXIT message to client!");
            // unlock_critical_section(&semOp);
            lock.l_type = F_UNLCK;
            fcntl(customerFileDescriptor, F_SETLK, &lock);
            return;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        // unlock_critical_section(&semOp);
        lock.l_type = F_UNLCK;
        fcntl(customerFileDescriptor, F_SETLK, &lock);
        return;
    } else if (offset == -1) {
        perror("Error while seeking to required account record!");
        // unlock_critical_section(&semOp);
        lock.l_type = F_UNLCK;
        fcntl(customerFileDescriptor, F_SETLK, &lock);
        return;
    }
    // locking 2nd customer record.
    struct flock lock2 = {F_WRLCK, SEEK_SET, offset2, sizeof(struct Customer), getpid()};

    int lockingStatus2 = fcntl(customerFileDescriptor, F_SETLKW, &lock2);
    if (lockingStatus2 == -1) {
        perror("Error obtaining read lock on account record!");
        // unlock_critical_section(&semOp);
        lock.l_type = F_UNLCK;
        fcntl(customerFileDescriptor, F_SETLK, &lock);
        close(customerFileDescriptor);
        return;
    }
    // copying 2nd customer record to custumer2
    readBytes = read(customerFileDescriptor, &customer2, sizeof(struct Customer));
    if (readBytes == -1) {
        perror("Error reading customer2 record from file!");
        // unlock_critical_section(&semOp);
        lock.l_type = F_UNLCK;
        fcntl(customerFileDescriptor, F_SETLK, &lock);
        lock2.l_type = F_UNLCK;
        fcntl(customerFileDescriptor, F_SETLK, &lock2);
        close(customerFileDescriptor);
    }

    // asking the amount you want to transfer
    bzero(writeBuffer, sizeof(writeBuffer));
    writeBytes = write(connFD, AMOUNT_TRANSFER, strlen(AMOUNT_TRANSFER));
    if (writeBytes == -1) {
        perror("Error while writing AMOUNT_TRANSFER message to client!");
        // unlock_critical_section(&semOp);
        lock.l_type = F_UNLCK;
        fcntl(customerFileDescriptor, F_SETLK, &lock);
        lock2.l_type = F_UNLCK;
        fcntl(customerFileDescriptor, F_SETLK, &lock2);
        close(customerFileDescriptor);
        return;
    }
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error while reading message from client!");
        // unlock_critical_section(&semOp);
        lock.l_type = F_UNLCK;
        fcntl(customerFileDescriptor, F_SETLK, &lock);
        lock2.l_type = F_UNLCK;
        fcntl(customerFileDescriptor, F_SETLK, &lock2);
        close(customerFileDescriptor);
        return;
    }
    if (customer.active && customer2.active) {
        int amount_to_transfer = atoi(readBuffer);
        if (amount_to_transfer != 0 && amount_to_transfer <= customer.balance) {
            // transaction wala part
            int newTransactionID = write_transaction_to_file(customer.id, customer.balance, customer.balance - amount_to_transfer, 2);
            write_transaction_to_array(customer.transactions, newTransactionID);

            int newTransactionID2 = write_transaction_to_file(customer2.id, customer2.balance, customer2.balance + amount_to_transfer, 3);
            write_transaction_to_array(customer2.transactions, newTransactionID2);

            // transfer
            customer.balance = customer.balance - amount_to_transfer;
            customer2.balance = customer2.balance + amount_to_transfer;
            // writtign to customer 1
            offset = lseek(customerFileDescriptor, customer.id * sizeof(struct Customer), SEEK_SET);
            writeBytes = write(customerFileDescriptor, &customer, sizeof(struct Customer));
            if (writeBytes == -1) {
                perror("Error storing updated deposit money in account record!");
                // unlock_critical_section(&semOp);
                lock.l_type = F_UNLCK;
                fcntl(customerFileDescriptor, F_SETLK, &lock);
                lock2.l_type = F_UNLCK;
                fcntl(customerFileDescriptor, F_SETLK, &lock2);
                close(customerFileDescriptor);
                return;
            }
            // writtign to customer 2
            offset2 = lseek(customerFileDescriptor, customer2.id * sizeof(struct Customer), SEEK_SET);
            writeBytes = write(customerFileDescriptor, &customer2, sizeof(struct Customer));
            if (writeBytes == -1) {
                perror("Error storing updated deposit money in account record!");
                // unlock_critical_section(&semOp);
                lock.l_type = F_UNLCK;
                fcntl(customerFileDescriptor, F_SETLK, &lock);
                lock2.l_type = F_UNLCK;
                fcntl(customerFileDescriptor, F_SETLK, &lock2);
                close(customerFileDescriptor);
                return;
            }
            write(connFD, TRANSFER_AMOUNT_SUCCESS, strlen(TRANSFER_AMOUNT_SUCCESS));
            read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        } else {
            writeBytes = write(connFD, TRANSFER_AMOUNT_INVALID, strlen(TRANSFER_AMOUNT_INVALID));
            read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        }
    } else {
        write(connFD, ACCOUNT_DEACTIVATED, strlen(ACCOUNT_DEACTIVATED));
        read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
    }

    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLK, &lock);
    lock2.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLK, &lock2);
    close(customerFileDescriptor);

    view_balance(connFD);

    // unlock_critical_section(&semOp);
}

void change_password(int connFD) {
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000], hashedPassword[1000];

    char newPassword[1000];

    // Lock the critical section
    // struct sembuf semOp;
    // lock_critical_section(&semOp);

    writeBytes = write(connFD, PASSWORD_CHANGE_OLD_PASS, strlen(PASSWORD_CHANGE_OLD_PASS));
    if (writeBytes == -1) {
        perror("Error writing PASSWORD_CHANGE_OLD_PASS message to client!");
        // unlock_critical_section(&semOp);
        return;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error reading old password response from client");
        // unlock_critical_section(&semOp);
        return;
    }

    if (strcmp(crypt(readBuffer, SALT_BAE), customer.password) == 0) {
        // Password matches with old password
        writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS, strlen(PASSWORD_CHANGE_NEW_PASS));
        if (writeBytes == -1) {
            perror("Error writing PASSWORD_CHANGE_NEW_PASS message to client!");
            // unlock_critical_section(&semOp);
            return;
        }
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1) {
            perror("Error reading new password response from client");
            // unlock_critical_section(&semOp);
            return;
        }

        strcpy(newPassword, crypt(readBuffer, SALT_BAE));

        writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS_RE, strlen(PASSWORD_CHANGE_NEW_PASS_RE));
        if (writeBytes == -1) {
            perror("Error writing PASSWORD_CHANGE_NEW_PASS_RE message to client!");
            // unlock_critical_section(&semOp);
            return;
        }
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1) {
            perror("Error reading new password reenter response from client");
            // unlock_critical_section(&semOp);
            return;
        }

        if (strcmp(crypt(readBuffer, SALT_BAE), newPassword) == 0) {
            // New & reentered passwords match
            bzero(customer.password, sizeof(customer.password));
            strcpy(customer.password, newPassword);

            int customerFileDescriptor = open(CUSTOMER_FILE, O_WRONLY);
            if (customerFileDescriptor == -1) {
                perror("Error opening customer file!");
                // unlock_critical_section(&semOp);
                return;
            }

            off_t offset = lseek(customerFileDescriptor, customer.id * sizeof(struct Customer), SEEK_SET);
            if (offset == -1) {
                perror("Error seeking to the customer record!");
                // unlock_critical_section(&semOp);
                return;
            }

            struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};
            int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
            if (lockingStatus == -1) {
                perror("Error obtaining write lock on customer record!");
                // unlock_critical_section(&semOp);
                return;
            }

            writeBytes = write(customerFileDescriptor, &customer, sizeof(struct Customer));
            if (writeBytes == -1) {
                perror("Error storing updated customer password into customer record!");
                // unlock_critical_section(&semOp);
                return;
            }

            lock.l_type = F_UNLCK;
            lockingStatus = fcntl(customerFileDescriptor, F_SETLK, &lock);

            close(customerFileDescriptor);

            writeBytes = write(connFD, PASSWORD_CHANGE_SUCCESS, strlen(PASSWORD_CHANGE_SUCCESS));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read

            // unlock_critical_section(&semOp);

            return;
        } else {
            // New & reentered passwords don't match
            writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS_INVALID, strlen(PASSWORD_CHANGE_NEW_PASS_INVALID));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        }
    } else {
        // Password doesn't match with old password
        writeBytes = write(connFD, PASSWORD_CHANGE_OLD_PASS_INVALID, strlen(PASSWORD_CHANGE_OLD_PASS_INVALID));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
    }

    // unlock_critical_section(&semOp);

    return;
}

void get_transaction_detail(int connFD) {
    ssize_t writeBytes, readBytes;  // Number of bytes read from / written to the client
    char readBuffer[1000], writeBuffer[1000];

    bzero(writeBuffer, sizeof(writeBuffer));
    writeBytes = write(connFD, ENTER_TRANCTION_ID, strlen(ENTER_TRANCTION_ID));
    if (writeBytes == -1) {
        perror("Error writing ENTER_TRANCTION_ID message to client!");
        return;
    }
    // take transaction id
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error reading transaction id from the client!");
        return;
    }
    int t_ID = atoi(readBuffer);
    // printf("%d\n", t_ID);
    show_transaction(connFD, t_ID, customer.id);
}

void apply_loan(int connFD) {
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];
    if (customer.loan_status == 0 || customer.loan_status == 4 || customer.loan_status == 3) {
        // customer can apply for loan when he haven't applied or his prev loan was rejected or approved
        struct Loan newloan, prevloan;
        int loanFD = open(LOAN_FILE, O_RDONLY);
        if (loanFD == -1 && errno == ENOENT) {
            // Loan file was never created
            newloan.id = 0;
        } else if (loanFD == -1) {
            perror("Error while opening Loan file");
            return;
        } else {
            int offset = lseek(loanFD, -sizeof(struct Loan), SEEK_END);
            if (offset == -1) {
                perror("Error seeking to last Loan record!");
                return;
            }

            struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Loan), getpid()};
            int lockingStatus = fcntl(loanFD, F_SETLKW, &lock);
            if (lockingStatus == -1) {
                perror("Error obtaining read lock on Loan record!");
                return;
            }
            // here while reading without lock will not effect or give any deadlock because id cannot be change once its created.
            readBytes = read(loanFD, &prevloan, sizeof(struct Loan));
            if (readBytes == -1) {
                perror("Error while reading Customer record from file!");
                return;
            }

            lock.l_type = F_UNLCK;
            fcntl(loanFD, F_SETLK, &lock);

            close(loanFD);

            newloan.id = prevloan.id + 1;
        }
        newloan.customerID = customer.id;
        newloan.status = 0;  // pending (manager)

        writeBytes = write(connFD, CUSTOMER_ADD_LOAN, strlen(CUSTOMER_ADD_LOAN));
        if (writeBytes == -1) {
            perror("Error writing CUSTOMER_ADD_LOAN message to client!");
            return;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1) {
            perror("Error reading customer loan amount response from client!");
            return;
        }

        int loan_amount = atoi(readBuffer);
        newloan.amount = loan_amount;
        // completer from here
        loanFD = open(LOAN_FILE, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
        if (loanFD == -1) {
            perror("Error while creating / opening loan file!");
            return;
        }
        // seeking  to loan record in file
        off_t offset = lseek(loanFD, newloan.id * sizeof(struct Loan), SEEK_SET);
        if (offset == -1) {
            perror("Error while seeking to required loan record!");
            return;
        }
        // Lock the record to be write
        struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Loan), getpid()};
        int lockingStatus = fcntl(loanFD, F_SETLKW, &lock);
        if (lockingStatus == -1) {
            perror("Couldn't obtain lock on loan record!");
            return;
        }
        // writting to customer file
        writeBytes = write(loanFD, &newloan, sizeof(struct Loan));
        if (writeBytes == -1) {
            perror("Error while writing to loan record to file!");
            return;
        }
        // unlocking
        lock.l_type = F_UNLCK;
        fcntl(loanFD, F_SETLKW, &lock);

        close(loanFD);

        // custoemr loan_status update
        int customerFD = open(CUSTOMER_FILE, O_WRONLY, S_IRWXU);
        if (customerFD == -1) {
            perror("Error while creating / opening customer file!");
            return;
        }
        // seeking  to customer record in file
        offset = lseek(customerFD, customer.id * sizeof(struct Customer), SEEK_SET);
        if (offset == -1) {
            perror("Error while seeking to required customer record!");
            return;
        }
        // write lock on customer record
        lock.l_type = F_WRLCK;
        lock.l_start = offset;
        lockingStatus = fcntl(customerFD, F_SETLKW, &lock);
        if (lockingStatus == -1) {
            perror("Error while obtaining write lock on customer record!");
            return;
        }
        customer.loan_status = 1;  // customer applied for loan
        // writting to record
        writeBytes = write(customerFD, &customer, sizeof(struct Customer));
        if (writeBytes == -1) {
            perror("Error while writing to customer record to file!");
            return;
        }
        // unlocking
        lock.l_type = F_UNLCK;
        fcntl(loanFD, F_SETLKW, &lock);

        close(customerFD);
        // wrtting success mesage to client
        writeBytes = write(connFD, "Applied for loan succesfully!^", strlen("Applied for loan succesfully!^"));
        if (writeBytes == -1) {
            perror("Error while writing Applied for loan succesfully message to client!");
            return;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read

    } else {
        writeBytes = write(connFD, "You cannot apply for loan as you already one loan!^", strlen("You cannot apply for loan as you already one loan!^"));
        if (writeBytes == -1) {
            perror("Error while writing You cannot apply for loan as you already one loan message to client!");
            return;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
    }
}

bool customer_operation(int connFD) {
    if (Clogin_handler(connFD)) {
        ssize_t writeBytes, readBytes;  // Number of bytes read from / written to the client
        char readBuffer[1000], writeBuffer[1000];

        // Get a semaphore for the user
        key_t semKey = ftok(CUSTOMER_FILE, customer.id);  // Generate a key based on the account number hence, different customers will have different semaphores

        union semun {
            int val;  // Value of the semaphore
        } semSet;

        int semctlStatus;
        semIdentifier_c = semget(semKey, 1, 0);  // Get the semaphore if it exists
        if (semIdentifier_c == -1) {
            semIdentifier_c = semget(semKey, 1, IPC_CREAT | 0700);  // Create a new semaphore
            if (semIdentifier_c == -1) {
                perror("Error while creating semaphore!");
                _exit(1);
            }

            semSet.val = 1;  // Set a binary semaphore
            semctlStatus = semctl(semIdentifier_c, 0, SETVAL, semSet);
            if (semctlStatus == -1) {
                perror("Error while initializing a binary sempahore!");
                _exit(1);
            }
        }
        // Lock the critical section
        struct sembuf semOp;
        lock_critical_section(&semOp);
        printf("Customer Logged in.\n");
        // write to client success login
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, CUSTOMER_LOGIN_SUCCESS);
        while (1) {
            strcat(writeBuffer, "\n");
            strcat(writeBuffer, CUSTOMER_MENU);
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1) {
                perror("Error while writing ADMIN_MENU to client!");
                return false;
            }
            bzero(writeBuffer, sizeof(writeBuffer));

            // read choice from client for menu selection
            readBytes = read(connFD, readBuffer, sizeof(readBuffer));
            if (readBytes == -1) {
                perror("Error while reading client's choice for ADMIN_MENU");
                return false;
            }

            int choice = atoi(readBuffer);
            switch (choice) {
                case 1:
                    view_balance(connFD);
                    break;
                case 2:
                    deposit_money(connFD);
                    break;
                case 3:
                    withdraw_money(connFD);
                    break;
                case 4:
                    transfer_funds(connFD);
                    break;
                case 5:
                    apply_loan(connFD);
                    break;
                case 6:
                    change_password(connFD);
                    break;
                case 7:
                    add_feedback(connFD);
                    break;
                case 8:
                    get_transaction_detail(connFD);
                    break;
                case 9:
                    transaction_history(connFD, customer.id);
                    break;
                case 10:
                    writeBytes = write(connFD, CUSTOMER_LOGOUT, strlen(CUSTOMER_LOGOUT));
                    readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // dummy read
                    unlock_critical_section(&semOp);
                    return true;
                default:
                    writeBytes = write(connFD, CUSTOMER_EXIT, strlen(CUSTOMER_EXIT));
                    unlock_critical_section(&semOp);
                    return false;
            }
        }
    } else {
        return true;
    }
}

#endif

// Customers
//  Aman-0 -> 12345
//  Rish-1 -> defaultPassword
//  Saloni-2 ->defaultPassword
// to see ipcs -s
// to delete semaphore--> ipcrm -s {write semid here}

// we have to add lock for previous transaction.and while wrtting the new transaction