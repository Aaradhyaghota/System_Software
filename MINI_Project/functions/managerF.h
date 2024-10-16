#ifndef MANAGER_FUNCTIONS
#define MANAGER_FUNCTIONS

#include <errno.h>      // Import for `errno`
#include <fcntl.h>      // Import for `open`
#include <stdbool.h>    // Import for `bool` data type
#include <stdio.h>      // Import for `printf` & `perror`
#include <stdlib.h>     // Import for `atoi`
#include <string.h>     // Import for string functions
#include <sys/stat.h>   // Import for `open`
#include <sys/types.h>  // Import for `open`, `lseek`
#include <unistd.h>     // Import for `read`, `write & `lseek`

#include "../record structures/admin.h"
#include "../record structures/customer.h"
#include "../record structures/employee.h"
#include "../record structures/feedback.h"
#include "../record structures/transaction.h"
#include "./server_constants.h"

void activate_deacticated(int connFD) {
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Customer cust;
    // enter employee id
    bzero(writeBuffer, sizeof(writeBuffer));
    writeBytes = write(connFD, MANAGER_ACTIVE_CUSTOMER, strlen(MANAGER_ACTIVE_CUSTOMER));
    if (writeBytes == -1) {
        perror("Error while writing MANAGER_ACTIVE_CUSTOMER message to client!");
        return;
    }
    // reading account no.
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error while reading customer ID from client!");
        return;
    }
    // opening the file
    int customerFileDescriptor = open(CUSTOMER_FILE, O_RDONLY);
    if (customerFileDescriptor == -1) {
        perror("Error while opening customer file");
        return;
    }
    // seekinng to that id
    int customerID = atoi(readBuffer);
    customerID -= 1000;
    off_t offset = lseek(customerFileDescriptor, customerID * sizeof(struct Customer), SEEK_SET);
    if (errno == EINVAL) {
        // Customer record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, CUSTOMER_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1) {
            perror("Error while writing CUSTOMER_ID_DOESNT_EXIT message to client!");
            return;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        return;
    } else if (offset == -1) {
        perror("Error while seeking to required customer record!");
        return;
    }

    // Lock the record to be read
    struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};
    int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1) {
        perror("Couldn't obtain lock on custmoer record!");
        return;
    }

    // reading of file to struct
    readBytes = read(customerFileDescriptor, &cust, sizeof(struct Customer));
    if (readBytes == -1) {
        perror("Error while reading customer record from the file!");
        return;
    }

    // Unlock the record
    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLK, &lock);
    close(customerFileDescriptor);

    // asking for role
    bzero(writeBuffer, sizeof(writeBuffer));
    writeBytes = write(connFD, "Enter 'A' for changing status to active and 'D' for changing status to Deactivate", strlen("Enter 'A' for changing status to active and 'D' for changing status to Deactivate"));
    if (writeBytes == -1) {
        perror("Error while writing message to client!");
        return;
    }

    // reading role
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error while reading status from client!");
        return;
    }

    // make changes
    if (readBuffer[0] == 'A' || readBuffer[0] == 'D') {
        if (readBuffer[0] == 'A')
            cust.active = 1;  // active
        else
            cust.active = 0;  // deactive
    } else {
        writeBytes = write(connFD, MANAGER_ADD_CUSTOMER_WRONG_STATUS, strlen(MANAGER_ADD_CUSTOMER_WRONG_STATUS));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        return;
    }
    customerFileDescriptor = open(CUSTOMER_FILE, O_WRONLY);
    if (customerFileDescriptor == -1) {
        perror("Error while opening customer file");
        return;
    }
    offset = lseek(customerFileDescriptor, customerID * sizeof(struct Customer), SEEK_SET);
    if (offset == -1) {
        perror("Error while seeking to required customer record!");
        return;
    }
    // locking
    lock.l_type = F_WRLCK;
    lock.l_start = offset;
    lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1) {
        perror("Error while obtaining write lock on customer record!");
        return;
    }
    // writting the employee structure
    writeBytes = write(customerFileDescriptor, &cust, sizeof(struct Customer));
    if (writeBytes == -1) {
        perror("Error while writing update customer info into file");
        return;
    }
    // unlocking
    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLKW, &lock);

    close(customerFileDescriptor);

    // priting success
    writeBytes = write(connFD, "Successfully changed status!^", strlen("Successfully changed status!^"));
    if (writeBytes == -1) {
        perror("Error while writing message to client!");
        return;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read

    return;
}

