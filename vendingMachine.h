#ifndef VENDINGMACHINE_H
#define VENDINGMACHINE_H
#include "printer.h"
#include "WATCard.h"
#include "MPRNG.h"

extern MPRNG mprng;     // access MPRNG object from program main

_Task NameServer; // forward declaration

_Task VendingMachine {
	Printer& printer;
	NameServer& nameServer;
	unsigned int id;
	unsigned int sodaCost;
	unsigned int * sodaInventory;
	void main();
  public:
	enum Flavours { 
        BLUES, 
        BLACK_CHERRY, 
        CLASSIC, 
        CREAM_SODA, 
        ROCK_ROOT_BEER, 
        JAZZ_LIME, 
        NUM_FLAVOURS 
    }; // flavours of soda (YOU DEFINE)
	_Event Free {};						// free, advertisement
	_Event Funds {};					// insufficient funds
	_Event Stock {};					// flavour out of stock
	VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost );
	void buy( Flavours flavour, WATCard & card );
	unsigned int * inventory();
	void restocked();
	_Nomutex unsigned int cost() const;
	_Nomutex unsigned int getId() const;
};

#endif
