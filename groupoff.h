#ifndef GROUPOFF_H
#define GROUPOFF_H

#include "printer.h"
#include "WATCard.h"
#include "MPRNG.h"
#include <memory.h>

extern MPRNG mprng;     // access MPRNG object from program main

_Task Groupoff {
	void main();
    Printer * printer;
    unsigned int numStudents;
    unsigned int sodaCost;
    unsigned int groupoffDelay;
    unsigned int unassigned = 0; // index of first unassigned WATCard
    std::unique_ptr<WATCard::FWATCard []> WATCardList; 
  public:
	Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
	WATCard::FWATCard giftCard();
};

#endif
