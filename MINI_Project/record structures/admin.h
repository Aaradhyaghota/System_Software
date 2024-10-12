#ifndef ADMIN_RECORD
#define ADMIN_RECORD


struct Admin {
    int id;  // 0, 1, 2 ....
    char name[25];
    // Login Credentials
    char login[30];  // Format : name-id (name will the first word in the structure member `name`)
    char password[30];
};

#endif