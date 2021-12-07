#include "groupoff.h"

/***** Groupoff::Groupoff *****
 * This is the constructor for the Groupoff task.
 ****************************/
Groupoff::Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay )
: printer( prt ), numStudents( numStudents ), sodaCost( sodaCost ), 
groupoffDelay( groupoffDelay ), WATCardList( new WATCard::FWATCard[numStudents] ) {}

/***** Groupoff::~Groupoff *****
 * This is the destructor for the Groupoff task.
 ****************************/
Groupoff::~Groupoff() {
    // delete unused giftcards
    for ( unsigned int i = 0 ; i < numStudents; i += 1 ) {
        if ( WATCardList[i].available() ) {
            delete WATCardList[i];
        } // if
    } // for
    
    delete [] WATCardList;
}

/***** Groupoff::giftCard *****
 * Return WATCard future.
 ****************************/
WATCard::FWATCard Groupoff::giftCard() {
    return WATCardList[cardIdx];
}

/***** Groupoff::main *****
 * This is the task main.
 * It starts by accepting a giftCard call from each student.
 * Then, it periodically deposits into a random giftcard 
 * until all cards have been deposited or the destructor is called.
 ****************************/
void Groupoff::main() {
    printer.print(Printer::Kind::Groupoff, 'S'); // start
    // each student obtains a future
    while ( cardIdx < numStudents ) {
        _Accept( giftCard ); 
        cardIdx += 1;
    } // while

    bool deposited[numStudents] = { false }; // track deposited cards
    // use cardIdx to keep track of number of undeposited cards
    while ( cardIdx > 0 ) { // add money to each watcard
        _Accept( ~Groupoff ) {
            break; // break out of for loop
        } _Else {
            yield( groupoffDelay ); // delay between deposits
            // deposit money into the depositIndex-th undeposited card
            unsigned int depositIndex = mprng( 0, cardIdx - 1 ); 
            // find the depositIndex-th undeposited card
            for ( unsigned int i = 0; i < numStudents; i += 1 ) {
                if ( deposited[i] ) { continue; } // skip deposited ones

                // found the depositIndex-th undeposited card?
                if ( depositIndex == 0 ) { 
                    WATCard * watCard = new WATCard();  // create watcard
			        watCard->deposit( sodaCost );       // deposit money
                    WATCardList[i].delivery( watCard ); // deliver watcard
                    printer.print( Printer::Kind::Groupoff, 'D', sodaCost ); // deliver
                    cardIdx -= 1; // decrement unassigned counter
                    deposited[i] = true;
                    break;
                } // if
                
                depositIndex -= 1; // found an empty watcard
            } // for
        } // _Accept
    } // while
    printer.print(Printer::Kind::Groupoff, 'F'); // finish
}