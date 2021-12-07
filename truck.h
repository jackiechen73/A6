#ifndef TRUCK_H
#define TRUCK_H
#include "printer.h"
#include "nameServer.h"
#include "bottlingPlant.h"
#include "MPRNG.h"

extern MPRNG mprng;     // access MPRNG object from program main

/***** Truck *****
 * The Truck picks up a productio run from the BottlingPlant
 * and delivers the sodas to the vending machines (in a fair way).
 * It is created and destructed by the BottlingPlant.
 ****************************/
_Task Truck {
        Printer & printer;                  // shared printer
        NameServer & nameServer;            // shared name server
        BottlingPlant & bottlingPlant;      // shared bottling plant
        unsigned int numVendingMachines;    // number of vending machines
        unsigned int maxStockPerFlavour;    // max stock per flavour

        unsigned int * cargo;       // truck cargo
        bool isCargoEmpty();        // returns if cargo is empty
        unsigned int cargoCount();  // returns sum of cargo
        
        void main();                // task main
    public:
        Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
            unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
        ~Truck();
}; // Truck

#endif // TRUCK_H