void view_unassigned_loans(int connFD) {
    struct Loan loan;
    char writeBuffer[1000], readBuffer[1000];
    ssize_t writeBytes, readBytes;

    int loanFD = open("./records/loan.txt", O_RDONLY, S_IRWXU);
    if (loanFD == -1) {
        perror("Error while opening the loan file");
        return;
    }
    off_t offset = lseek(loanFD, 0, SEEK_SET);
    while (EOF) {
        int readBytes = read(loanFD, &loan, sizeof(struct Loan));
        if (readBytes == 0) {
            break;
        } else if (readBytes == -1) {
            perror("Error in reading from loan file");
            return;
        }
        if (loan.status == 0) {
            bzero(writeBuffer, sizeof(writeBuffer));
            sprintf(writeBuffer, "%s%d \n%s%d \n%s%d \n%s\n", "Loan Id- ", loan.id, "Customer Id- ", loan.customerID, "Loan amount - ", loan.amount, "----------------------");
            strcat(writeBuffer, "^");
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1) {
                perror("Error sending loanID and other info to the client!");
                return;
            }

            readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        }
    }
    close(loanFD);
    return;
}

void assign_loans_to_employee(int connFD) {
    struct Loan loan;
    char writeBuffer[1000], readBuffer[1000];
    ssize_t writeBytes, readBytes;

    int flag = 0;
    int loanFD = open(LOAN_FILE, O_RDONLY, S_IRWXU);
    if (loanFD == -1) {
        perror("Error while opening the loan file");
        return;
    }
    off_t offset = lseek(loanFD, 0, SEEK_SET);
    while (EOF) {
        int readBytes = read(loanFD, &loan, sizeof(struct Loan));
        if (readBytes == 0) {
            break;
        } else if (readBytes == -1) {
            perror("Error in reading from loan file");
            return;
        }
        if (loan.status == 0) {
            flag = 1;
            bzero(writeBuffer, sizeof(writeBuffer));
            sprintf(writeBuffer, "%s%d \n%s%d \n%s%d \n%s\n", "Loan Id- ", loan.id, "Customer Id- ", loan.customerID, "Loan amount - ", loan.amount, "----------------------");
            strcat(writeBuffer, "^");
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1) {
                perror("Error sending loanID and other info to the client!");
                return;
            }

            readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        }
    }
    close(loanFD);

    if (flag) {
        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connFD, "Enter the loan id you want to assign", strlen("Enter the loan id you want to assign"));
        if (writeBytes == -1) {
            perror("Error writing laon id asking message to client!");
            return;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1) {
            perror("Error reading loan id response from client!");
            return;
        }
        int loan_id = atoi(readBuffer);

        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connFD, "Enter the Employee id you want to assign", strlen("Enter the employee id you want to assign"));
        if (writeBytes == -1) {
            perror("Error writing employee id asking message to client!");
            return;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1) {
            perror("Error reading employee id response from client!");
            return;
        }
        int empID = atoi(readBuffer);

        loanFD = open(LOAN_FILE, O_RDWR, S_IRWXU);
        if (loanFD == -1) {
            perror("Error while opening the loan file");
            return;
        }

        off_t offset = lseek(loanFD, loan_id * sizeof(struct Loan), SEEK_SET);
        if (offset == -1) {
            perror("Error while seeking to required Loan record!");
            return;
        }
        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Loan), getpid()};
        int lockingStatus = fcntl(loanFD, F_SETLKW, &lock);
        if (lockingStatus == -1) {
            perror("Couldn't obtain read lock on loan record!");
            return;
        }
        // read from loan file
        readBytes = read(loanFD, &loan, sizeof(struct Loan));
        if (readBytes == -1) {
            perror("Error in reading form loan file");
            return;
        }
        // changing loan status bit
        loan.status = 1;  // assigned to an employee

        offset = lseek(loanFD, loan_id * sizeof(struct Loan), SEEK_SET);
        if (offset == -1) {
            perror("Error while seeking to required loan record!");
            return;
        }
        // Lock the record to be write
        lock.l_type = F_WRLCK;
        lock.l_start = offset;
        lockingStatus = fcntl(loanFD, F_SETLKW, &lock);
        if (lockingStatus == -1) {
            perror("Couldn't obtain write lock on loan record!");
            return;
        }
        // writting to loan file
        writeBytes = write(loanFD, &loan, sizeof(struct Loan));
        if (writeBytes == -1) {
            perror("Error while writing loan record to file!");
            return;
        }
        // unlocking
        lock.l_type = F_UNLCK;
        fcntl(loanFD, F_SETLKW, &lock);

        close(loanFD);

        // Customer---------------------------

        int custID = loan.customerID;
        struct Customer cust;
        // custoemr loan_status update
        int customerFD = open(CUSTOMER_FILE, O_RDWR, S_IRWXU);
        if (customerFD == -1) {
            perror("Error while creating / opening customer file!");
            return;
        }
        // seeking  to customer record in file
        offset = lseek(customerFD, custID * sizeof(struct Customer), SEEK_SET);
        if (offset == -1) {
            perror("Error while seeking to required customer record!");
            return;
        }
        // read lock on customer record
        lock.l_type = F_RDLCK;
        lock.l_start = offset;
        lockingStatus = fcntl(customerFD, F_SETLKW, &lock);
        if (lockingStatus == -1) {
            perror("Error while obtaining read lock on customer record!");
            return;
        }
        // reading from customer file
        readBytes = read(customerFD, &cust, sizeof(struct Customer));
        if (readBytes == -1) {
            perror("Error while reading Customer record from file!");
            return;
        }
        cust.loan_status = 2;  // customer  loan assigned to employee
        // seeking  to customer record in file
        offset = lseek(customerFD, custID * sizeof(struct Customer), SEEK_SET);
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

        // writting to record
        writeBytes = write(customerFD, &cust, sizeof(struct Customer));
        if (writeBytes == -1) {
            perror("Error while writing to customer record to file!");
            return;
        }
        // unlocking
        lock.l_type = F_UNLCK;
        fcntl(loanFD, F_SETLKW, &lock);

        close(customerFD);

        // employee assignment-----------------------------

        struct Employee empl;
        // employee loan assginment loan_status update
        int employeeFD = open(EMPLOYEE_FILE, O_RDWR, S_IRWXU);
        if (employeeFD == -1) {
            perror("Error while opening EMPLOYEE file!");
            return;
        }
        // seeking  to employee record in file
        offset = lseek(employeeFD, empID * sizeof(struct Employee), SEEK_SET);
        if (offset == -1) {
            perror("Error while seeking to required employee record!");
            return;
        }
        // read lock on employee record
        lock.l_type = F_RDLCK;
        lock.l_start = offset;
        lockingStatus = fcntl(employeeFD, F_SETLKW, &lock);
        if (lockingStatus == -1) {
            perror("Error while obtaining read lock on employee record!");
            return;
        }
        // reading from employee file
        readBytes = read(employeeFD, &empl, sizeof(struct Employee));
        if (readBytes == -1) {
            perror("Error while reading employee record from file!");
            return;
        }

        // assignment to loan array for employee
        int iter = 0;
        while (empl.loan[iter] != -1)
            iter++;
        if (iter >= MAX_LOANS) {
            // shifting elements one step descarding to oldest loan
            for (iter = 1; iter < MAX_LOANS; iter++) {
                empl.loan[iter - 1] = empl.loan[iter];
            }
            empl.loan[iter - 1] = loan_id;
        } else {
            empl.loan[iter] = loan_id;
        }

        // seeking  to employee record in file
        offset = lseek(employeeFD, empID * sizeof(struct Employee), SEEK_SET);
        if (offset == -1) {
            perror("Error while seeking to required employee record!");
            return;
        }

        // write lock on employee record
        lock.l_type = F_WRLCK;
        lock.l_start = offset;
        lockingStatus = fcntl(employeeFD, F_SETLKW, &lock);
        if (lockingStatus == -1) {
            perror("Error while obtaining write lock on employee record!");
            return;
        }

        // writting to employee record
        writeBytes = write(employeeFD, &empl, sizeof(struct Employee));
        if (writeBytes == -1) {
            perror("Error while writing to employee record to file!");
            return;
        }
        // unlocking
        lock.l_type = F_UNLCK;
        fcntl(loanFD, F_SETLKW, &lock);

        close(employeeFD);

    } else {
        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connFD, "No loan to assign to employee!^", strlen("No loan to assign to employee!^"));
        if (writeBytes == -1) {
            perror("Error writing no loan message  to client!");
            return;
        }
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // dummy read
    }
}

