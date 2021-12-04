#ifndef TRUCK_H
#define TRUCK_H
#include "printer.h"
#include "nameServer.h"
#include "bottlingPlant.h"

#include <memory.h>

_Task Truck {
	Printer& printer;
	NameServer& nameServer;
	BottlingPlant& bottlingPlant;
	unsigned int numVendingMachines;
	unsigned int maxStockPerFlavour;

	std::unique_ptr<unsigned int[]> cargo;

	bool isCargoEmpty();
	void main();
  public:
	Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
		   unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};

#endif
