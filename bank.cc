#include "bank.h"

/***** Bank::Bank *****
 * This the the constructor for the Bank monitor.
 * It initializes all student balances to 0.
 * numStudents: total number of students 
 ****************************/
Bank::Bank( unsigned int numStudents ) : numStudents( numStudents ), balances( new unsigned int[numStudents]{0} ), withdrawBench( new uCondition[numStudents] ) {}

/***** Bank::~Bank *****
 * This is the deconstructor for the Bank monitor. 
 ****************************/
Bank::~Bank() {
    delete [] balances;
    delete [] withdrawBench;
}

/***** Bank::deposit *****
 * Deposit the specified amount to the specified student.
 * id: student id
 * amount: amount to be deposited
 ****************************/
void Bank::deposit( unsigned int id, unsigned int amount ) {
    balances[id] += amount; // deposit amount
    // if sufficient funds, wake up withdraw call
    if ( !withdrawBench[id].empty() && withdrawBench[id].front() <= balances[id] ) {
        withdrawBench[id].signal();
    } // if
}

/***** Bank::withdraw *****
 * Withdraw the specified amount from the specified student.
 * This function will block until there is sufficient funds.
 * id: student id
 * amount: amount to be deposited
 ****************************/
void Bank::withdraw( unsigned int id, unsigned int amount ) {
    if ( balances[id] < amount ) { // insufficient funds?
        withdrawBench[id].wait( amount );
    } // if
    balances[id] -= amount;
    // if sufficient funds, wake up next withdraw call
    if ( !withdrawBench[id].empty() && withdrawBench[id].front() <= balances[id] ) {
        withdrawBench[id].signal();
    } // if
}