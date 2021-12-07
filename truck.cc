#include "truck.h"
#include "vendingMachine.h"

/***** Truck::Truck *****
 * This is the constructor of the Truck class.
 * printer: shared printer
 * nameServer: shared name server
 * plant: shared bottling plant
 * numVendingMachines: number of vending machines
 * maxStockPerFlavour: maximum stock per flavour in each vending machine
 ****************************/
Truck::Truck(Printer & prt, NameServer & nameServer, BottlingPlant & plant,
    unsigned int numVendingMachines, unsigned int maxStockPerFlavour) 
    : printer( prt ), nameServer( nameServer ), bottlingPlant( plant ),
    numVendingMachines( numVendingMachines ), maxStockPerFlavour( maxStockPerFlavour ),
    cargo( new unsigned int[VendingMachine::NUM_FLAVOURS]{0} ) {}

/***** Truck::~Truck *****
 * This is the destructor of the Truck class.
 ****************************/
Truck::~Truck() {
    delete [] cargo;
}

/***** Truck::main *****
 * This is the main task of the Truck class.
 * It periodically gets a shipment from the BottlingPlant
 * and delivers the sodas to the machines.
 * It attempts to cycle through the machines to deliver fairly.
 ****************************/
void Truck::main() {
    printer.print( Printer::Kind::Truck, 'S' ); // start
    // get array of vending machine pointers
    VendingMachine** machineList = nameServer.getMachineList();
    unsigned int curMachine = 0;
    for (;;) {
        // yield to get coffee
        yield( mprng( 1, 10 ) );
        try {
            _Enable {
                bottlingPlant.getShipment( cargo );
            } // _Enable
        } catch ( BottlingPlant::Shutdown & ) { // plant shut down?
            break;
        } // try
        // print the count of cargo picked up
        printer.print( Printer::Kind::Truck, 'P', cargoCount() ); 
        
        // transfer as much of each kind of soda to each machine
        unsigned int startingMachine = curMachine;
        for ( ;; ) {
            // begin delivery
            printer.print( Printer::Kind::Truck, 'd', curMachine, cargoCount() );
            // get inventory of curMachine
            unsigned int * inventory = machineList[curMachine]->inventory();
            
            // transfer as much soda as possible
            unsigned int unsuccessful = 0; // number of unfilled inventory
            for ( unsigned int i = 0; i < VendingMachine::NUM_FLAVOURS; i++ ) {
                unsigned requestAmount = maxStockPerFlavour - inventory[i];
                // if request amount exceeds the amount in cargo, empty the cargo
                if ( requestAmount > cargo[i] ) { // not enough cargo?
                    unsuccessful += requestAmount - cargo[i];
                    inventory[i] += cargo[i];
                    cargo[i] = 0;
                } else {
                    inventory[i] += requestAmount;
                    cargo[i] -= requestAmount;
                } // if
            } // for
            if ( unsuccessful > 0 ) { // unfilled inventory?
                printer.print( Printer::Kind::Truck, 'U', curMachine, unsuccessful );
            } // if

            // end delivery
            printer.print( Printer::Kind::Truck, 'D', curMachine, cargoCount() ); 
            machineList[curMachine]->restocked(); // notify vending machine restocking is finished

            if (mprng(99) == 0) { // flat tire?
                printer.print(Printer::Kind::Truck, 'X'); // flat tire message
                yield(10); // yield for flat tire
            }

            if ( isCargoEmpty() ) break; // need to restock if truck is empty

            curMachine = (curMachine + 1) % numVendingMachines;
            if (curMachine == startingMachine) break;
        } // for
    } // for
    printer.print(Printer::Kind::Truck, 'F'); // finish
}

/***** Truck::isCargoEmpty *****
 * Return true if the cargo is empty.
 * Otherwise, return false.
 ****************************/
bool Truck::isCargoEmpty() {
    for ( unsigned int i = 0; i < VendingMachine::NUM_FLAVOURS; i++ ) {
        if ( cargo[i] > 0 ) return false; // cargo not empty
    } // for
    return true; // cargo empty
}

/***** Truck::cargoCount *****
 * Return total count of sodas in cargo.
 ****************************/
unsigned int Truck::cargoCount() {
    unsigned int count = 0; // track total count
    for (unsigned int i = 0; i < VendingMachine::NUM_FLAVOURS; i++) {
        count += cargo[i];
    } // for
    return count;
}
