#ifndef NAMESERVER_H
#define NAMESERVER_H
#include "vendingMachine.h"

_Task NameServer {
    Printer& printer;
    unsigned int numVendingMachines;
    unsigned int numStudents;
    unsigned int* studentAssignments;
    unsigned int VMidx = 0;
    unsigned int studId = 0;
    VendingMachine** machineList;
	void main();
  public:
	NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents );
	void VMregister( VendingMachine * vendingmachine );
	VendingMachine * getMachine( unsigned int id );
	VendingMachine ** getMachineList();
};

#endif