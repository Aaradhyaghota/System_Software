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
#include "../record structures/feedback.h"
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
            sprintf(writeBuffer, "%s\n%s%d\n%s%d\n%s%d\n%s%s\n%s%d\n%s", "---------------Transaction Detail---------------",
                    "Transaction id - ", trans.transactionID, "Customer account no. - ", trans.customer_id + 1000,
                    "Old balance - ", trans.oldBalance, "Operation - ", operation, "New Balance - ", trans.newBalance,
                    "----------------------------------------------------");

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

// custmeor and emp can see transation history
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

// any body can add feedback
void add_feedback(int connFD) {
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];
    struct Feedback newFdbk, prevFdbk;

    int feedbackFD = open(FEEDBACK_FILE, O_RDONLY);
    if (feedbackFD == -1 && errno == ENOENT) {
        // Feedback file was never created
        newFdbk.id = 0;
    } else if (feedbackFD == -1) {
        perror("Error while opening feedback file");
        return;
    } else {
        int offset = lseek(feedbackFD, -sizeof(struct Feedback), SEEK_END);
        if (offset == -1) {
            perror("Error seeking to last feedback record!");
            return;
        }

        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Feedback), getpid()};
        int lockingStatus = fcntl(feedbackFD, F_SETLKW, &lock);
        if (lockingStatus == -1) {
            perror("Error obtaining read lock on feedback record!");
            return;
        }
        // here while reading without lock will not effect or give any deadlock because id cannot be change once its created.
        readBytes = read(feedbackFD, &prevFdbk, sizeof(struct Feedback));
        if (readBytes == -1) {
            perror("Error while reading feedback record from file!");
            return;
        }

        lock.l_type = F_UNLCK;
        fcntl(feedbackFD, F_SETLK, &lock);

        close(feedbackFD);

        newFdbk.id = prevFdbk.id + 1;
    }
    // intitializing all variable of feedback
    newFdbk.reviewed = 0;

    sprintf(writeBuffer, "%s", CUSTOMER_ADD_FEEDBACK);
    writeBytes = write(connFD, writeBuffer, sizeof(writeBuffer));
    if (writeBytes == -1) {
        perror("Error writing CUSTOMER_ADD_FEEDBACK message to client!");
        return;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error reading  feedback response from client!");
        return;
    }
    bzero(newFdbk.text, sizeof(newFdbk.text));
    strcpy(newFdbk.text, readBuffer);

    feedbackFD = open(FEEDBACK_FILE, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
    if (feedbackFD == -1) {
        perror("Error while creating / opening customer file!");
        return;
    }
    // seeking  to customer record in file
    off_t offset = lseek(feedbackFD, newFdbk.id * sizeof(struct Feedback), SEEK_SET);
    if (offset == -1) {
        perror("Error while seeking to required employee record!");
        return;
    }

    // Lock the record to be write
    struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Feedback), getpid()};
    int lockingStatus = fcntl(feedbackFD, F_SETLKW, &lock);
    if (lockingStatus == -1) {
        perror("Couldn't obtain lock on feedback record!");
        return;
    }
    // writting to customer file
    writeBytes = write(feedbackFD, &newFdbk, sizeof(newFdbk));
    if (writeBytes == -1) {
        perror("Error while writing feedback record to file!");
        return;
    }
    // unlocking
    lock.l_type = F_UNLCK;
    fcntl(feedbackFD, F_SETLKW, &lock);

    close(feedbackFD);
    return;
}

// emp and manager password change
void em_change_password(int connFD, int emp_id) {
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000], hashedPassword[1000];
    char newPassword[1000];

    struct Employee empl;
    int empFD = open(EMPLOYEE_FILE, O_RDONLY);
    if (empFD == -1) {
        perror("Error opening employee file!");
        return;
    }

    off_t offset = lseek(empFD, emp_id * sizeof(struct Employee), SEEK_SET);
    if (offset == -1) {
        perror("Error seeking to the cusemployee record!");
        return;
    }

    struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Employee), getpid()};
    int lockingStatus = fcntl(empFD, F_SETLKW, &lock);
    if (lockingStatus == -1) {
        perror("Error obtaining write lock on cusemployee record!");
        return;
    }

    readBytes = read(empFD, &empl, sizeof(struct Employee));
    if (readBytes == -1) {
        perror("Error readding into employee struct!");
        return;
    }

    lock.l_type = F_UNLCK;
    lockingStatus = fcntl(empFD, F_SETLK, &lock);

    close(empFD);

    // enter old password
    writeBytes = write(connFD, PASSWORD_CHANGE_OLD_PASS, strlen(PASSWORD_CHANGE_OLD_PASS));
    if (writeBytes == -1) {
        perror("Error writing PASSWORD_CHANGE_OLD_PASS message to client!");
        return;
    }
    // read password
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error reading old password response from client");
        return;
    }
    if (strcmp(crypt(readBuffer, SALT_BAE), empl.password) == 0) {
        // Password matches with old password
        writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS, strlen(PASSWORD_CHANGE_NEW_PASS));
        if (writeBytes == -1) {
            perror("Error writing PASSWORD_CHANGE_NEW_PASS message to client!");
            return;
        }
        // readin new password
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1) {
            perror("Error reading new password response from client");
            return;
        }

        strcpy(newPassword, crypt(readBuffer, SALT_BAE));
        // reenter new pass
        writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS_RE, strlen(PASSWORD_CHANGE_NEW_PASS_RE));
        if (writeBytes == -1) {
            perror("Error writing PASSWORD_CHANGE_NEW_PASS_RE message to client!");
            return;
        }
        // reading new admin
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1) {
            perror("Error reading new password reenter response from client");
            return;
        }

        if (strcmp(crypt(readBuffer, SALT_BAE), newPassword) == 0) {
            // New & reentered passwords match
            bzero(empl.password, sizeof(empl.password));
            strcpy(empl.password, newPassword);

            empFD = open(EMPLOYEE_FILE, O_WRONLY);
            if (empFD == -1) {
                perror("Error opening employee file!");
                return;
            }

            offset = lseek(empFD, emp_id * sizeof(struct Employee), SEEK_SET);
            if (offset == -1) {
                perror("Error seeking to the employee record!");
                return;
            }

            lock.l_type = F_WRLCK;
            lock.l_start = offset;
            int lockingStatus = fcntl(empFD, F_SETLKW, &lock);
            if (lockingStatus == -1) {
                perror("Error obtaining write lock on employee record!");
                return;
            }

            writeBytes = write(empFD, &empl, sizeof(struct Employee));
            if (writeBytes == -1) {
                perror("Error storing updated employee password into employee record!");
                return;
            }

            lock.l_type = F_UNLCK;
            lockingStatus = fcntl(empFD, F_SETLK, &lock);

            close(empFD);

            writeBytes = write(connFD, PASSWORD_CHANGE_SUCCESS, strlen(PASSWORD_CHANGE_SUCCESS));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read

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

    return;
}

#endif