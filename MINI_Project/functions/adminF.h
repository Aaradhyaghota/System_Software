#ifndef ADMIN_FUNCTIONS
#define ADMIN_FUNCTIONS

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
#include "../record structures/transaction.h"
#include "./server_constants.h"

// Function Prototypes =================================
bool login_handler(int connFD);
void add_new_employee(int connFD);
bool modify_customer_details(int connFD);
bool admin_operation_handler(int connFD);
// int add_customer(int connFD, bool isPrimary, int newAccountNumber);
// bool delete_account(int connFD);
// bool modify_customer_info(int connFD);

// =====================================================

// Function Definition =================================

// =====================================================
struct Admin admin;

bool login_handler(int connFD) {
    ssize_t readBytes, writeBytes;             // Number of bytes written to / read from the socket
    char readBuffer[1000], writeBuffer[1000];  // Buffer for reading from / writing to the client
    char tempBuffer[1000];

    bzero(readBuffer, sizeof(readBuffer));
    bzero(writeBuffer, sizeof(writeBuffer));

    strcpy(writeBuffer, ADMIN_LOGIN_WELCOME);

    // Append the request for LOGIN ID message
    strcat(writeBuffer, "\n");
    strcat(writeBuffer, LOGIN_ID);

    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1) {
        perror("Error writing WELCOME & LOGIN_ID message to the client!");
        return false;
    }

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
    int adminFileFD = open(ADMIN_FILE, O_RDONLY);
    if (adminFileFD == -1) {
        perror("Error opening employee file in read mode!");
        return false;
    }
    // reading data from admin.txt to check user existance
    off_t offset = lseek(adminFileFD, ID * sizeof(struct Admin), SEEK_SET);
    if (offset >= 0) {
        struct flock lock = {F_RDLCK, SEEK_SET, ID * sizeof(struct Admin), sizeof(struct Admin), getpid()};

        int lockingStatus = fcntl(adminFileFD, F_SETLKW, &lock);
        if (lockingStatus == -1) {
            perror("Error obtaining read lock on admin record!");
            return false;
        }

        readBytes = read(adminFileFD, &admin, sizeof(struct Admin));
        if (readBytes == -1) {
            perror("Error reading admin record from file!");
        }

        lock.l_type = F_UNLCK;
        fcntl(adminFileFD, F_SETLK, &lock);

        if (strcmp(admin.login, readBuffer) == 0)
            userFound = true;

        close(adminFileFD);
    } else {
        writeBytes = write(connFD, ADMIN_LOGIN_ID_DOESNT_EXIT, strlen(ADMIN_LOGIN_ID_DOESNT_EXIT));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
    }

    if (userFound) {
        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connFD, PASSWORD, strlen(PASSWORD));
        if (writeBytes == -1) {
            perror("Error writing PASSWORD message to client!");
            return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == 1) {
            perror("Error reading password from the client!");
            return false;
        }

        char hashedPassword[1000];
        strcpy(hashedPassword, crypt(readBuffer, SALT_BAE));

        if (strcmp(hashedPassword, admin.password) == 0) {
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

void add_new_employee(int connFD) {
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Employee newEmployee, previousEmployee;

    // default assignment to variables
    for (int i = 0; i < MAX_LOANS; i++) {
        newEmployee.loan[i] = -1;
    }
    // NAME
    sprintf(writeBuffer, "%s%s", ADMIN_ADD_EMPLOYEE, ADMIN_ADD_EMPLOYEE_NAME);

    writeBytes = write(connFD, writeBuffer, sizeof(writeBuffer));
    if (writeBytes == -1) {
        perror("Error writing ADMIN_ADD_EMPLOYEE_NAME message to client!");
        return;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error reading employee name response from client!");
        return;
    }
    bzero(newEmployee.name, sizeof(newEmployee.name));
    strcpy(newEmployee.name, readBuffer);

    // GENDER
    writeBytes = write(connFD, ADMIN_ADD_EMPLOYEE_GENDER, strlen(ADMIN_ADD_EMPLOYEE_GENDER));
    if (writeBytes == -1) {
        perror("Error writing ADMIN_ADD_EMPLOYEE_GENDER message to client!");
        return;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error reading employee gender response from client!");
        return;
    }

    if (readBuffer[0] == 'M' || readBuffer[0] == 'F' || readBuffer[0] == 'O')
        newEmployee.gender = readBuffer[0];
    else {
        writeBytes = write(connFD, ADMIN_ADD_EMPLOYEE_WRONG_GENDER, strlen(ADMIN_ADD_EMPLOYEE_WRONG_GENDER));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        return;
    }

    // AGE
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, ADMIN_ADD_EMPLOYEE_AGE);
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1) {
        perror("Error writing ADMIN_ADD_CUSTOMER_AGE message to client!");
        return;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error reading employee age response from client!");
        return;
    }

    int employeeAge = atoi(readBuffer);
    if (employeeAge == 0) {
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
    newEmployee.age = employeeAge;

    // role
    writeBytes = write(connFD, ADMIN_ADD_EMPLOYEE_ROLE, strlen(ADMIN_ADD_EMPLOYEE_ROLE));
    if (writeBytes == -1) {
        perror("Error writing ADMIN_ADD_EMPLOYEE_ROLE message to client!");
        return;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error reading employee gender response from client!");
        return;
    }

    if (readBuffer[0] == 'M' || readBuffer[0] == 'E') {
        if (readBuffer[0] == 'M')
            newEmployee.role = 1;  // manager
        else
            newEmployee.role = 0;  // employee
    } else {
        writeBytes = write(connFD, ADMIN_ADD_EMPLOYEE_WRONG_ROLE, strlen(ADMIN_ADD_EMPLOYEE_WRONG_ROLE));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        return;
    }

        char hashedPassword[1000];
    strcpy(hashedPassword, crypt(AUTOGEN_PASSWORD, SALT_BAE));
    strcpy(newEmployee.password, hashedPassword);

    // reading from file
    int employeeFileDescriptor = open(EMPLOYEE_FILE, O_RDONLY);
    if (employeeFileDescriptor == -1 && errno == ENOENT) {
        // Employee file was never created
        newEmployee.id = 0;
    } else if (employeeFileDescriptor == -1) {
        perror("Error while opening customer file");
        return;
    } else {
        int offset = lseek(employeeFileDescriptor, -sizeof(struct Employee), SEEK_END);
        if (offset == -1) {
            perror("Error seeking to last Customer record!");
            return;
        }

        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Employee), getpid()};
        int lockingStatus = fcntl(employeeFileDescriptor, F_SETLKW, &lock);
        if (lockingStatus == -1) {
            perror("Error obtaining read lock on Customer record!");
            return;
        }

        readBytes = read(employeeFileDescriptor, &previousEmployee, sizeof(struct Employee));
        if (readBytes == -1) {
            perror("Error while reading Customer record from file!");
            return;
        }

        lock.l_type = F_UNLCK;
        fcntl(employeeFileDescriptor, F_SETLK, &lock);

        close(employeeFileDescriptor);

        newEmployee.id = previousEmployee.id + 1;
    }

    // LOGIN _ PASSWORD
    bzero(newEmployee.login, sizeof(newEmployee.login));
    strcpy(newEmployee.login, newEmployee.name);
    strcat(newEmployee.login, "-");
    sprintf(writeBuffer, "%d", newEmployee.id);
    strcat(newEmployee.login, writeBuffer);

    // WRITTING TO FILE
    employeeFileDescriptor = open(EMPLOYEE_FILE, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
    if (employeeFileDescriptor == -1) {
        perror("Error while creating / opening employee file!");
        return;
    }

    writeBytes = write(employeeFileDescriptor, &newEmployee, sizeof(newEmployee));
    if (writeBytes == -1) {
        perror("Error while writing Employee record to file!");
        return;
    }

    close(employeeFileDescriptor);

    // SHOWING CREDENTIALS
    bzero(writeBuffer, sizeof(writeBuffer));
    sprintf(writeBuffer, "%s%s-%d\n%s%s", ADMIN_ADD_EMPLOYEE_AUTOGEN_LOGIN, newEmployee.name, newEmployee.id, ADMIN_ADD_EMPLOYEE_AUTOGEN_PASSWORD, AUTOGEN_PASSWORD);
    strcat(writeBuffer, "^");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1) {
        perror("Error sending customer loginID and password to the client!");
        return;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read

    return;
}

