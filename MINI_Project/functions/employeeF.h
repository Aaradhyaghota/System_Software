#ifndef EMPLOYEE_FUNCTIONS
#define EMPLOYEE_FUNCTIONS

#include <errno.h>      // Import for `errno`
#include <fcntl.h>      // Import for `open`
#include <stdbool.h>    // Import for `bool` data type
#include <stdio.h>      // Import for `printf` & `perror`
#include <stdlib.h>     // Import for `atoi`
#include <string.h>     // Import for string functions
#include <sys/stat.h>   // Import for `open`
#include <sys/types.h>  // Import for `open`, `lseek`
#include <unistd.h>     // Import for `read`, `write & `lseek`

#include "../record structures/customer.h"
#include "../record structures/employee.h"
#include "../record structures/transaction.h"
#include "./managerF.h"
#include "./server_constants.h"

// Function Prototypes =================================
bool Elogin_handler(int connFD);
void Eadd_new_customer(int connFD);
bool Emodify_customer_details(int connFD);
// int add_customer(int connFD);
// bool delete_account(int connFD);

// =====================================================

// Function Definition =================================

// =====================================================
struct Employee employee;

bool Elogin_handler(int connFD) {
    ssize_t readBytes, writeBytes;             // Number of bytes written to / read from the socket
    char readBuffer[1000], writeBuffer[1000];  // Buffer for reading from / writing to the client
    char tempBuffer[1000];

    bzero(readBuffer, sizeof(readBuffer));
    bzero(writeBuffer, sizeof(writeBuffer));

    // Get login message for respective user type
    strcpy(writeBuffer, EMPLOYEE_LOGIN_WELCOME);

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

    int employeeFileFD = open(EMPLOYEE_FILE, O_RDONLY);
    if (employeeFileFD == -1) {
        perror("Error opening employee file in read mode!");
        return false;
    }

    off_t offset = lseek(employeeFileFD, ID * sizeof(struct Employee), SEEK_SET);
    if (offset >= 0) {
        struct flock lock = {F_RDLCK, SEEK_SET, ID * sizeof(struct Employee), sizeof(struct Employee), getpid()};

        int lockingStatus = fcntl(employeeFileFD, F_SETLKW, &lock);
        if (lockingStatus == -1) {
            perror("Error obtaining read lock on employee record!");
            return false;
        }

        readBytes = read(employeeFileFD, &employee, sizeof(struct Employee));
        if (readBytes == -1) {
            perror("Error reading customer record from file!");
        }

        lock.l_type = F_UNLCK;
        fcntl(employeeFileFD, F_SETLK, &lock);

        if (strcmp(employee.login, readBuffer) == 0)
            userFound = true;

        close(employeeFileFD);
    } else {
        writeBytes = write(connFD, EMPLOYEE_LOGIN_ID_DOESNT_EXIT, strlen(EMPLOYEE_LOGIN_ID_DOESNT_EXIT));
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

        if (strcmp(hashedPassword, employee.password) == 0) {
            return true;
        }

        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connFD, INVALID_PASSWORD, strlen(INVALID_PASSWORD));
    } else {
        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connFD, INVALID_LOGIN, strlen(INVALID_LOGIN));
    }

    return false;
}

