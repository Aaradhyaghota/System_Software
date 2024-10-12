#ifndef TRANSACTIONS
#define TRANSACTIONS

#include <time.h>

struct Transaction {
    int transactionID;  // 0, 1, 2, 3 ...
    int customer_id;
    int operation;  // 0 -> Withdraw, 1 -> Deposit , 2->debit , 3-> credit
    int oldBalance;
    int newBalance;
    time_t transactionTime;
};

#endif