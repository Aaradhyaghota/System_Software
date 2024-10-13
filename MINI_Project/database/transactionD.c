#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../record structures/transaction.h"
void main() {
    struct Transaction trans;
    char writeBuffer[1000];

    int transFD = open("../records/transactions.txt", O_RDONLY, S_IRWXU);
    if (transFD == -1) {
        perror("Error while opening file");
        return;
    }
    int offset = lseek(transFD, 0, SEEK_SET);
    while (EOF) {
        int readBytes = read(transFD, &trans, sizeof(struct Transaction));
        if (readBytes == 0) {
            break;
        }
        bzero(writeBuffer, sizeof(writeBuffer));
        sprintf(writeBuffer, "%s%d \n%s%d \n%s%d \n%s%d \n%s%d \n%s\n", "trans Id- ", trans.transactionID, "trans customer id - ",
                trans.customer_id, "trans operation - ", trans.operation, "trans old balance - ", trans.oldBalance,
                "trans newbalance - ", trans.newBalance, "----------------- ");
        printf("%s", writeBuffer);
    }
}

// operation  0 -> Withdraw, 1 -> Deposit , 2->debit , 3-> credit