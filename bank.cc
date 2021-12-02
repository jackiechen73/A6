#include "Bank.h"

Bank::Bank( unsigned int numStudents ) : numStudents(numStudents), balances( new unsigned int[numStudents] ), withdrawBench( new uCondition[numStudents] ) {}

void Bank::deposit( unsigned int id, unsigned int amount ) {
    balances[id] += amount;
    withdrawBench[id].signal();
}

void Bank::withdraw( unsigned int id, unsigned int amount ) {
    while ( balances[id] < amount ) {
        withdrawBench[id].wait();
    } // while
    balances[id] -= amount;
    withdrawBench[id].signal(); // wake up next withdrawal
}