bool modify_customer_details(int connFD) {
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

bool modify_employee_details(int connFD) {
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Employee employee;

    writeBytes = write(connFD, ADMIN_MOD_EMPLOYEE_ID, strlen(ADMIN_MOD_EMPLOYEE_ID));
    if (writeBytes == -1) {
        perror("Error while writing ADMIN_MOD_EMPLOYEE_ID message to client!");
        return false;
    }
    // reading account no
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error while reading customer ID from client!");
        return false;
    }

    int employeeID = atoi(readBuffer);
    // 2printf("%d\n", customerID);
    int employeeFileDescriptor = open(EMPLOYEE_FILE, O_RDONLY);
    if (employeeFileDescriptor == -1) {
        // Customer File doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, EMPLOYEE_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1) {
            perror("Error while writing CUSTOMER_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        return false;
    }

    off_t offset = lseek(employeeFileDescriptor, employeeID * sizeof(struct Employee), SEEK_SET);
    if (errno == EINVAL) {
        // Customer record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, EMPLOYEE_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1) {
            perror("Error while writing EMPLOYEE_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        return false;
    } else if (offset == -1) {
        perror("Error while seeking to required customer record!");
        return false;
    }

    // Lock the record to be read
    struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Employee), getpid()};
    int lockingStatus = fcntl(employeeFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1) {
        perror("Couldn't obtain lock on customer record!");
        return false;
    }
    // reading of file to struct
    readBytes = read(employeeFileDescriptor, &employee, sizeof(struct Employee));
    if (readBytes == -1) {
        perror("Error while reading customer record from the file!");
        return false;
    }

    // Unlock the record
    lock.l_type = F_UNLCK;
    fcntl(employeeFileDescriptor, F_SETLK, &lock);

    close(employeeFileDescriptor);

    // dispaly menu
    writeBytes = write(connFD, ADMIN_MOD_EMPLOYEE_MENU, strlen(ADMIN_MOD_EMPLOYEE_MENU));
    if (writeBytes == -1) {
        perror(
            "Error while writing ADMIN_MOD_EMPLOYEE_MENU message to client!");
        return false;
    }
    // reading input 1,2,.....
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error while getting employee modification menu choice from client!");
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
            writeBytes = write(connFD, ADMIN_MOD_EMPLOYEE_NEW_NAME, strlen(ADMIN_MOD_EMPLOYEE_NEW_NAME));
            if (writeBytes == -1) {
                perror("Error while writing ADMIN_MOD_EMPLOYEE_NEW_NAME message to client!");
                return false;
            }
            readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
            if (readBytes == -1) {
                perror("Error while getting response for employee's new name from client!");
                return false;
            }
            bzero(employee.name, sizeof(employee.name));
            strcpy(employee.name, readBuffer);
            break;
        case 2:
            writeBytes = write(connFD, ADMIN_MOD_EMPLOYEE_NEW_AGE, strlen(ADMIN_MOD_EMPLOYEE_NEW_AGE));
            if (writeBytes == -1) {
                perror("Error while writing ADMIN_MOD_EMPLOYEE_NEW_AGE message to client!");
                return false;
            }
            readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
            if (readBytes == -1) {
                perror("Error while getting response for employee's new age from client!");
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

            employee.age = updatedAge;
            break;
        case 3:
            writeBytes = write(connFD, ADMIN_MOD_EMPLOYEE_NEW_GENDER, strlen(ADMIN_MOD_EMPLOYEE_NEW_GENDER));
            if (writeBytes == -1) {
                perror("Error while writing ADMIN_MOD_EMPLOYEE_NEW_GENDER message to client!");
                return false;
            }
            readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
            if (readBytes == -1) {
                perror("Error while getting response for EMPLOYEE's new gender from client!");
                return false;
            }
            employee.gender = readBuffer[0];
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

    employeeFileDescriptor = open(EMPLOYEE_FILE, O_WRONLY);
    if (employeeFileDescriptor == -1) {
        perror("Error while opening employee file");
        return false;
    }
    offset = lseek(employeeFileDescriptor, employeeID * sizeof(struct Employee), SEEK_SET);
    if (offset == -1) {
        perror("Error while seeking to required employee record!");
        return false;
    }
    // locking
    lock.l_type = F_WRLCK;
    lock.l_start = offset;
    lockingStatus = fcntl(employeeFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1) {
        perror("Error while obtaining write lock on employee record!");
        return false;
    }
    // writting
    writeBytes = write(employeeFileDescriptor, &employee, sizeof(struct Employee));
    if (writeBytes == -1) {
        perror("Error while writing update employee info into file");
    }
    // unlocking
    lock.l_type = F_UNLCK;
    fcntl(employeeFileDescriptor, F_SETLKW, &lock);

    close(employeeFileDescriptor);

    writeBytes = write(connFD, ADMIN_MOD_EMPLOYEE_SUCCESS, strlen(ADMIN_MOD_EMPLOYEE_SUCCESS));
    if (writeBytes == -1) {
        perror("Error while writing ADMIN_MOD_EMPLOYEE_SUCCESS message to client!");
        return false;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read

    return true;
}

void manage_role(int connFD) {
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Employee newEmployee;
    // enter employee id
    bzero(writeBuffer, sizeof(writeBuffer));
    writeBytes = write(connFD, ADMIN_MANG_EMPLOYEE_ROLE, strlen(ADMIN_MANG_EMPLOYEE_ROLE));
    if (writeBytes == -1) {
        perror("Error while writing ADMIN_MANG_EMPLOYEE_ROLE message to client!");
        return;
    }
    // reading account no.
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error while reading employee ID from client!");
        return;
    }
    // opening the file
    int employeeFileDescriptor = open(EMPLOYEE_FILE, O_RDONLY);
    if (employeeFileDescriptor == -1) {
        perror("Error while opening employee file");
        return;
    }
    // seekinng to that id
    int employeeID = atoi(readBuffer);
    off_t offset = lseek(employeeFileDescriptor, employeeID * sizeof(struct Employee), SEEK_SET);
    if (errno == EINVAL) {
        // employee record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, EMPLOYEE_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1) {
            perror("Error while writing EMPLOYEE_ID_DOESNT_EXIT message to client!");
            return;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        return;
    } else if (offset == -1) {
        perror("Error while seeking to required employee record!");
        return;
    }

    // Lock the record to be read
    struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Employee), getpid()};
    int lockingStatus = fcntl(employeeFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1) {
        perror("Couldn't obtain lock on employee record!");
        return;
    }

    // reading of file to struct
    readBytes = read(employeeFileDescriptor, &newEmployee, sizeof(struct Employee));
    if (readBytes == -1) {
        perror("Error while reading employee record from the file!");
        return;
    }

    // Unlock the record
    lock.l_type = F_UNLCK;
    fcntl(employeeFileDescriptor, F_SETLK, &lock);
    close(employeeFileDescriptor);
    // asking for role
    bzero(writeBuffer, sizeof(writeBuffer));
    writeBytes = write(connFD, "Enter 'M' for changing role to manager and 'E' for changing role to employee", strlen("Enter 'M' for changing role to manager and 'E' for changing role to employee"));
    if (writeBytes == -1) {
        perror("Error while writing message to client!");
        return;
    }
    // reading role
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error while reading role from client!");
        return;
    }

    // make changes
    if (readBuffer[0] == 'M' || readBuffer[0] == 'E') {
        if (readBuffer[0] == 'M')
            newEmployee.role = 1;  // manager
        else
            newEmployee.role = 0;  // employee
    } else {
        writeBytes = write(connFD, ADMIN_ADD_EMPLOYEE_WRONG_ROLE, strlen(ADMIN_ADD_EMPLOYEE_WRONG_ROLE));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
        return;
    }

    employeeFileDescriptor = open(EMPLOYEE_FILE, O_WRONLY);
    if (employeeFileDescriptor == -1) {
        perror("Error while opening employeee file");
        return;
    }
    offset = lseek(employeeFileDescriptor, employeeID * sizeof(struct Employee), SEEK_SET);
    if (offset == -1) {
        perror("Error while seeking to required employee record!");
        return;
    }
    // locking
    lock.l_type = F_WRLCK;
    lock.l_start = offset;
    lockingStatus = fcntl(employeeFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1) {
        perror("Error while obtaining write lock on employee record!");
        return;
    }

    // writting the employee structure
    writeBytes = write(employeeFileDescriptor, &newEmployee, sizeof(struct Employee));
    if (writeBytes == -1) {
        perror("Error while writing update employee info into file");
        return;
    }
    // unlocking
    lock.l_type = F_UNLCK;
    fcntl(employeeFileDescriptor, F_SETLKW, &lock);

    close(employeeFileDescriptor);

    char x[20];
    if (newEmployee.role == 1) {
        strcpy(x, "Manager");
    } else
        strcpy(x, "Employee");

    // showing current role
    strcpy(writeBuffer, "Current role is : ");
    strcat(writeBuffer, x);
    strcat(writeBuffer, "!^");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1) {
        perror("Error while writing message to client!");
        return;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read
    // priting success
    writeBytes = write(connFD, "Successfully changed role!^", strlen("Successfully changed role!^"));
    if (writeBytes == -1) {
        perror("Error while writing message to client!");
        return;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // Dummy read

    return;
}

void Achange_password(int connFD) {
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000], hashedPassword[1000];

    char newPassword[1000];
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
    if (strcmp(crypt(readBuffer, SALT_BAE), admin.password) == 0) {
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
            bzero(admin.password, sizeof(admin.password));
            strcpy(admin.password, newPassword);

            int adminFileDescriptor = open(ADMIN_FILE, O_WRONLY);
            if (adminFileDescriptor == -1) {
                perror("Error opening customer file!");
                return;
            }

            off_t offset = lseek(adminFileDescriptor, admin.id * sizeof(struct Admin), SEEK_SET);
            if (offset == -1) {
                perror("Error seeking to the customer record!");
                return;
            }

            struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Admin), getpid()};
            int lockingStatus = fcntl(adminFileDescriptor, F_SETLKW, &lock);
            if (lockingStatus == -1) {
                perror("Error obtaining write lock on customer record!");
                return;
            }

            writeBytes = write(adminFileDescriptor, &admin, sizeof(struct Admin));
            if (writeBytes == -1) {
                perror("Error storing updated customer password into customer record!");
                return;
            }

            lock.l_type = F_UNLCK;
            lockingStatus = fcntl(adminFileDescriptor, F_SETLK, &lock);

            close(adminFileDescriptor);

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

