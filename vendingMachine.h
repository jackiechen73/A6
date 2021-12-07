#ifndef VENDINGMACHINE_H
#define VENDINGMACHINE_H
#include "printer.h"
#include "WATCard.h"
#include "MPRNG.h"

extern MPRNG mprng;     // access MPRNG object from program main

_Task NameServer;       // forward declaration

/***** VendingMachine *****
 * The VendingMachine sells sodas to the students.
 * Its stock is restocked by the truck.
 * When a student attempts to make a purchase, one of 4 options will happen:
 * 1. their WATCard may have insufficient funds
 * 2. the soda is out of stock
 * 3. the soda is free
 * 4. a regular purchase
 ****************************/
_Task VendingMachine {
        Printer & printer;              // shared printer
        NameServer & nameServer;        // shared name server
        unsigned int id;                // vending machine id
        unsigned int sodaCost;          // cost of each soda
        unsigned int * sodaInventory;   // soda inventory for each flavour

        uCondition purchasing;          // waiting bench for purchasing student
        
        unsigned int requestedFlavour;  // flavour to be purchased
                                        // (communication variable)
        WATCard * buyerCard;            // WATcard of purchaser
                                        // (communication variable)
        enum PurchaseResult {
            SUCCESS,
            FUNDS,
            STOCK,
            FREE
        }; // all possible purchase outcomes
        PurchaseResult purchaseResult;  // result of buy (communication variable)

        void main();                    // task main
    public:
        enum Flavours {
            BLUES_BLACK_CHERRY, 
            CLASSIC_CREAM_SODA, 
            ROCK_ROOT_BEER, 
            JAZZ_LIME, 
            NUM_FLAVOURS 
        }; // flavours of soda
        _Event Free {};						// free, advertisement
        _Event Funds {};					// insufficient funds
        _Event Stock {};					// flavour out of stock
        VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost );
        ~VendingMachine();
        void buy( Flavours flavour, WATCard & card );   // make a purchase
        unsigned int * inventory();                     // returns stock
        void restocked(); // notify machine that it is restocked
        _Nomutex unsigned int cost() const;             // get sodaCost
        _Nomutex unsigned int getId() const;            // get machine id
}; // VendingMachine

#endif // VENDINGMACHINE_H
