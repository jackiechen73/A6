#ifndef TRUCK_H
#define TRUCK_H
#include "printer.h"
#include "nameServer.h"
#include "bottlingPlant.h"
#include "MPRNG.h"
#include <memory.h>

extern MPRNG mprng;     // access MPRNG object from program main

_Task Truck {
	Printer& printer;
	NameServer& nameServer;
	BottlingPlant& bottlingPlant;
	unsigned int numVendingMachines;
	unsigned int maxStockPerFlavour;

	unsigned int * cargo;

	bool isCargoEmpty();
    unsigned int cargoCount();
	void main();
  public:
	Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
		   unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};

#endif
