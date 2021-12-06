#include "bank.h"

Bank::Bank( unsigned int numStudents ) : numStudents(numStudents), balances( new unsigned int[numStudents] ), withdrawBench( new uCondition[numStudents] ) {}

Bank::~Bank() {
    delete [] balances;
    delete [] withdrawBench;
}

void Bank::deposit( unsigned int id, unsigned int amount ) {
    balances[id] += amount; // deposit amount
    // if sufficient funds, wake up withdraw
    if ( !withdrawBench[id].empty() && withdrawBench[id].front() >= balances[id] ) {
        withdrawBench[id].signal();
    } // if
}

void Bank::withdraw( unsigned int id, unsigned int amount ) {
    if ( balances[id] < amount ) { // insufficient funds?
        withdrawBench[id].wait( amount );
    } // if
    balances[id] -= amount;
    // if sufficient funds, wake up next withdraw
    if ( !withdrawBench[id].empty() && withdrawBench[id].front() >= balances[id] ) {
        withdrawBench[id].signal();
    } // if
}