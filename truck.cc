#include "truck.h"
#include "vendingMachine.h"

Truck::Truck(Printer & prt, NameServer & nameServer, BottlingPlant & plant,
    unsigned int numVendingMachines, unsigned int maxStockPerFlavour) : 
    printer(prt), nameServer(nameServer), bottlingPlant(plant),
    numVendingMachines(numVendingMachines), maxStockPerFlavour(maxStockPerFlavour),
    cargo(new unsigned int[VendingMachine::NUM_FLAVOURS]{0}) {}

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
        for ( ; curMachine != startingMachine; curMachine = (curMachine + 1) % numVendingMachines ) {
            printer.print(Printer::Kind::Truck, 'd', curMachine, cargoCount());
            // get inventory of curMachine
            unsigned int *inventory = machineList[curMachine] -> inventory();
            // transfer as much soda as possible
            unsigned int unsuccessful = 0;
            for ( unsigned int i = 0; i < VendingMachine::NUM_FLAVOURS; i++ ) {
                unsigned requestAmount = maxStockPerFlavour - inventory[i];
                // if request amount exceeds the amount in cargo, empty the cargo
                if (requestAmount > cargo[i]) {
                    inventory[i] += cargo[i];
                    cargo[i] = 0;
                } else {
                    unsuccessful += requestAmount - cargo[i];
                    inventory[i] += requestAmount;
                    cargo[i] -= requestAmount;
                }
            }
            if ( unsuccessful > 0 ) {
                printer.print(Printer::Kind::Truck, 'U', curMachine, unsuccessful);
            } // if
            printer.print(Printer::Kind::Truck, 'D', curMachine, cargoCount());
            machineList[curMachine]->restocked();

            // flat tire
            if (mprng(99) == 0) yield(10);

            // check if truck is empty
            if (isCargoEmpty()) break;
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
