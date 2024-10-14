#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../record structures/customer.h"
void main() {
    struct Customer customer;
    char writeBuffer[1000];

    int customerFD = open("../records/customer.txt", O_RDONLY, S_IRWXU);
    if (customerFD == -1) {
        perror("Error while opening file");
        return;
    }
    int offset = lseek(customerFD, 0, SEEK_SET);
    while (EOF) {
        int readBytes = read(customerFD, &customer, sizeof(struct Customer));
        if (readBytes == 0) {
            break;
        }
        bzero(writeBuffer, sizeof(writeBuffer));
        sprintf(writeBuffer, "%s%d \n%s%s \n%s%c \n%s%d \n%s%s \n%s%s \n%s%d \n%s%d \n%s%d \n", "customer Id- ",
                customer.id, "customer name - ", customer.name, "customer gendeer - ", customer.gender,
                "customer age - ", customer.age, "customer login - ", customer.login, "customer password - ",
                customer.password, "customer Loan Status - ", customer.loan_status, "customer Active - ", customer.active, "customer balance - ", customer.balance);
        printf("%s", writeBuffer);
        for (int i = 0; i < 10; i++) {
            printf("%d ", customer.transactions[i]);
        }
        printf("\n---------------------------\n");
    }
}