bool admin_operation(int connFD) {
    if (login_handler(connFD)) {
        printf("Admin Logged in.\n");
        ssize_t writeBytes, readBytes;             // Number of bytes read from / written to the client
        char readBuffer[1000], writeBuffer[1000];  // A buffer used for reading
                                                   // & writing to the client
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, ADMIN_LOGIN_SUCCESS);
        while (1) {
            strcat(writeBuffer, "\n");
            strcat(writeBuffer, ADMIN_MENU);
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1) {
                perror("Error while writing ADMIN_MENU to client!");
                return false;
            }
            bzero(writeBuffer, sizeof(writeBuffer));

            readBytes = read(connFD, readBuffer, sizeof(readBuffer));
            if (readBytes == -1) {
                perror("Error while reading client's choice for ADMIN_MENU");
                return false;
            }

            int choice = atoi(readBuffer);
            switch (choice) {
                case 1:
                    add_new_employee(connFD);
                    break;
                case 2:
                    modify_customer_details(connFD);
                    break;
                case 3:
                    modify_employee_details(connFD);
                    break;
                case 4:
                    manage_role(connFD);
                    break;
                case 5:
                    Achange_password(connFD);
                    break;
                case 6:
                    writeBytes = write(connFD, ADMIN_LOGOUT, strlen(ADMIN_LOGOUT));
                    readBytes = read(connFD, readBuffer, sizeof(readBuffer));  // dummy read
                    return true;
                default:
                    writeBytes = write(connFD, ADMIN_EXIT, strlen(ADMIN_EXIT));
                    return false;
            }
        }
    } else {
        // ADMIN LOGIN FAILED
        return true;
    }
}

#endif