#include "VendingMachine.h"
#include "nameServer.h"

VendingMachine::VendingMachine(Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost) :
    printer(prt), nameServer(nameServer), id(id), sodaCost(sodaCost), sodaInventory(new unsigned int[NUM_FLAVOURS]{0}) {}

VendingMachine::~VendingMachine() {
    delete [] sodaInventory;
}

void VendingMachine::main() {
    printer.print(Printer::Kind::Vending, id, 'S', sodaCost); // start
    nameServer.VMregister(this); // register with name server
    for ( ;; ) {
        try {
            _Accept(~VendingMachine) {
                break;
            } or _Accept(inventory) {
                printer.print(Printer::Vending, id, 'r');
                _Accept(restocked);
                printer.print(Printer::Vending, id, 'R');
            } or _Accept(buy) {
                if (buyerCard->getBalance() < sodaCost) { // sufficient funds?
                    purchaseResult = FUNDS;
                } else if (sodaInventory[requestedFlavour] == 0) { // enough stock?
                    purchaseResult = STOCK;
                } else if (mprng(4) == 0) { // free drink?
                    purchaseResult = FREE;
                    sodaInventory[requestedFlavour] -= 1;
                    printer.print(Printer::Kind::Vending, id, 'A');
                } else {
                    purchaseResult = SUCCESS;
                    buyerCard->withdraw(sodaCost);
                    sodaInventory[requestedFlavour] -= 1;
                    printer.print(Printer::Kind::Vending, id, 'B', requestedFlavour, sodaInventory[requestedFlavour]);
                } // if
                purchasing.signalBlock();
            }
        } catch (uMutexFailure::RendezvousFailure &) {} // catch accepts that fail
    } // for
    printer.print(Printer::Kind::Vending, id, 'F'); // finish
}

void VendingMachine::buy(Flavours flavour, WATCard & card) {
    requestedFlavour = (unsigned int) flavour;
    buyerCard = &card;
    purchasing.wait();
    switch (purchaseResult) {
        case PurchaseResult::SUCCESS:
            break;
        case PurchaseResult::FUNDS:
            throw Funds();
        case PurchaseResult::FREE:
            throw Free();
        case PurchaseResult::STOCK:
            throw Stock();
    }    
}

unsigned int *  VendingMachine::inventory() {
    return sodaInventory;
}

void VendingMachine::restocked() {}

_Nomutex unsigned int VendingMachine::cost() const {
    return sodaCost;
}

_Nomutex unsigned int VendingMachine::getId() const {
    return id;
}


