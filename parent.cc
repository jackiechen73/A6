#include "parent.h"

Parent::Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay )
: printer(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay) {}

void Parent::main() {
    printer.print(Printer::Kind::Parent, 'S'); // start
	for ( ;; ) {
		_Accept(~Parent) {
			break; // out of while loop
		} _Else	{
            // generate student number in range [0,numStudents)
			unsigned int student = mprng( numStudents - 1 );
            // generate amount in range [1,3]
			unsigned int amount = mprng( 1, 3 ); 
			yield( parentalDelay );
			bank.deposit( student, amount );
            printer.print( Printer::Kind::Parent, 'D', student, amount ); // deposit
		} // _Accept
	} // for
    printer.print( Printer::Kind::Parent, 'F' ); // finish
}