void Eadd_new_customer(int connFD) {
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];
    struct Customer newCustomer, previousCustomer;

    int customerFileDescriptor = open(CUSTOMER_FILE, O_RDONLY);
    if (customerFileDescriptor == -1 && errno == ENOENT) {
        // Customer file was never created
        newCustomer.id = 0;
    } else if (customerFileDescriptor == -1) {
        perror("Error while opening customer file");
        return;
    } else {
        int offset = lseek(customerFileDescriptor, -sizeof(struct Customer), SEEK_END);
        if (offset == -1) {
            perror("Error seeking to last Customer record!");
            return;
        }

        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};
        int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
        if (lockingStatus == -1) {
            perror("Error obtaining read lock on Customer record!");
            return;
        }
        // here while reading without lock will not effect or give any deadlock because id cannot be change once its created.
        readBytes = read(customerFileDescriptor, &previousCustomer, sizeof(struct Customer));
        if (readBytes == -1) {
            perror("Error while reading Customer record from file!");
            return;
        }

        lock.l_type = F_UNLCK;
        fcntl(customerFileDescriptor, F_SETLK, &lock);

        close(customerFileDescriptor);

        newCustomer.id = previousCustomer.id + 1;
    }
    // intitializing all variable of customer
    newCustomer.balance = 0;
    newCustomer.active = true;    // 1->active 0->deactive
    newCustomer.loan_status = 0;  // not applied for loan
    for (int i = 0; i < MAX_TRANSACTIONS; i++) {
        newCustomer.transactions[i] = -1;
    }

    sprintf(writeBuffer, "%s%s", EMPLOYEE_ADD_CUSTOMER, EMPLOYEE_ADD_CUSTOMER_NAME);
    writeBytes = write(connFD, writeBuffer, sizeof(writeBuffer));
    if (writeBytes == -1) {
        perror("Error writing EMPLOYEE_ADD_CUSTOMER_NAME message to client!");
        return;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error reading customer name response from client!");
        return;
    }
    bzero(newCustomer.name, sizeof(newCustomer.name));
    strcpy(newCustomer.name, readBuffer);

    writeBytes = write(connFD, EMPLOYEE_ADD_CUSTOMER_GENDER, strlen(EMPLOYEE_ADD_CUSTOMER_GENDER));
    if (writeBytes == -1) {
        perror("Error writing ADMIN_ADD_CUSTOMER_GENDER message to client!");
        return;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error reading customer gender response from client!");
        return;
    }

    if (readBuffer[0] == 'M' || readBuffer[0] == 'F' || readBuffer[0] == 'O')
        newCustomer.gender = readBuffer[0];
    else {
        writeBytes = write(connFD, EMPLOYEE_ADD_CUSTOMER_WRONG_GENDER, strlen(EMPLOYEE_ADD_CUSTOMER_WRONG_GENDER));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        return;
    }

    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, EMPLOYEE_ADD_CUSTOMER_AGE);
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1) {
        perror("Error writing EMPLOYEE_ADD_CUSTOMER_AGE message to client!");
        return;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error reading customer age response from client!");
        return;
    }

    int customerAge = atoi(readBuffer);
    if (customerAge == 0) {
        // Either client has sent age as 0 (which is invalid) or has entered a
        // non-numeric string
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, ERRON_INPUT_FOR_NUMBER);
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1) {
            perror("Error while writing ERRON_INPUT_FOR_NUMBER message to client!");
            return;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        return;
    }
    newCustomer.age = customerAge;

    bzero(newCustomer.login, sizeof(newCustomer.login));
    strcpy(newCustomer.login, newCustomer.name);
    strcat(newCustomer.login, "-");
    sprintf(writeBuffer, "%d", newCustomer.id);
    strcat(newCustomer.login, writeBuffer);

    char hashedPassword[1000];
    strcpy(hashedPassword, crypt(AUTOGEN_PASSWORD, SALT_BAE));
    strcpy(newCustomer.password, hashedPassword);

    customerFileDescriptor = open(CUSTOMER_FILE, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
    if (customerFileDescriptor == -1) {
        perror("Error while creating / opening customer file!");
        return;
    }
    // seeking  to customer record in file
    off_t offset = lseek(customerFileDescriptor, newCustomer.id * sizeof(struct Customer), SEEK_SET);
    if (offset == -1) {
        perror("Error while seeking to required customer record!");
        return;
    }

    // Lock the record to be write
    struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};
    int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1) {
        perror("Couldn't obtain lock on customer record!");
        return;
    }
    // writting to customer file
    writeBytes = write(customerFileDescriptor, &newCustomer, sizeof(newCustomer));
    if (writeBytes == -1) {
        perror("Error while writing Customer record to file!");
        return;
    }
    // unlocking
    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLKW, &lock);

    close(customerFileDescriptor);

    bzero(writeBuffer, sizeof(writeBuffer));
    sprintf(writeBuffer, "%s%s-%d\n%s%s\n%s%d", EMPLOYEE_ADD_CUSTOMER_AUTOGEN_LOGIN, newCustomer.name, newCustomer.id, EMPLOYEE_ADD_CUSTOMER_AUTOGEN_PASSWORD, AUTOGEN_PASSWORD, EMPLOYEE_AUTOGEN_ACCOUNTNO, 1000 + newCustomer.id);
    strcat(writeBuffer, "^");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1) {
        perror("Error sending customer loginID and password to the client!");
        return;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read

    return;
}

