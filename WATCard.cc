#include "WATCard.h"

/***** WATCard::WATCard *****
 * This is the constructor for the WATCard class.
 ****************************/
WATCard::WATCard() : balance(0) {}

/***** WATCard::deposit *****
 * Deposits the given amount into the WATCard.
 * amount: amount to be deposited
 ****************************/
void WATCard::deposit( unsigned int amount ) {
    balance += amount;
}

/***** WATCard::deposit *****
 * Withdraws the given amount into the WATCard.
 * You should call WATCard::getBalance before withdrawing
 * to ensure sufficient funds.
 * amount: amount to be withdrawn
 ****************************/
void WATCard::withdraw( unsigned int amount ) {
    balance -= amount;
}

/***** WATCard::getBalance *****
 * Returns the current balance in the WATCard.
 ****************************/
unsigned int WATCard::getBalance() {
    return balance;
}