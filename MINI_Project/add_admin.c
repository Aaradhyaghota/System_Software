#include <errno.h>      // Import for `errno`
#include <fcntl.h>      // Import for `open`
#include <stdbool.h>    // Import for `bool` data type
#include <stdio.h>      // Import for `printf` & `perror`
#include <stdlib.h>     // Import for `atoi`
#include <string.h>     // Import for string functions
#include <sys/stat.h>   // Import for `open`
#include <sys/types.h>  // Import for `open`, `lseek`
#include <unistd.h>     // Import for `read`, `write & `lseek`

#include "./functions/server_constants.h"
#include "./record structures/admin.h"
void main() {
    ssize_t readBytes, writeBytes;
    struct Admin admin;
    int adminFD = open("./records/admin.txt", O_CREAT | O_APPEND | O_RDWR, S_IRWXU);
    if (adminFD == -1) {
        perror("Error while opening file");
        return;
    }
    off_t offset = lseek(adminFD, -sizeof(struct Admin), SEEK_END);
    if (offset >= 0) {
        // There exists at least one transaction record
        struct Admin prevadmin;
        readBytes = read(adminFD, &prevadmin, sizeof(struct Admin));

        admin.id = prevadmin.id + 1;
    } else
        // No transaction records exist
        admin.id = 0;

    printf("Enter name of admin\n");
    scanf("%[^\n]%*c", admin.name);

    char writeBuffer[1000];
    strcpy(admin.login, admin.name);
    strcat(admin.login, "-");
    sprintf(writeBuffer, "%d", admin.id);
    strcat(admin.login, writeBuffer);

    char hashedPassword[1000];
    strcpy(hashedPassword, crypt(AUTOGEN_PASSWORD, SALT_BAE));
    strcpy(admin.password, hashedPassword);

    writeBytes = write(adminFD, &admin, sizeof(struct Admin));
    if (writeBytes == -1) {
        perror("Error in writting to the file\n");
        return;
    }
    printf("Admin generated with login id : %s\n", admin.login);
    printf("Auto generated Password : %s\n", AUTOGEN_PASSWORD);
}

// aaradhya-0