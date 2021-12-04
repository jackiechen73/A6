#include "BottlingPlant.h"


BottlingPlant::BottlingPlant(Printer & prt, NameServer & nameServer,
    unsigned int numVendingMachines, unsigned int maxShippedPerFlavour,
    unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments) : 
    printer(prt), nameServer(nameServer), numVendingMachines(numVendingMachines),
    maxStockPerFlavour(maxStockPerFlavour), timeBetweenShipments(timeBetweenShipments),
    waitingForTruck(false), stock(new unsigned int[VendingMachine::NUM_FLAVOURS]){};

void BottlingPlant::main() {
    /* 
    periodically produces random new
    quantities of each ﬂavour of soda,
    [ 0, MaxShippedPerFlavour ] per ﬂavour.
    */
    for(;;) {
        _Accept(~BottlingPlant) {
            break;
        // wait for truck to get shipment
        } or _When (waitingForTruck) _Accept(getShipment) {
            waitingForTruck = false;
        // production run
        } _Else {
            
        }
    }
   
}


void BottlingPlant::getShipment(unsigned int cargo[]) {

}