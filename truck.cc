#include "truck.h"
#include "vendingMachine.h"

Truck::Truck(Printer & prt, NameServer & nameServer, BottlingPlant & plant,
    unsigned int numVendingMachines, unsigned int maxStockPerFlavour) : 
    printer(prt), nameServer(nameServer), bottlingPlant(plant),
    numVendingMachines(numVendingMachines), maxStockPerFlavour(maxStockPerFlavour),
    cargo(new unsigned int[VendingMachine::NUM_FLAVOURS]{0}) {}

Truck::~Truck() {
    delete [] cargo;
}

void Truck::main() {
    printer.print(Printer::Kind::Truck, 'S');
    // get array of vending machine pointers
    VendingMachine** machineList = nameServer.getMachineList();
    unsigned int curMachine = 0;
    for (;;) {
        // yield to get coffee
        yield( mprng( 1, 10 ) );
        try {
            _Enable {
                bottlingPlant.getShipment(cargo);
            } // _Enable
        } catch (BottlingPlant::Shutdown &) { // shut down
            break;
        } // try
        printer.print(Printer::Kind::Truck, 'P', cargoCount());
        
        // transfer as much of each kind of soda to each machine
        unsigned int startingMachine = curMachine;
        for ( ;; ) {
            printer.print(Printer::Kind::Truck, 'd', curMachine, cargoCount()); // begin delivery
            // get inventory of curMachine
            unsigned int * inventory = machineList[curMachine]->inventory();
            // transfer as much soda as possible
            unsigned int unsuccessful = 0; // number of unfilled inventory
            for ( unsigned int i = 0; i < VendingMachine::NUM_FLAVOURS; i++ ) {
                unsigned requestAmount = maxStockPerFlavour - inventory[i];
                // if request amount exceeds the amount in cargo, empty the cargo
                if (requestAmount > cargo[i]) { // not enough cargo?
                    unsuccessful += requestAmount - cargo[i];
                    inventory[i] += cargo[i];
                    cargo[i] = 0;
                } else {
                    inventory[i] += requestAmount;
                    cargo[i] -= requestAmount;
                } // if
            } // for
            if ( unsuccessful > 0 ) {
                printer.print(Printer::Kind::Truck, 'U', curMachine, unsuccessful);
            } // if
            printer.print(Printer::Kind::Truck, 'D', curMachine, cargoCount()); // end delivery

            machineList[curMachine]->restocked(); // notify vending machine restocking is finished

            if (mprng(99) == 0) {
                printer.print(Printer::Kind::Truck, 'X');
                yield(10); // yield for flat tire
            }

            if (isCargoEmpty()) break; // restock if truck is empty

            curMachine = (curMachine + 1) % numVendingMachines;
            if (curMachine == startingMachine) break;
        } // for
    } // for
    printer.print(Printer::Kind::Truck, 'F');
}

bool Truck::isCargoEmpty() {
    for (unsigned int i = 0; i < VendingMachine::NUM_FLAVOURS; i++) {
        if (cargo[i] > 0) return false;
    } // for
    return true;
}

unsigned int Truck::cargoCount() {
    unsigned int count = 0;
    for (unsigned int i = 0; i < VendingMachine::NUM_FLAVOURS; i++) {
        count += cargo[i];
    } // for
    return count;
}
