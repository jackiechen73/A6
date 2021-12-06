#include "student.h"
#include "WATCard.h"
#include "vendingMachine.h"

Student::Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, 
    Groupoff & groupoff, unsigned int id, unsigned int maxPurchases ) 
    : printer(prt), nameServer(nameServer), cardOffice(cardOffice), 
    groupoff(groupoff), id(id), maxPurchases(maxPurchases) {}

void Student::main() {
    // periodically buy favorite soda
    unsigned int numPurchases = mprng(1, maxPurchases);
    unsigned int favFlavourInt = mprng(VendingMachine::Flavours::NUM_FLAVOURS - 1);
    VendingMachine::Flavours favFlavour = (VendingMachine::Flavours) favFlavourInt;
    printer.print(Printer::Kind::Student, id, 'S', favFlavourInt, numPurchases); // start
    // create watcard 
    WATCard::FWATCard watCard = cardOffice.create( id, 5 );
    // create giftcard from groupoff
    WATCard::FWATCard giftCard = groupoff.giftCard();

    VendingMachine * VM = nameServer.getMachine(id);
    printer.print(Printer::Kind::Student, id, 'V', VM->getId()); // print vending machine id

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

    if (giftCard.available()) { // clean up unused giftcard?
        delete giftCard;
    } // if

    try{
        if (watCard.available()) {
            delete watCard;
        } // if
    } catch (WATCardOffice::Lost &) {} // catch
    
    printer.print(Printer::Kind::Student, id, 'F');
}