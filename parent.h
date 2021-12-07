#ifndef PARENT_H
#define PARENT_H

#include "printer.h"
#include "bank.h"
#include "MPRNG.h"

extern MPRNG mprng;     // access MPRNG object from program main

/***** Parent *****
 * The Parent periodically deposits money into a random student's bank account.
 ****************************/
_Task Parent {
    private:
        Printer & printer;          // shared printer
        Bank & bank;                // shared bank
        unsigned int numStudents;   // number of students
        unsigned int parentalDelay; // delay between deposits
	    void main();                // task main
    public:
	    Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay );
}; // Parent

#endif // PARENT_H
