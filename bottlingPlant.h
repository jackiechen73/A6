#ifndef BOTTLINGPLANT_H
#define BOTTLINGPLANT_H
#include "printer.h"
#include "nameServer.h"
#include "MPRNG.h"

extern MPRNG mprng;     // access MPRNG object from program main

_Task BottlingPlant {
    Printer & printer;                  // shared printer
    NameServer & nameServer;            // shared name server
    unsigned int numVendingMachines;    // number of vending machines
    unsigned int maxShippedPerFlavour; 
    unsigned int maxStockPerFlavour;
    unsigned int timeBetweenShipments;

    bool shutdown = false;
    uCondition updatingCargo; // wait for bottling plant to update cargo
    unsigned int * stock; // amount of each flavour produced
    unsigned int * cargo; // communication variable
    void productionRun(); // simulates a production run
    void main(); // task main
  public:
	_Event Shutdown {};					// shutdown plant
	BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
				 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
				 unsigned int timeBetweenShipments );
    ~BottlingPlant();
	void getShipment( unsigned int cargo[] );
};

#endif
