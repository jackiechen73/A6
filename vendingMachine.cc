#include "VendingMachine.h"
#include "nameServer.h"

VendingMachine::VendingMachine(Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost) :
    printer(prt), nameServer(nameServer), id(id), sodaCost(sodaCost), sodaInventory(new unsigned int[NUM_FLAVOURS]{0}) {}

void VendingMachine::main() {
    printer.print(Printer::Kind::Vending, id, 'S'); // start
    nameServer.VMregister(this); // register with name server
    for ( ;; ) {
        try {
            _Accept(~VendingMachine) {
                break;
            } or _Accept(inventory) {
                printer.print(Printer::Vending, id, 'r');
                _Accept(restocked);
                printer.print(Printer::Vending, id, 'R');
            } or _Accept(buy) {}
        } catch (uMutexFailure::RendezvousFailure &) {} // catch accepts that fail
    }
    printer.print(Printer::Kind::Vending, id, 'F'); // start
}

void VendingMachine::buy(Flavours flavour, WATCard & card) {
    if (card.getBalance() < sodaCost) { // sufficient funds?
        _Throw Funds();
    } // if

    if (sodaInventory[flavour] == 0) { // enough stock?
        _Throw Stock();
    } // if

    // free
    if (mprng(4) == 0) { // free drink?
        _Throw Free();
    } else {
        card.withdraw(sodaCost);
    } // if
    sodaInventory[(int) flavour] -= 1;
    printer.print(Printer::Kind::Vending, id, 'B', flavour, sodaInventory[(int) flavour]);
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


