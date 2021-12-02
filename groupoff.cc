#include "Groupoff.h"

Groupoff::Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay )
: printer(prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay), WATCardList( new WATCard::FWATCard[numStudents] ) {}

WATCard::FWATCard Groupoff::giftCard() {
    return WATCardlist[assigned];
}

void Groupoff::main() {
    printer->print(Printer::Kind::Groupoff, 'S'); // start
    // each student obtains a future
    while ( assigned < numStudents ) {
        _Accept( giftCard ); 
        assigned += 1;
    } // while

    assigned -= 1;
    // add money to each watcard
    while ( assigned >= 0 ) {
        _Accept( ~Groupoff ) {
            break; // break for loop if destructor is called
        } _Else {
            WATCard * watCard = new WATCard(); // create new watcard
			watCard->deposit(sodaCost); // deposit money into watcard
			yield(groupOffDelay); // delay
            WATCardList[assigned].delivery(watCard); // deliver watcard
			printer->print(Printer::Kind::Groupoff, 'D', sodaCost); // deliver
            asigned -= 1;
        } // _Accept
    } // while
    printer->print(Printer::Kind::Groupoff, 'F'); // finish
}