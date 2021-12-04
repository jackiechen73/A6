#include "bank.h"

Bank::Bank( unsigned int numStudents ) : numStudents(numStudents), balances( new unsigned int[numStudents] ), withdrawBench( new uCondition[numStudents] ) {}

void Bank::deposit( unsigned int id, unsigned int amount ) {
    balances[id] += amount;
    if ( !withdrawBench[id].empty() && withdrawBench[id].front() >= balances[id] ) {
        withdrawBench[id].signal();
    } // if
}

void Bank::withdraw( unsigned int id, unsigned int amount ) {
    if ( balances[id] < amount ) {
        withdrawBench[id].wait( amount );
    } // if
    balances[id] -= amount;
    if ( !withdrawBench[id].empty() && withdrawBench[id].front() >= balances[id] ) {
        withdrawBench[id].signal();
    } // if
}