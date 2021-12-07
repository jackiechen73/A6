#ifndef NAMESERVER_H
#define NAMESERVER_H
#include "vendingMachine.h"
#include "printer.h"

/***** NameServer *****
 * The NameServer tracks the vending machines and their ids.
 * It assigns vending machines to students.
 ****************************/
_Task NameServer {
        Printer& printer;                   // shared printer
        unsigned int numVendingMachines;    // number of vending machines
        unsigned int numStudents;           // number of students
        unsigned int* studentAssignments;   // machine id assigned to student
        unsigned int VMidx = 0;             // machine id (communication variable)
        unsigned int studId = 0;            // student id (communication variable)
        VendingMachine** machineList;       // list of vending machines
        void main();                        // task main
    public:
        NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents );
        ~NameServer();
        void VMregister( VendingMachine * vendingmachine );     // register machine
        VendingMachine * getMachine( unsigned int id );         // get new machine
        VendingMachine ** getMachineList();                     // get machine list
}; // NameServer

#endif // NAMESERVER_H
