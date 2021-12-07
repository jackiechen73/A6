#include "parent.h"

/***** Parent::Parent *****
 * This is the constructor of the parent class.
 * printer: shared printer
 * bank: shared bank
 * numStudents: number of students
 * parentalDelay: delay between each deposit
 ****************************/
Parent::Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay )
: printer( prt ), bank( bank ), numStudents( numStudents ), parentalDelay( parentalDelay ) {}

/***** Parent::main *****
 * This is the task main.
 * The parent periodically adds a random amount 
 * into a random student's bank account.
 ****************************/
void Parent::main() {
    printer.print(Printer::Kind::Parent, 'S'); // start
	for ( ;; ) {
		_Accept(~Parent) {
			break; // out of while loop
		} _Else	{
            yield( parentalDelay );
            // generate student number in range [0,numStudents)
			unsigned int student = mprng( numStudents - 1 );
            // generate amount in range [1,3]
			unsigned int amount = mprng( 1, 3 ); 
            // deposit into bank account
			bank.deposit( student, amount );
            printer.print( Printer::Kind::Parent, 'D', student, amount ); 
		} // _Accept
	} // for
    printer.print( Printer::Kind::Parent, 'F' ); // finish
}