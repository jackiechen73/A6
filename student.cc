#include "student.h"
#include "WATCard.h"
#include "vendingMachine.h"

/***** Student::Student *****
 * This is the constructor of the Student class.
 * printer: shared printer
 * nameServer: shared name server
 * cardOffice: shared WATCard office
 * groupoff: shared groupoff
 * id: student id
 * maxPurchases: maximum number of purchases
 ****************************/
Student::Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, 
    Groupoff & groupoff, unsigned int id, unsigned int maxPurchases ) 
    : printer( prt ), nameServer( nameServer ), cardOffice( cardOffice ), 
    groupoff( groupoff ), id( id ), maxPurchases( maxPurchases ) {}

/***** Student::main *****
 * This is the task main.
 * Students periodically make purchases using their giftcard or WATCard.
 * Students may encounter their WATCard being lost, insufficient funds, 
 * vending machine out of stock, and free drinks.
 ****************************/
void Student::main() {
    // generate number of purchases
    unsigned int numPurchases = mprng(1, maxPurchases);
    // select favourite flavour
    unsigned int favFlavourInt = mprng(VendingMachine::Flavours::NUM_FLAVOURS - 1);
    VendingMachine::Flavours favFlavour = (VendingMachine::Flavours) favFlavourInt;
    // start message
    printer.print(Printer::Kind::Student, id, 'S', favFlavourInt, numPurchases); 
    // create watcard 
    WATCard::FWATCard watCard = cardOffice.create( id, 5 );
    // create giftcard from groupoff
    WATCard::FWATCard giftCard = groupoff.giftCard();

    VendingMachine * VM = nameServer.getMachine(id); // get vending machine
    printer.print(Printer::Kind::Student, id, 'V', VM->getId()); // machine id

    nextpurchase: for (unsigned int i = 0; i < numPurchases; i += 1 ) {
        yield(mprng(1, 10));
        tryPurchase: for ( ;; ) {
            _Select( giftCard ) { // giftcard future available?
                try {
                    _Enable {
                        VM->buy(favFlavour, *(giftCard()));
                        printer.print(Printer::Kind::Student, id, 'G', favFlavourInt, (*giftCard).getBalance());
                        delete giftCard; // delete watcard object inside future
                        giftCard.reset();
                        continue nextpurchase;
                    } // _Enable
                } catch (VendingMachine::Stock &) {
                    // get new vending machine id
                    VM = nameServer.getMachine(id);
                    printer.print(Printer::Kind::Student, id, 'V', VM->getId());
                    yield(mprng(1, 10));
                } catch (VendingMachine::Free &) {
                    printer.print(Printer::Kind::Student, id, 'a', favFlavourInt, (*giftCard).getBalance());
                } // try
                continue tryPurchase;
            } or _Select( watCard ) { // watcard future available?
                try {
                    _Enable {
                        VM->buy(favFlavour, *(watCard())); // try to access the watcard and buy
                        printer.print(Printer::Kind::Student, id, 'B', favFlavour, (*watCard).getBalance());
                        continue nextpurchase;
                    } // _Enable
                } catch (WATCardOffice::Lost &) {
                    printer.print(Printer::Kind::Student, id, 'L');
                    watCard = cardOffice.create(id, 5);
                } catch (VendingMachine::Funds &) {
                    watCard = cardOffice.transfer(id, VM->cost() + 5, watCard);
                    yield(mprng(1, 10));
                } catch (VendingMachine::Stock &) {
                    // get new vendingmachine
                    VM = nameServer.getMachine(id);
                    printer.print(Printer::Kind::Student, id, 'V', VM->getId());
                    yield(mprng(1, 10));
                } catch (VendingMachine::Free &) {
                    printer.print(Printer::Kind::Student, id, 'A', favFlavourInt, (*watCard).getBalance());
                } // try
                continue tryPurchase;
            } // _Select
        } // for
    } // for
    
    // wait for WATCard transactions to complete and delete
    try{
        delete watCard();
    } catch (WATCardOffice::Lost &) {} // catch
    
    printer.print(Printer::Kind::Student, id, 'F');
}