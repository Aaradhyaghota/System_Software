#ifndef CUSTOMER_RECORD
#define CUSTOMER_RECORD
#define MAX_TRANSACTIONS 10

struct Customer {
    int id;  // 0, 1, 2 ....
    char name[25];
    char gender;  // M -> Male, F -> Female, O -> Other
    int age;
    // Login Credentials
    char login[30];  // Format : name-id (name will the first word in the structure member `name`)
    char password[30];
    // Bank data
    bool active;
    int balance;
    int transactions[MAX_TRANSACTIONS];
};

#endif