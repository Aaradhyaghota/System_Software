#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../record structures/employee.h"
void main() {
    struct Employee employee;
    char writeBuffer[1000];

    int employeeFD = open("../records/employee.txt", O_RDONLY, S_IRWXU);
    if (employeeFD == -1) {
        perror("Error while opening file");
        return;
    }
    int offset = lseek(employeeFD, 0, SEEK_SET);
    while (EOF) {
        int readBytes = read(employeeFD, &employee, sizeof(struct Employee));
        if (readBytes == 0) {
            break;
        }
        bzero(writeBuffer, sizeof(writeBuffer));
        sprintf(writeBuffer, "%s%d \n%s%s \n%s%c \n%s%d \n%s%s \n%s%s \n%s%d \n", "employee Id- ",
                employee.id, "employee name - ", employee.name, "employee gendeer - ", employee.gender,
                "employee age - ", employee.age, "employee login - ", employee.login, "employee password - ",
                employee.password, "employee role - ", employee.role);
        printf("%s", writeBuffer);
        for (int i = 0; i < 10; i++) {
            printf("%d ", employee.loan[i]);
        }
        printf("\n---------------------------\n");
    }
}