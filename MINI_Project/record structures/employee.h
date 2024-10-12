#ifndef EMPLOYEE_RECORD
#define EMPLOYEE_RECORD
#define MAX_LOANS 10

struct Employee {
    int id;  // 0, 1, 2, 3, ....
    char name[25];
    char gender;  // M -> Male, F -> Female, O -> Other
    int age;
    // Login Credentials
    char login[30];  // Format : name-id (name will the first word in the
                     // structure member `name`)
    char password[30];
    bool role;  // 0->employee , 1->manager.
    int loan[MAX_LOANS];
};

#endif
