#ifndef GROUPOFF_H
#define GROUPOFF_H

#include "printer.h"
#include "WATCard.h"
#include "MPRNG.h"

extern MPRNG mprng;     // access MPRNG object from program main

_Task Groupoff {
    private:
        void main();
        Printer & printer;
        unsigned int numStudents;
        unsigned int sodaCost;
        unsigned int groupoffDelay;
        unsigned int cardIdx = 0; // index of first unassigned WATCard
        WATCard::FWATCard * WATCardList; 
    public:
        Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
        ~Groupoff();
        WATCard::FWATCard giftCard();
};

#endif
