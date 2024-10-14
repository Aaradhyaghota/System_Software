#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../record structures/loan.h"
void main() {
    struct Loan loan;
    char writeBuffer[1000];

    int loanFD = open("../records/loan.txt", O_RDONLY, S_IRWXU);
    if (loanFD == -1) {
        perror("Error while opening file");
        return;
    }
    int offset = lseek(loanFD, 0, SEEK_SET);
    while (EOF) {
        int readBytes = read(loanFD, &loan, sizeof(struct Loan));
        if (readBytes == 0) {
            break;
        }
        bzero(writeBuffer, sizeof(writeBuffer));
        sprintf(writeBuffer, "%s%d \n%s%d \n%s%d \n%s%d \n%s\n", "Loan Id- ", loan.id, "Customer id - ",
                loan.customerID, "Loan amount  - ", loan.amount, "Loan status - ", loan.status, "----------------- ");
        printf("%s", writeBuffer);
    }
}