void review_feedback(int connFD) {
    struct Feedback feed;
    char writeBuffer[1000], readBuffer[1000];
    ssize_t writeBytes, readBytes;

    int feedbackFD = open("./records/feedback.txt", O_RDONLY, S_IRWXU);
    if (feedbackFD == -1) {
        perror("Error whiel opening the feedback file");
        return;
    }
    int flag = 0;
    off_t offset = lseek(feedbackFD, 0, SEEK_SET);
    while (EOF) {
        int readBytes = read(feedbackFD, &feed, sizeof(struct Feedback));
        if (readBytes == 0) {
            break;
        }
        if (feed.reviewed == 0) {
            flag = 1;
            bzero(writeBuffer, sizeof(writeBuffer));
            sprintf(writeBuffer, "%s%d \n%s%s \n%s\n", "Feedback Id- ", feed.id, "Feedback - ", feed.text, "----------------------");
            strcat(writeBuffer, "^");
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1) {
                perror("Error sending customer loginID and password to the client!");
                return;
            }

            readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        }
    }
    close(feedbackFD);
    if (flag) {
        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connFD, "Enter the feedback id you want to review", strlen("Enter the feedback id you want to review"));
        if (writeBytes == -1) {
            perror("Error writing feeback id asking message to client!");
            return;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1) {
            perror("Error reading feedback id response from client!");
            return;
        }
        int feedback_id = atoi(readBuffer);

        feedbackFD = open("./records/feedback.txt", O_RDWR, S_IRWXU);
        if (feedbackFD == -1) {
            perror("Error while opening the feedback file");
            return;
        }

        offset = lseek(feedbackFD, feedback_id * sizeof(struct Feedback), SEEK_SET);
        if (offset == -1) {
            perror("Error while seeking to required feedback record!");
            return;
        }

        // changing reviewed bit
        readBytes = read(feedbackFD, &feed, sizeof(struct Feedback));
        if (readBytes == -1) {
            perror("Error in reading form feedback file");
            return;
        }

        feed.reviewed = 1;
        // Lock the record to be write
        struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Feedback), getpid()};
        int lockingStatus = fcntl(feedbackFD, F_SETLKW, &lock);
        if (lockingStatus == -1) {
            perror("Couldn't obtain lock on feedback record!");
            return;
        }
        offset = lseek(feedbackFD, feedback_id * sizeof(struct Feedback), SEEK_SET);
        if (offset == -1) {
            perror("Error while seeking to required feedback record!");
            return;
        }
        // writting to customer file
        writeBytes = write(feedbackFD, &feed, sizeof(struct Feedback));
        if (writeBytes == -1) {
            perror("Error while writing feedabck record to file!");
            return;
        }
        // unlocking
        lock.l_type = F_UNLCK;
        fcntl(feedbackFD, F_SETLKW, &lock);

        close(feedbackFD);
    } else {
        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connFD, "NO feedback to review to review!^", strlen("NO feedback to review to review!^"));
        if (writeBytes == -1) {
            perror("Error writing no feedback message  to client!");
            return;
        }
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // dummy read
    }
}

