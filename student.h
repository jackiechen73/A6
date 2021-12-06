#ifndef STUDENT_H
#define STUDENT_H
#include "nameServer.h"
#include "WATCardOffice.h"
#include "groupoff.h"
#include "printer.h"
#include "MPRNG.h"

extern MPRNG mprng;     // access MPRNG object from program main

_Task Student {
    private:
        Printer & printer;
        NameServer & nameServer;
        WATCardOffice & cardOffice;
        Groupoff & groupoff;
        unsigned int id;
        unsigned int maxPurchases;
        void main();
    public:
	    Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
			 unsigned int id, unsigned int maxPurchases );
};
#endif
