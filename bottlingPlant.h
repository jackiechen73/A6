#ifndef BOTTLINGPLANT_H
#define BOTTLINGPLANT_H
#include "printer.h"
#include "nameServer.h"
#include "MPRNG.h"

extern MPRNG mprng;     // access MPRNG object from program main

_Task BottlingPlant {
    Printer* printer;
    NameServer* nameServer;
    unsigned int numVendingMachines;
    unsigned int maxShippedPerFlavour;
    unsigned int maxStockPerFlavour;
    unsigned int timeBetweenShipments;

    bool waitingForTruck;
    bool shutdown;
    std::unique_ptr<unsigned int[]> stock;
    void main();
  public:
	_Event Shutdown {};					// shutdown plant
	BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
				 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
				 unsigned int timeBetweenShipments );
	void getShipment( unsigned int cargo[] );
};

#endif
