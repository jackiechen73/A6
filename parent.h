#ifndef PARENT_H
#define PARENT_H

#include "printer.h"
#include "Bank.h"
#include "MPRNG.h"

extern MPRNG mprng;     // access MPRNG object from program main

_Task Parent {
    private:
        Printer & printer;
        Bank & bank;
        unsigned int numStudents;
        unsigned int parentalDelay;
	    void main(); // task main
    public:
	    Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay );
};

#endif

