#ifndef GROUPOFF_H
#define GROUPOFF_H

#include "printer.h"
#include "WATCard.h"
#include "MPRNG.h"

extern MPRNG mprng;     // access MPRNG object from program main

/***** Groupoff *****
 * The Groupoff creates a giftcard for each student.
 * Intially, each student retrieves a giftcard future.
 * Then, Groupoff will periodically deposit money into a random giftcard.  
 ****************************/
_Task Groupoff {
    private:
        Printer & printer;                  // shared printer
        unsigned int numStudents;           // number of students
        unsigned int sodaCost;              // cost of soda
        unsigned int groupoffDelay;         // delay between deposits
        unsigned int cardIdx = 0;           // index of first unassigned WATCard
        WATCard::FWATCard * WATCardList;    // list of giftcard futures
        void main();                        // task main
    public:
        Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
        ~Groupoff();
        WATCard::FWATCard giftCard();       // return giftcard future
}; // Groupoff

#endif // GROUPOFF_H
