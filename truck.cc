#include "truck.h"
#include "vendingMachine.h"

Truck::Truck(Printer & prt, NameServer & nameServer, BottlingPlant & plant,
    unsigned int numVendingMachines, unsigned int maxStockPerFlavour) : 
    printer(prt), nameServer(nameServer), bottlingPlant(plant),
    numVendingMachines(numVendingMachines), maxStockPerFlavour(maxStockPerFlavour),
    cargo(new unsigned int[VendingMachine::NUM_FLAVOURS]) {}

void Truck::main() {
    // get array of vending machine pointers
    VendingMachine** machineList = nameServer.getMachineList();
    unsigned int curMachine = 0;
    for (;;) {
        _Accept(~Truck) {
            break;
        } _Else {
            // yield to get coffee
            yield(1,10);
            try {
                bottlingPlant -> getShipment(cargo);
            } catch (BottlingPlant::Shutdown &) {
                // shut down
            }
            // call inventory() to return a pointer to an array container the amount of each soda
            // transfer as much of each kind of soda
            unsigned int startingMachine = curMachine
            for (; curMachine = (curMachine + 1) % numVendingMachines; curMachine != startingMachine) {
                unsigned int *inventory = machineList[currentMachine] -> inventory();
                // transfer stuffs
                for (unsigned int i = 0; i < VendingMachine::NUM_FLAVOURS; i++) {
                    unsigned requestAmount = maxStockPerFlavour - inventory[i];
                    
                    // if request amount exceeds the amount in cargo, empty the cargo
                    if (requestAmount > cargo[i]) {
                        inventory[i] += cargo[i];
                        cargo[i] = 0;
                    } else {
                        invectory[i] += requestAmount;
                        cargo -= requestAmount;
                    }
                }
                machineList[currentMachine] -> restocked();

                // flat tire
                if (mprng(100) == 0) yield(10);

                // check if truck is empty
                if (isCargoEmpty()) break;
            }
        }
    }
}

bool Truck::isCargoEmpty() {
    for (unsigned int i = 0; i < VendingMachines::NUM_FLAVOURS; i++) {
        if (cargo[i] == 0) return false;
    }
    return true;
}
