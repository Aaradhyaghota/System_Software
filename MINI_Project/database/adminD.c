#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../record structures/admin.h"
void main() {
    struct Admin admin;
    char writeBuffer[1000];

    int adminFD = open("../records/admin.txt", O_RDONLY, S_IRWXU);
    if (adminFD == -1) {
        perror("Error while opening file");
        return;
    }
    int offset = lseek(adminFD, 0, SEEK_SET);
    while (EOF) {
        int readBytes = read(adminFD, &admin, sizeof(struct Admin));
        if (readBytes == 0) {
            break;
        }
        bzero(writeBuffer, sizeof(writeBuffer));
        sprintf(writeBuffer, "%s%d \n%s%s \n%s%s \n%s%s \n%s\n", "Admin Id- ", admin.id, "Admin name - ",
                admin.name, "Admin login - ", admin.login, "Admin password - ", admin.password, "----------------- ");
        printf("%s", writeBuffer);
    }
}