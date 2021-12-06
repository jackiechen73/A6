#ifndef NAMESERVER_H
#define NAMESERVER_H
#include "vendingMachine.h"
#include "printer.h"

_Task NameServer {
    Printer& printer; // shared printer
    unsigned int numVendingMachines;
    unsigned int numStudents;
    unsigned int* studentAssignments; // vending machine id assigned to each student
    unsigned int VMidx = 0; // counter for assigned vending machine id
    unsigned int studId = 0; // student id - communication variable
    VendingMachine** machineList; // list of vending machines
	void main(); // task main
  public:
	NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents );
	~NameServer();
    void VMregister( VendingMachine * vendingmachine );
	VendingMachine * getMachine( unsigned int id );
	VendingMachine ** getMachineList();
};

#endif