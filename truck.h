#ifndef TRUCK_H
#define TRUCK_H
#include "printer.h"
#include "nameServer.h"
#include "bottlingPlant.h"
#include "MPRNG.h"

extern MPRNG mprng;     // access MPRNG object from program main

_Task Truck {
	Printer & printer;
	NameServer & nameServer;
	BottlingPlant & bottlingPlant;
	unsigned int numVendingMachines;
	unsigned int maxStockPerFlavour;

	unsigned int * cargo;
	bool isCargoEmpty(); // returns if cargo is empty
    unsigned int cargoCount(); // returns sum of cargo
    
	void main(); // task main
  public:
	Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
		   unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
    ~Truck();
};

#endif
