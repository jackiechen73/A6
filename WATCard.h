#ifndef WATCARD_H
#define WATCARD_H

#include <uFuture.h>

/***** WATCard *****
 * The WATCard represents the watcards issued by the WATCard office.
 * Couriers will deposit money from the bank into the cards. 
 * Vending machines will withdraw money from the cards 
 * when a student makes a purchase.
 ****************************/
class WATCard {
	WATCard( const WATCard & ) = delete;	// prevent copying
	WATCard & operator=( const WATCard & ) = delete;
    unsigned int balance;                   // current balance in WATCard
  public:
	typedef Future_ISM<WATCard *> FWATCard;	// future watcard pointer
	WATCard();
	void deposit( unsigned int amount );    // deposit specified amount
	void withdraw( unsigned int amount );   // withdraw specified amount
	unsigned int getBalance();              // return card balance
}; // WATCard

#endif // WATCARD_H
