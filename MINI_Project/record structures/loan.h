#ifndef LOAN
#define LOAN

struct Loan {
    int id;  // 1, 2, 3 , 4
    long int amount;
    int customer_account_number;
    int status;  // 0-> pending (manager),1->employee , 2-> loan approved(customer), 3->loan rejected(customer)
    //bool assigned;
};

#endif