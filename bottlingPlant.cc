#include "BottlingPlant.h"
#include "truck.h"

BottlingPlant::BottlingPlant(Printer & prt, NameServer & nameServer,
    unsigned int numVendingMachines, unsigned int maxShippedPerFlavour,
    unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments) : 
    printer(prt), nameServer(nameServer), numVendingMachines(numVendingMachines),
    maxShippedPerFlavour(maxShippedPerFlavour), maxStockPerFlavour(maxStockPerFlavour), 
    timeBetweenShipments(timeBetweenShipments), stock(new unsigned int[VendingMachine::NUM_FLAVOURS]) {};

BottlingPlant::~BottlingPlant() {
    delete [] stock;
}

void BottlingPlant::productionRun() {
    yield(timeBetweenShipments);
    unsigned int totalGenerated = 0;
    for (unsigned int i = 0; i < VendingMachine::NUM_FLAVOURS; i++) {
        stock[i] = mprng(maxShippedPerFlavour);
        totalGenerated += stock[i];
    } // for
    printer.print(Printer::Kind::BottlingPlant, 'G', totalGenerated);
}

void BottlingPlant::main() {
    printer.print(Printer::Kind::BottlingPlant, 'S');
    Truck truck(printer, nameServer, *this, numVendingMachines, maxStockPerFlavour);
    for(;;) {
        productionRun();
        try {
            _Enable {
                _Accept(~BottlingPlant) {
                    shutdown = true;
                    _Accept(getShipment); // notify truck about shutdown
                    // this accept should throw rendezvous failure
                } or _Accept(getShipment) { // production run
                    // copy stock to cargo
                    for (unsigned int i = 0; i < VendingMachine::NUM_FLAVOURS; i++) {
                        cargo[i] = stock[i];
                    } // for
                    updatingCargo.signalBlock();
                    printer.print(Printer::Kind::BottlingPlant, 'P');
                } // _Accept
            } // _Enable
        } catch (uMutexFailure::RendezvousFailure &) {
            break;
        } // catch accepts that fail
    } // for
    printer.print(Printer::Kind::BottlingPlant, 'F');
}

void BottlingPlant::getShipment(unsigned int cargo[]) {
    if (shutdown) _Throw( Shutdown() );
    BottlingPlant::cargo = cargo;
    updatingCargo.wait();
}