bool Emodify_customer_details(int connFD) {
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Customer customer;

    writeBytes = write(connFD, ADMIN_MOD_CUSTOMER_ID, strlen(ADMIN_MOD_CUSTOMER_ID));
    if (writeBytes == -1) {
        perror("Error while writing ADMIN_MOD_CUSTOMER_ID message to client!");
        return false;
    }
    // reading account no
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error while reading customer ID from client!");
        return false;
    }

    int customerID = atoi(readBuffer);
    customerID = customerID - 1000;
    // 2printf("%d\n", customerID);
    int customerFileDescriptor = open(CUSTOMER_FILE, O_RDONLY);
    if (customerFileDescriptor == -1) {
        // Customer File doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, CUSTOMER_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1) {
            perror("Error while writing CUSTOMER_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        return false;
    }

    off_t offset = lseek(customerFileDescriptor, customerID * sizeof(struct Customer), SEEK_SET);
    if (errno == EINVAL) {
        // Customer record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, CUSTOMER_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1) {
            perror("Error while writing CUSTOMER_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        return false;
    } else if (offset == -1) {
        perror("Error while seeking to required customer record!");
        return false;
    }

    // Lock the record to be read
    struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};
    int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1) {
        perror("Couldn't obtain lock on customer record!");
        return false;
    }
    // reading of file to struct
    readBytes = read(customerFileDescriptor, &customer, sizeof(struct Customer));
    if (readBytes == -1) {
        perror("Error while reading customer record from the file!");
        return false;
    }

    // Unlock the record
    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLK, &lock);

    close(customerFileDescriptor);

    // dispaly menu
    writeBytes = write(connFD, ADMIN_MOD_CUSTOMER_MENU, strlen(ADMIN_MOD_CUSTOMER_MENU));
    if (writeBytes == -1) {
        perror(
            "Error while writing ADMIN_MOD_CUSTOMER_MENU message to client!");
        return false;
    }
    // reading input 1,2,.....
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error while getting customer modification menu choice from client!");
        return false;
    }

    int choice = atoi(readBuffer);
    // printf("%d\n", choice);
    if (choice == 0) {  // A non-numeric string was passed to atoi
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, ERRON_INPUT_FOR_NUMBER);
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1) {
            perror("Error while writing ERRON_INPUT_FOR_NUMBER message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        return false;
    }
    bzero(readBuffer, sizeof(readBuffer));

    switch (choice) {
        case 1:
            writeBytes = write(connFD, ADMIN_MOD_CUSTOMER_NEW_NAME, strlen(ADMIN_MOD_CUSTOMER_NEW_NAME));
            if (writeBytes == -1) {
                perror("Error while writing ADMIN_MOD_CUSTOMER_NEW_NAME message to client!");
                return false;
            }
            readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
            if (readBytes == -1) {
                perror("Error while getting response for customer's new name from client!");
                return false;
            }
            bzero(customer.name, sizeof(customer.name));
            strcpy(customer.name, readBuffer);
            break;
        case 2:
            writeBytes = write(connFD, ADMIN_MOD_CUSTOMER_NEW_AGE, strlen(ADMIN_MOD_CUSTOMER_NEW_AGE));
            if (writeBytes == -1) {
                perror("Error while writing ADMIN_MOD_CUSTOMER_NEW_AGE message to client!");
                return false;
            }
            readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
            if (readBytes == -1) {
                perror("Error while getting response for customer's new age from client!");
                return false;
            }
            int updatedAge = atoi(readBuffer);
            if (updatedAge == 0) {
                // Either client has sent age as 0 (which is invalid) or has
                // entered a non-numeric string
                bzero(writeBuffer, sizeof(writeBuffer));
                strcpy(writeBuffer, ERRON_INPUT_FOR_NUMBER);
                writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
                if (writeBytes == -1) {
                    perror("Error while writing ERRON_INPUT_FOR_NUMBER message to client!");
                    return false;
                }
                readBytes =
                    read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
                return false;
            }

            customer.age = updatedAge;
            break;
        case 3:
            writeBytes = write(connFD, ADMIN_MOD_CUSTOMER_NEW_GENDER, strlen(ADMIN_MOD_CUSTOMER_NEW_GENDER));
            if (writeBytes == -1) {
                perror("Error while writing ADMIN_MOD_CUSTOMER_NEW_GENDER message to client!");
                return false;
            }
            readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
            if (readBytes == -1) {
                perror("Error while getting response for customer's new gender from client!");
                return false;
            }
            customer.gender = readBuffer[0];
            break;
        default:
            bzero(writeBuffer, sizeof(writeBuffer));
            strcpy(writeBuffer, INVALID_MENU_CHOICE);
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1) {
                perror(
                    "Error while writing INVALID_MENU_CHOICE message to "
                    "client!");
                return false;
            }
            readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
            return false;
    }

    customerFileDescriptor = open(CUSTOMER_FILE, O_WRONLY);
    if (customerFileDescriptor == -1) {
        perror("Error while opening customer file");
        return false;
    }
    offset = lseek(customerFileDescriptor, customerID * sizeof(struct Customer), SEEK_SET);
    if (offset == -1) {
        perror("Error while seeking to required customer record!");
        return false;
    }
    // locking
    lock.l_type = F_WRLCK;
    lock.l_start = offset;
    lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1) {
        perror("Error while obtaining write lock on customer record!");
        return false;
    }
    // writting
    writeBytes = write(customerFileDescriptor, &customer, sizeof(struct Customer));
    if (writeBytes == -1) {
        perror("Error while writing update customer info into file");
    }
    // unlocking
    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLKW, &lock);

    close(customerFileDescriptor);

    writeBytes = write(connFD, ADMIN_MOD_CUSTOMER_SUCCESS, strlen(ADMIN_MOD_CUSTOMER_SUCCESS));
    if (writeBytes == -1) {
        perror("Error while writing ADMIN_MOD_CUSTOMER_SUCCESS message to client!");
        return false;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read

    return true;
}

