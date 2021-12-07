#ifndef STUDENT_H
#define STUDENT_H
#include "nameServer.h"
#include "WATCardOffice.h"
#include "groupoff.h"
#include "printer.h"
#include "MPRNG.h"

extern MPRNG mprng;     // access MPRNG object from program main

/***** Student *****
 * The Student periodically tries to buy their favourite drink 
 * from a vending machine.
 * Students can make purchases using their giftcard (preferably) or WATCard.
 * Students must deal with their WATCard being lost, insufficient funds, 
 * vending machine out of stock, and free drinks.
 ****************************/
_Task Student {
    private:
        Printer & printer;          // shared printer
        NameServer & nameServer;    // shared name server
        WATCardOffice & cardOffice; // shared WATCard office
        Groupoff & groupoff;        // shared groupoff
        unsigned int id;            // student id
        unsigned int maxPurchases;  // max number of purchases
        void main();                // task main
    public:
	    Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
			 unsigned int id, unsigned int maxPurchases );
};
#endif
