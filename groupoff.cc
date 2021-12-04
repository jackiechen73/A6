#include "Groupoff.h"

Groupoff::Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay )
: printer(&prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay), WATCardList( new WATCard::FWATCard[numStudents] ) {}

WATCard::FWATCard Groupoff::giftCard() {
    return WATCardList[cardIdx];
}

void Groupoff::main() {
    printer->print(Printer::Kind::Groupoff, 'S'); // start
    // each student obtains a future
    while ( cardIdx < numStudents ) {
        _Accept( giftCard ); 
        cardIdx += 1;
    } // while

    bool deposited[numStudents] = { false };

    // add money to each watcard
    while ( cardIdx > 0 ) {
        _Accept( ~Groupoff ) {
            break; // break for loop if destructor is called
        } _Else {
            yield(groupoffDelay); // delay
            unsigned int depositIndex = mprng(0, cardIdx-1); // generate random deposit
            // loop through to find the depositIndex-th undeposited card
            for ( unsigned int i = 0; i < numStudents; i += 1 ) {
                if ( deposited[i] ) { continue; }
                if ( depositIndex == 0 ) {
                    WATCard * watCard = new WATCard(); // create new watcard
			        watCard->deposit(sodaCost); // deposit money into watcard
                    WATCardList[i].delivery(watCard); // deliver watcard
                    printer->print(Printer::Kind::Groupoff, 'D', sodaCost); // deliver
                    cardIdx -= 1;
                    deposited[i] = true;
                    break;
                } // if
                depositIndex -= 1;
            } // for
        } // _Accept
    } // while
    printer->print(Printer::Kind::Groupoff, 'F'); // finish
}