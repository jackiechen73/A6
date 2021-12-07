#include "vendingMachine.h"
#include "nameServer.h"

/***** VendingMachine::VendingMachine *****
 * This is the constructor of the VendingMachine class.
 * printer: shared printer
 * nameServer: shared name server
 * id: vending machine id
 * sodaCost: cost of each soda
 ****************************/
VendingMachine::VendingMachine(Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost) 
    : printer( prt ), nameServer( nameServer ), id( id ), 
    sodaCost( sodaCost ), sodaInventory( new unsigned int[NUM_FLAVOURS]{0} ) {}

/***** VendingMachine::~VendingMachine *****
 * This is the destructor of the VendingMachine class.
 ****************************/
VendingMachine::~VendingMachine() {
    delete [] sodaInventory;
}

/***** VendingMachine::main *****
 * This is the task main of the VendingMachine class.
 * The vending machine will be periodically restocked by the truck.
 * It must also accept purchases from students. Purchases have 4 outcomes:
 * 1. WATCard may have insufficient funds
 * 2. the soda is out of stock
 * 3. the soda is free
 * 4. a regular purchase
 ****************************/
void VendingMachine::main() {
    printer.print( Printer::Kind::Vending, id, 'S', sodaCost ); // start
    nameServer.VMregister( this ); // register with name server
    for ( ;; ) {
        try {
            _Accept( ~VendingMachine ) {
                break;
            } or _Accept( inventory ) {
                printer.print(Printer::Vending, id, 'r'); // start reload 
                _Accept( restocked );
                printer.print(Printer::Vending, id, 'R'); // finish reload
            } or _Accept( buy ) {
                if ( buyerCard->getBalance() < sodaCost ) { 
                    // insufficient funds
                    purchaseResult = FUNDS;
                } else if ( sodaInventory[requestedFlavour] == 0 ) { 
                    // not enoguh stock
                    purchaseResult = STOCK;
                } else if ( mprng(4) == 0 ) { 
                    // free drink
                    purchaseResult = FREE;
                    sodaInventory[requestedFlavour] -= 1;
                    printer.print( Printer::Kind::Vending, id, 'A' ); // free soda
                } else {
                    // regular purchase
                    purchaseResult = SUCCESS;
                    buyerCard->withdraw( sodaCost );
                    sodaInventory[requestedFlavour] -= 1;
                    // bought soda
                    printer.print( Printer::Kind::Vending, id, 'B', requestedFlavour, sodaInventory[requestedFlavour] );
                } // if
                purchasing.signalBlock(); // let buy call finish
            } // _Accept          
        } catch ( uMutexFailure::RendezvousFailure & ) {} // catch accepts that throw exceptions
    } // for
    printer.print(Printer::Kind::Vending, id, 'F'); // finish
}

/***** VendingMachine::buy *****
 * Attempts to purchase the specified flavour using the specified WATCard.
 * This function may throw the following:
 * Funds - insufficient funds in the WATCard
 * Free - free soda if advertisement is watched
 * Stock - desired flavour is out of stock
 * flavour: the desired flavour
 * card: WATCard to make purchase
 ****************************/
void VendingMachine::buy( Flavours flavour, WATCard & card ) {
    requestedFlavour = ( unsigned int ) flavour;
    buyerCard = &card;
    purchasing.wait();
    switch ( purchaseResult ) {
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

/***** VendingMachine::inventory *****
 * Returns the inventory of the vending machine.
 * Called by Truck right before loading its cargo into the machine's inventory.
 ****************************/
unsigned int *  VendingMachine::inventory() {
    return sodaInventory;
}

/***** VendingMachine::restocked *****
 * Notifies the vending machine that it has been restocked.
 ****************************/
void VendingMachine::restocked() {}

/***** VendingMachine::cost *****
 * Returns cost of each soda.
 * This is a _Nomutex call.
 ****************************/
_Nomutex unsigned int VendingMachine::cost() const {
    return sodaCost;
}

/***** VendingMachine::getId *****
 * Returns the id of the machine.
 * This is a _Nomutex call.
 ****************************/
_Nomutex unsigned int VendingMachine::getId() const {
    return id;
}


