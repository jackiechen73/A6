#ifndef BOTTLINGPLANT_H
#define BOTTLINGPLANT_H
#include "printer.h"
#include "nameServer.h"
#include "MPRNG.h"

extern MPRNG mprng;     // access MPRNG object from program main

/***** BottlingPlant *****
 * The BottlingPlant periodically produces sodas 
 * which are picked up by the Truck.
 * The Truck will distribute the sodas to each vending machine.
 ****************************/
_Task BottlingPlant {
        Printer & printer;                  // shared printer
        NameServer & nameServer;            // shared name server
        unsigned int numVendingMachines;    // number of vending machines
        unsigned int maxShippedPerFlavour;  // maximum shipped per flavour
        unsigned int maxStockPerFlavour;    // maximum stock per flavour
        unsigned int timeBetweenShipments;  // delay between shipments

        bool shutdown = false;              // is plant shutdown?
        uCondition updatingCargo;           // wait for main to update cargo
        unsigned int * stock;               // amount of each flavour produced
        unsigned int * cargo;               // truck cargo (communication variable)
        void productionRun();               // simulates a production run
        void main();                        // task main
    public:
        _Event Shutdown {};					// shutdown plant
        BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
                    unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                    unsigned int timeBetweenShipments );
        ~BottlingPlant();
        void getShipment( unsigned int cargo[] ); // update cargo with produced soda
}; // BottlingPlant

#endif // BOTTLINGPLANT_H
