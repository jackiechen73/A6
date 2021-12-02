#ifndef PARENT_H
#define PARENT_H

#include "Printer.h"
#include "Bank.h"
#include "MPRNG.h"

extern MPRNG mprng;     // access MPRNG object from program main

_Task Parent {
	void main();
    Printer* printer;
    Bank* bank;
    unsigned int numStudents;
    unsigned int parentalDelay;
  public:
	Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay );
};

#endif

