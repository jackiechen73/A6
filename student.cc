#include "student.h"

Student::Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, 
    Groupoff & groupoff, unsigned int id, unsigned int maxPurchases ) 
    : printer(prt), nameServer(nameServer), cardOffice(cardOffice), 
    groupoff(groupoff), id(id), maxPurchases(maxPurchases) {}

void Student::main() {
    // periodically buy favorite soda
    unsigned int numPurchases = mprng(1, maxPurchases);
    unsigned int favFlavour = mprng(Flavours::NUM_FLAVOURS - 1);
    printer.print(Printer::Kind::Student, id, 'S', favFlavour, numPurchases);

    // create watcard 
    WATCard::FWATCard watCard = cardOffice.create( id, 5 );
    // create giftcard from groupoff
    WATCard::FWATCard giftCard = groupoff.giftCard();

    VendingMachine* VM = nameServer.getMachine(id);
    printer.print(Printer::Kind::Student, id, 'V', VM->getId());

    nextpurchase: for (int i = 0; i < numPurchases; i += 1 ) {
        yield(mprng(1, 10));
        busywait: for ( ;; ) {
            _Select(giftCard) {
                try {
                    _Enable {
                        VM->buy(favFlavour, *giftCard);
                        printer.print(Printer::Kind::Student, id, 'G', favFlavour, giftCard.getBalance());
                        giftCard.reset();
                        continue nextpurchase;
                    } // _Enable
                } catch (VendingMachine::Stock &) {
                    // get new vendingmachine
                    VM = nameServer.getMachine(id);
                    printer.print(Printer::Kind::Student, id, 'V', VM->getId());
                    yield(mprng(1, 10));
                } catch (VendingMachine::Free &) {
                    printer.print(Printer::Kind::Student, id, 'a', favFlavour, giftCard.getBalance());
                } // try
                continue busywait;
            } or _Select(watCard) {
                try {
                    _Enable {
                        VM->buy(flavour, *watcard());
                        printer.print(Printer::Kind::Student, id, 'B', favFlavour, giftCard.getBalance());
                        continue nextpurchase;
                    } // _Enable
                } catch (WATCardOffice::Lost &) {
                    printer.print(Printer::Kind::Student, id, 'L');
                    watcard = cardOffice.create(id, 5);
                } catch (VendingMachine::Funds &) {
                    watCard = cardOffice.transfer(id, vendingMachine->cost() + 5, watCard);
                    yield(mprng(1, 10));
                } catch (VendingMachine::Stock &) {
                    // get new vendingmachine
                    VM = nameServer.getMachine(id);
                    printer.print(Printer::Kind::Student, id, 'V', VM->getId());
                    yield(mprng(1, 10));
                } catch (VendingMachine::Free &) {
                    printer.print(Printer::Kind::Student, id, 'A', favFlavour, giftCard.getBalance());
                } // try
                continue busywait;
            } // _Select
        } // for
    } // for
    printer.print(Printer::Kind::Student, id, 'F');
}