bool manager_menu(int connFD, int emp_id) {
    printf("Manager Logged in\n");
    ssize_t writeBytes, readBytes;             // Number of bytes read from / written to the client
    char readBuffer[1000], writeBuffer[1000];  // A buffer used for reading
                                               // & writing to the client
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, MANAGER_LOGIN_SUCCESS);
    while (1) {
        strcat(writeBuffer, "\n");
        strcat(writeBuffer, MANAGER_MENU);
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1) {
            perror("Error while writing MANAGER_MENU to client!");
            return false;
        }
        bzero(writeBuffer, sizeof(writeBuffer));

        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1) {
            perror("Error while reading client's choice for MANAGER_MENU");
            return false;
        }

        int choice = atoi(readBuffer);
        switch (choice) {
            case 1:
                activate_deacticated(connFD);
                break;
            case 2:
                view_unassigned_loans(connFD);
                break;
            case 3:
                assign_loans_to_employee(connFD);
                break;
            case 4:
                review_feedback(connFD);
                break;
            case 5:
                em_change_password(connFD, emp_id);
                break;
            case 6:
                writeBytes = write(connFD, MANAGER_LOGOUT, strlen(MANAGER_LOGOUT));
                readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // dummy read
                return true;
            default:
                writeBytes = write(connFD, MANAGER_EXIT, strlen(MANAGER_EXIT));
                return false;
        }
    }

    return true;
}

#endif