void get_passbook(int connFD) {
    ssize_t writeBytes, readBytes;  // Number of bytes read from / written to the client
    char readBuffer[1000], writeBuffer[1000];
    char tempBuffer[1000];

    bzero(readBuffer, sizeof(readBuffer));
    bzero(writeBuffer, sizeof(writeBuffer));
    writeBytes = write(connFD, "Enter customer account no. whom passbook you want to see", strlen("Enter customer account no. whom passbook you want to see"));
    if (writeBytes == -1) {
        perror("Error writing  message to client!");
        return;
    }
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error reading customer account no. from the client!");
        return;
    }
    int c_ID = atoi(readBuffer);
    c_ID -= 1000;
    transaction_history(connFD, c_ID);
}

bool employee_menu(int connFD) {
    printf("Employee Logged in.\n");
    ssize_t writeBytes, readBytes;             // Number of bytes read from / written to the client
    char readBuffer[1000], writeBuffer[1000];  // A buffer used for reading
                                               // & writing to the client
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, EMPLOYEE_LOGIN_SUCCESS);
    while (1) {
        strcat(writeBuffer, "\n");
        strcat(writeBuffer, EMPLOYEE_MENU);
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1) {
            perror("Error while writing EMPLOYEE_MENU to client!");
            return false;
        }
        bzero(writeBuffer, sizeof(writeBuffer));

        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1) {
            perror("Error while reading client's choice for EMPLOYEE_MENU");
            return false;
        }

        int choice = atoi(readBuffer);
        switch (choice) {
            case 1:
                Eadd_new_customer(connFD);
                break;
            case 2:
                Emodify_customer_details(connFD);
                break;
            case 3:
                // Process Loan Applications
                break;
            case 4:
                // Approve/Reject Loans
                break;
            case 5:
                // View Assigned Loan Applications
                break;
            case 6:
                // get transation of a customer
                get_passbook(connFD);
                break;
            case 7:
                // change password
                // change_password(connFD);
                return false;
            case 8:
                writeBytes = write(connFD, EMPLOYEE_LOGOUT, strlen(EMPLOYEE_LOGOUT));
                return false;
            default:
                writeBytes = write(connFD, EMPLOYEE_EXIT, strlen(EMPLOYEE_EXIT));
                return false;
        }
    }

    return true;
}

bool emp_mag_operation(int connFD, int num) {
    if (Elogin_handler(connFD)) {
        if (employee.role == 0 && num == 0)
            employee_menu(connFD);
        else if (employee.role == 1 && num == 1)
            manager_menu(connFD);
        else
            write(connFD, "Wrong User!!$", strlen("Wrong User!!$"));
    }
    return true;
}

#endif

// employee -->> 1. Himanshu-0  E 2. Raj-1 M

/*
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = ID * sizeof(struct Customer);
    lock.l_len = sizeof(struct Customer);
    lock.l_pid = getpid();
*/